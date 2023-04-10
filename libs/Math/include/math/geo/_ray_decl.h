#pragma once

#include <sized.h>

#include "math/vector.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

namespace geo {

template <typename T = f64>
struct Ray {
	using Vec3 = Vector<3,T>;

	Vec3 origin;
	Vec3 delta;

	constexpr Ray() = default;
	constexpr Ray(const Vec3& origin, const Vec3& delta);
	constexpr Ray(const Vec3& origin, const Vec3& direction, T length);
};

} // namespace geo
} // namespace math
