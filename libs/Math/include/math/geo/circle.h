#pragma once

#include <sized.h>

#include "math/vector.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

namespace geo {

struct Circle {
	Vec3 center;
	Vec3 normal;
	flt radius = 0;
};

} // namespace geo
} // namespace math
