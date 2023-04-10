#pragma once

#include "math/_matrix_decl.h"
#include "math/_matrix_impl.hpp"

namespace math {
template <usize R, usize C, typename T> class Matrix;

using Mat2x2 = Matrix<2,2>;
using Mat3x3 = Matrix<3,3>;
using Mat4x4 = Matrix<4,4>;

using Mat4x3 = Matrix<4,3>;
using Mat3x4 = Matrix<3,4>;

}
