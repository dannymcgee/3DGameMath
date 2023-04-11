#pragma once

#include <sized.h>

#include <math/vector.h>

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

namespace geo {

template <typename T = f64>
struct Circle {
	using Vec3 = Vector<3,T>;

	Vec3 center;
	Vec3 normal;
	T radius;
};

} // namespace geo
} // namespace math
