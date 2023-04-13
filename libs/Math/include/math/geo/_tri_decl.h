#pragma once

#include <sized.h>

#include "math/matrix.h"
#include "math/vector.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

namespace geo {

struct Circle;


struct Tri {
	Vec3 v1;
	Vec3 v2;
	Vec3 v3;


	// Derived properties -------------------------------------------------------

	/** Get the edge at the 1-based index indicated by the template argument. */
	template <usize Index>
	constexpr auto edge() const -> Vec3;

	/** Get the edge at the provided 1-based index. */
	constexpr auto edge(usize index) const -> Vec3;

	/** Compute the perimeter length of the triangle. */
	constexpr auto perimeter() const -> flt;

	/** Compute the surface area of the triangle. */
	constexpr auto area() const -> flt;

	/** Compute the point at the triangle's center of gravity */
	constexpr auto centroid() const -> Vec3;

	/** Compute the point that's equidistant from all sides of the triangle. */
	constexpr auto incenter() const -> Vec3;

	/** Compute the circle that's tangent to all edges of the triangle. */
	constexpr auto inscribed_circle() const -> Circle;

	/** Compute the point that's equidistant from all vertices of the triangle. */
	constexpr auto circumcenter() const -> Vec3;

	/** Compute the circle that's coincident with all vertices of the triangle. */
	constexpr auto circumscribed_circle() const -> Circle;


	// Barycentric <-> Cartesian coordinates ------------------------------------

	/**
	 * Get a Mat3x3 that can transform barycentric coordinates into cartesian
	 * coordinates.
	 */
	constexpr auto bary2cart() const -> Matrix<3,3>;

	/** Get the cartesian point for the given barycentric coordinates. */
	constexpr auto bary2cart(flt x, flt y, flt z) const -> Vec3;
	/** Get the cartesian point for the given barycentric coordinates. */
	constexpr auto bary2cart(const Vec3& coords) const -> Vec3;

	/**
	 * Get the barycentric coordinates for the given point projected onto the
	 * surface of the triangle.
	 */
	auto cart2bary(flt x, flt y, flt z) const -> Vec3;
	/**
	 * Get the barycentric coordinates for the given point projected onto the
	 * surface of the triangle.
	 */
	auto cart2bary(const Vec3& p) const -> Vec3;
};

} // namespace geo
} // namespace math
