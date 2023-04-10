#include <fmt/format.h>

#include <math/literals.h>
#include <math/matrix.h>
#include <math/matrix/rotation.h>
#include <math/matrix/translation.h>
#include <math/vector.h>

using namespace math::literals; // NOLINT(*-using-namespace)
using math::Mat4x4;
using math::RotationMatrix;
using math::TranslationMatrix;
using math::Vec3;

void matrix_transformation()
{
	auto rotation = RotationMatrix(45.0_deg, Vec3{ -0.25, 0.5, 0.33 }.normal());
	auto translation = TranslationMatrix(5.0, 1.0, -12.125);
	auto transform = rotation * translation;

	fmt::print("Rotation:\n{}\n", rotation.to_string());
	fmt::print("Rotation euler: {}\n\n", rotation.euler().to_string());
	fmt::print("Translation:\n{}\n", translation.to_string());
	fmt::print("Combined transform:\n{}\n", transform.to_string());

	auto local = Mat4x4{
		{ 1, 0, 0, 1 }, // forward vector
		{ 0, 1, 0, 1 }, // right vector
		{ 0, 0, 1, 1 }, // up vector
		{ 0, 0, 0, 1 }, // origin point
	};
	auto world = local * transform;
	fmt::print("Transformed matrix:\n{}\n\n", world.to_string());

	if (auto inverse = transform.inverse()) {
		auto world2local = world * (*inverse);
		fmt::print("world -> local:\n{}\n\n", world2local.to_string());
	}
}
