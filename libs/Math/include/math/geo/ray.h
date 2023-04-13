#pragma once

#include <sized.h>

#include "math/vector.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

namespace geo {

struct Ray {
	Vec3 origin;
	Vec3 delta;

	constexpr Ray() = default;

	constexpr Ray(const Vec3& origin, const Vec3& delta)
		: origin(origin)
		, delta(delta)
	{}

	constexpr Ray(const Vec3& origin, const Vec3& direction, flt length)
		: origin(origin)
		, delta(direction * length)
	{}
};

} // namespace geo
} // namespace math
