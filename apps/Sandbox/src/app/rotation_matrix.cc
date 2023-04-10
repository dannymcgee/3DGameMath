#include <fmt/format.h>

#include <math/fmt.h>
#include <math/literals.h>
#include <math/matrix.h>
#include <math/matrix/rotation.h>

using namespace math::literals; // NOLINT(*-using-namespace)
using math::fmt::AlignedValues;
using math::Vec3;
using math::Mat3x3;
using math::RotationMatrix;

void rotation_matrix()
{
	auto angle = 45.0_deg;
	auto axis = Vec3{ -0.25, 0.5, 0.33 }.unit();

	auto rotation_mat = RotationMatrix(angle, axis);

	auto rotated = Mat3x3::identity() * rotation_mat;
	auto formatter = AlignedValues(rotated.begin(), rotated.end(), 5);

	fmt::print("============= Rotation Matrix =============\n");
	fmt::print("Right:   {}\n", rotated.row<1>().to_string(formatter));
	fmt::print("Up:      {}\n", rotated.row<2>().to_string(formatter));
	fmt::print("Forward: {}\n", rotated.row<3>().to_string(formatter));
	fmt::print("-------------------------------------------\n");
	fmt::print("Axis:    {}\n", axis.to_string(formatter));
	fmt::print("Radians: {}\n", angle);
	fmt::print("\n");

	fmt::print("Rotation matrix is orthogonal? {}\n", rotation_mat.is_orthogonal());

	auto transposed = rotation_mat.transpose();
	auto id = rotation_mat * transposed;
	fmt::print("RM*RM^T:\n{}\n", id.to_string());
}
