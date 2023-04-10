#pragma once

#include "math/_vector_decl.h"
#include "math/_vector_impl.hpp"


namespace math {
template <usize D, typename T> struct Vector;

using Vec2 = Vector<2>;
using Vec3 = Vector<3>;
using Vec4 = Vector<4>;

}
