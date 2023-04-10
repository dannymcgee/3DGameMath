#include <fmt/format.h>

#include <math/matrix.h>
#include <math/matrix/scale.h>

using math::Mat3x3;
using math::ScaleMatrix;

void scale_matrix()
{
	auto mat = ScaleMatrix(-12.125);
	fmt::print("ScaleMatrix(-12.125)\n{}\n", mat.to_string());

	auto non_uniform = ScaleMatrix({ -1.0, 1.5, 1.0 });
	fmt::print("non-uniform:\n{}\n", non_uniform.to_string(5));

	auto scaled = Mat3x3::identity() * non_uniform;
	fmt::print("scaled identity:\n{}\n", scaled.to_string(5));
}
