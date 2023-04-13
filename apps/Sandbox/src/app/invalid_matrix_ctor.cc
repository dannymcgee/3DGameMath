#include <fmt/format.h>

#include <math/matrix.h>
#include <sized.h>

using namespace sized; // NOLINT(*-using-namespace)
using math::Matrix;

void invalid_matrix_ctor()
{
	auto whoops = Matrix<2,3>{
		{ 11, 12, 13 },
		{ 21, 22, 23 },
		{ 31, 32, 33 },
	};
	fmt::print("Truncated Mat2x3:\n{}", whoops.to_string());
}
