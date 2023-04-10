#include <fmt/format.h>

#include <math/matrix.h>

using math::Mat4x3;

void matrix()
{
	auto mat = Mat4x3{
		{ 11, 12, 13 },
		{ 21, 22, 23 },
		{ 31, 32, 33 },
		{ 41, 42, 43 },
	};
	fmt::print("{}\n", mat.to_string());

	auto m21 = mat.m21;
	fmt::print("m21: {}\n", m21);

	mat.m21 = 5;
	fmt::print("{}\n", mat.to_string());

	auto transposed = mat.transpose();
	fmt::print("{}\n", transposed.to_string());
}
