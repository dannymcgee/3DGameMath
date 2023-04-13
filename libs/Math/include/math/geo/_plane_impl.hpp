#pragma once

#include "math/geo/_plane_decl.h"

#include <cmath>

#include "math/assert.h"

#ifndef NDEBUG
	#include "math/utility.h"
#endif

namespace math::geo {

// Constructors ----------------------------------------------------------------

inline Plane::Plane() {} // NOLINT

constexpr Plane::Plane(const Vec3& normal, flt distance)
	: normal(normal)
	, distance(distance)
{}

constexpr auto Plane::from_points(
	const Vec3& p1,
	const Vec3& p2,
	const Vec3& p3)
-> Plane
{
	Vec3 e3 = p2 - p1;
	Vec3 e1 = p3 - p2;

	Vec3 perp = e3 ^ e1;
	flt sq_length = perp.sq_length();

	ASSERT(!math::nearly_equal(sq_length, 0, 0.001),
		"Cannot construct a plane from nearly colinear points:\n\t{}\n\t{}\n\t{}\n",
		p1.to_string(),
		p2.to_string(),
		p3.to_string());

	Vec3 normal = perp * (1 / std::sqrt(sq_length));
	flt distance = p1 | normal;

	return Plane{ normal, distance };
}

template <typename Iter>
constexpr auto Plane::best_fit(const Iter& points) -> ENABLE_IF(ITER_OF(Iter, Vec3), Plane)
{
	// normal.x = E (z[i] + z[i+1]) (y[i] - y[i+1])
	// normal.y = E (x[i] + x[i+1]) (z[i] - z[i+1])
	// normal.z = E (y[i] + y[i+1]) (x[i] - x[i+1])

	usize count = 1;
	Vec3 normal = Vec3::Zero;
	Vec3 prev, sum; // NOLINT(*-isolate-declaration)

	for (const Vec3& current : points) {
		if (count == 1) {
			prev = sum = current;
			++count;
			continue;
		}

		normal.x += (prev.z + current.z) * (prev.y - current.y);
		normal.y += (prev.x + current.x) * (prev.z - current.z);
		normal.z += (prev.y + current.y) * (prev.x - current.x);

		prev = current;
		sum += current;
		++count;
	}

	normal.normalize();

	// Compute the distance as the average of the distance for each point
	//   = avg(p | n)
	//   = avg(p) | n
	flt distance = ((1 / static_cast<flt>(count)) * sum) | normal;

	return Plane{ normal, distance };
}


// Queries ---------------------------------------------------------------------

constexpr auto Plane::dist(const Vec3& point) const -> flt
{
	return (point | normal) - distance;
}

} // namespace math::geo
