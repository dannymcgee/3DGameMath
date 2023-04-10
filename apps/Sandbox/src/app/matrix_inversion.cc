#include <fmt/format.h>

#include <math/matrix.h>

using math::Matrix;

void matrix_inversion()
{
	auto mat = Matrix<4,4>{
		{ -4.0, -3.0,  3.0,  1.0 },
		{  0.0,  2.0, -2.0,  0.0 },
		{  1.0,  4.0, -1.0,  1.0 },
		{  0.0,  2.0, -2.0,  1.0 },
	};
	fmt::print("Mat4x4:\n{}\n", mat.to_string());

	if (auto inverted = mat.inverse())
		fmt::print("Inverse Mat4x4:\n{}\n", inverted->to_string());
}
