#pragma once

#include "math/geo/_ray_decl.h"

namespace math::geo {

template <typename T>
constexpr Ray<T>::Ray(const Vec3& origin, const Vec3& delta)
	: origin(origin)
	, delta(delta)
{}

template <typename T>
constexpr Ray<T>::Ray(const Vec3& origin, const Vec3& direction, T length)
	: origin(origin)
	, delta(direction * length)
{}

} // namespace math::geo
