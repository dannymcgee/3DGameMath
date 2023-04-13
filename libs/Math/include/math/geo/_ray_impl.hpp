#pragma once

#include "math/geo/_ray_decl.h"

namespace math::geo {

constexpr Ray::Ray(const Vec3& origin, const Vec3& delta)
	: origin(origin)
	, delta(delta)
{}

constexpr Ray::Ray(const Vec3& origin, const Vec3& direction, flt length)
	: origin(origin)
	, delta(direction * length)
{}

} // namespace math::geo
