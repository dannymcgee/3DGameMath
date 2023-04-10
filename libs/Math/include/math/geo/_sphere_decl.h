#pragma once

#include <sized.h>

#include "math/vector.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

namespace geo {

template <typename T = f64>
struct Sphere {
	Vector<3,T> center;
	T radius;
};

} // namespace geo
} // namespace math
