#pragma once

#include <sized.h>

#include <math/vector.h>

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

namespace geo {

struct Circle {
	Vec3 center;
	Vec3 normal;
	flt radius;
};

} // namespace geo
} // namespace math
