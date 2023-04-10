#pragma once

#include <sized.h>

#include "math/sfinae.h"
#include "math/vector.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

namespace geo {

template <typename T = f64>
struct Plane {
	using Vec3 = Vector<3,T>;

	/** The direction of the plane's surface */
	Vec3 normal;
	/** Distance from the origin in the direction of `normal` */
	T distance;


	// Constructors -------------------------------------------------------------

	constexpr Plane() = default;
	constexpr Plane(const Vec3& normal, T distance);

	/** Create a plane from three coplanar, non-colinear points. */
	static constexpr auto from_points(const Vec3& p1, const Vec3& p2, const Vec3& p3) -> Plane;

	/** Create a best-fit plane from a collection of points. */
	template <typename Iter>
	static constexpr auto best_fit(Iter points) -> ENABLE_IF(ITER_OF(Iter, Vec3), Plane);


	// Queries ------------------------------------------------------------------

	/**
	 * Compute the signed distance from the point to the plane. Result will be
	 * negative if the point is on the back side of the plane.
	 */
	constexpr auto dist(const Vec3& point) const -> T;
};

} // namespace geo
} // namespace math
