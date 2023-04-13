#pragma once

#include <sized.h>

#include "math/vector.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

namespace geo {

struct Sphere {
	Vec3 center;
	flt radius = 0;
};

} // namespace geo
} // namespace math
