#pragma once

#include "math/geo/_circle_decl.h"
#include "math/geo/_circle_impl.hpp"

namespace math::geo {
template <typename T> struct Circle;
}

namespace math {
using Circle = geo::Circle<f64>;
}
