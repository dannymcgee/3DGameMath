#pragma once

#include <sized.h>

#include "math/vector.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

namespace geo {

struct Sphere {
	Vec3 center;
	flt radius;
};

} // namespace geo
} // namespace math
