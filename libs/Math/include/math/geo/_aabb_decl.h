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
template <typename T = f64>
struct AABBox {
	using Vec3 = Vector<3,T>;

	/** The smallest x, y, and z coordinates of the box. */
	Vec3 min;
	/** The largest x, y, and z coordinates of the box. */
	Vec3 max;


	// Constructors -------------------------------------------------------------

	/** Declare an uninitialized bounding box. */
	constexpr AABBox() = default;

	/** Create a bounding box with pre-defined bounds. */
	constexpr AABBox(const Vec3& min, const Vec3& max);

	/** Create a bounding box that contains no points. */
	static constexpr auto empty() -> AABBox;


	// Builder methods ----------------------------------------------------------

	/** Collapse the bounding box so that it contains no points. */
	auto clear() -> AABBox&;

	/** Expand the bounding box to contain the given point. */
	auto add(const Vec3& point) -> AABBox&;

	/** Expand the bounding box to contain all the given points. */
	auto add(std::initializer_list<Vec3> points) -> AABBox&;

	/** Expand the bounding box to contain all the given points. */
	template <typename Iter>
	auto add(Iter points) -> ENABLE_IF(ITER_OF(Iter, Vec3), AABBox&);


	// Derived properties -------------------------------------------------------

	/** Get the center point of the bounding box. */
	constexpr auto center() const -> Vec3;

	/** Get the three-dimensional size of the bounding box. */
	constexpr auto size() const -> Vec3;

	/** Get the distance from the center point to the sides along each axis. */
	constexpr auto radius() const -> Vec3;


	// Collision testing --------------------------------------------------------

	/** Test whether a point lies within the box's bounds. */
	auto contains(const Vec3& point) const -> bool;


	// Transformation -----------------------------------------------------------

	/** Transform the bounding box to another coordinate space. */
	template <usize R, usize C>
	auto transform(const Matrix<R,C,T>& m) const -> AABBox;
};

} // namespace geo
} // namespace math
