#include <fmt/format.h>

#include <math/matrix.h>
#include <sized.h>

using namespace sized; // NOLINT(*-using-namespace)
using math::Matrix;

void matrix_determinants()
{
	auto mat2x2 = Matrix<2,2>{
		{ -3.0, 4.0 },
		{  2.0, 5.0 },
	};
	fmt::print("Mat2x2:\n{}", mat2x2.to_string());
	fmt::print("determinant: {}\n\n", mat2x2.determinant());

	auto mat3x3 = Matrix<3,3>{
		{ -4.0, -3.0,  3.0 },
		{  0.0,  2.0, -2.0 },
		{  1.0,  4.0, -1.0 },
	};
	fmt::print("Mat3x3:\n{}", mat3x3.to_string());
	fmt::print("determinant: {}\n\n", mat3x3.determinant());

	auto mat4x4 = Matrix<4,4>{
		{ -4.0, -3.0,  3.0,  1.0 },
		{  0.0,  2.0, -2.0,  0.0 },
		{  1.0,  4.0, -1.0,  1.0 },
		{  0.0,  2.0, -2.0,  1.0 },
	};
	fmt::print("Mat4x4:\n{}", mat4x4.to_string());
	fmt::print("determinant: {}\n\n", mat4x4.determinant());
}
