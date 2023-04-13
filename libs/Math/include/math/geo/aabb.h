#pragma once

#include <initializer_list>
#include <type_traits>

#include <sized.h>

#include "math/sfinae.h"
#include "math/matrix.h"
#include "math/vector.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

namespace geo {

/**
 * An axis-aligned bounding-box.
 */
struct AABBox {
	/** The smallest x, y, and z coordinates of the box. */
	Vec3 min;
	/** The largest x, y, and z coordinates of the box. */
	Vec3 max;


	// Constructors -------------------------------------------------------------

	/** Declare an uninitialized bounding box. */
	constexpr AABBox() = default;

	/** Create a bounding box with pre-defined bounds. */
	constexpr AABBox(const Vec3& min, const Vec3& max)
		: min(min)
		, max(max)
	{}

	/** Create a bounding box that contains no points. */
	static constexpr auto empty() -> AABBox {
		return AABBox{
			Vec3::all(std::numeric_limits<flt>::infinity()),
			Vec3::all(-std::numeric_limits<flt>::infinity()),
		};
	}


	// Builder methods ----------------------------------------------------------

	/** Collapse the bounding box so that it contains no points. */
	auto clear() -> AABBox& {
		min = Vec3::all(std::numeric_limits<flt>::infinity());
		max = Vec3::all(-std::numeric_limits<flt>::infinity());

		return *this;
	}

	/** Expand the bounding box to contain the given point. */
	auto add(const Vec3& point) -> AABBox& {
		min.x = std::min(min.x, point.x);
		min.y = std::min(min.y, point.y);
		min.z = std::min(min.z, point.z);

		max.x = std::max(max.x, point.x);
		max.y = std::max(max.y, point.y);
		max.z = std::max(max.z, point.z);

		return *this;
	}

	/** Expand the bounding box to contain all the given points. */
	auto add(std::initializer_list<Vec3> points) -> AABBox& {
		for (const auto& p : points)
			add(p);

		return *this;
	}

	/** Expand the bounding box to contain all the given points. */
	template <typename Iter>
	auto add(const Iter& points) -> ENABLE_IF(ITER_OF(Iter, Vec3), AABBox&) {
		for (const auto& p : points)
			add(p);

		return *this;
	}


	// Derived properties -------------------------------------------------------

	/** Get the center point of the bounding box. */
	constexpr auto center() const -> Vec3 {
		return 0.5 * (min + max);
	}

	/** Get the three-dimensional size of the bounding box. */
	constexpr auto size() const -> Vec3 {
		return max - min;
	}

	/** Get the distance from the center point to the sides along each axis. */
	constexpr auto radius() const -> Vec3 {
		return 0.5 * (max - min);
	}


	// Collision testing --------------------------------------------------------

	/** Test whether a point lies within the box's bounds. */
	auto contains(const Vec3& p) const -> bool {
		return p.x >= min.x && p.x <= max.x
		    && p.y >= min.y && p.y <= max.y
		    && p.z >= min.z && p.z <= max.z;
	}


	// Transformation -----------------------------------------------------------

	/** Transform the bounding box to another coordinate space. */
	template <usize R, usize C>
	auto transform(const Matrix<R,C>& m) const -> AABBox {
		static_assert(C >= 3 && C <= 4 && R == 4, "Expected a 4x3 or 4x4 matrix");

		// Start with a zero-sized box at the translation portion of the matrix.
		auto origin = Vec3{ m.m31, m.m32, m.m33 };
		auto result = AABBox{ origin, origin };

		// Set the new min and max coords by determining the smallest and largest
		// possible products for each x, y, and z coordinate
		for (usize r = 0; r < R; ++r) {
			for (usize c = 0; c < 3; ++c) {
				flt mm = m[r][c];

				if (mm > 0) {
					result.min[c] += mm * min[c];
					result.max[c] += mm * max[c];
				} else {
					result.min[c] += mm * max[c]; // Note the inversion of the factors
					result.max[c] += mm * min[c]; // compared to the previous branch
				}
			}
		}

		return result;
	}
};

} // namespace geo
} // namespace math
