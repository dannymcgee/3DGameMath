#include "math/matrix/rotation.h"


namespace math {

RotationMatrix::RotationMatrix(f64 angle, Axis axis)
	: Super(construct(angle, axis))
{}

RotationMatrix::RotationMatrix(f64 angle, const Vec3& axis)
	: Super(construct(angle, axis))
{}

// ReSharper disable once CppNotAllPathsReturnValue
constexpr auto RotationMatrix::construct(f64 angle, Axis axis) -> Super
{
	switch (axis) {
		case Axis::X: {
			return {
				{ 1.0, 0.0, 0.0 },
				{ 0.0, std::cos(angle), std::sin(angle) },
				{ 0.0, -std::sin(angle), std::cos(angle) },
			};
		}
		case Axis::Y: {
			return {
				{ std::cos(angle), 0.0, -std::sin(angle) },
				{ 0.0, 1.0, 0.0 },
				{ std::sin(angle), 0.0, std::cos(angle) },
			};
		}
		case Axis::Z: {
			return {
				{ std::cos(angle), std::sin(angle), 0.0 },
				{ -std::sin(angle), std::cos(angle), 0.0 },
				{ 0.0, 0.0, 1.0 },
			};
		}
	}
}

inline auto RotationMatrix::construct(f64 angle, const Vec3& axis) -> Super
{
	auto cos_theta = std::cos(angle);
	auto sin_theta = std::sin(angle);
	auto one_sub_cos_theta = 1.0 - cos_theta;

	auto [x,y,z] = axis;

	auto x2 = x * x;
	auto y2 = y * y;
	auto z2 = z * z;

	auto xy = x * y;
	auto xz = x * z;
	auto yz = y * z;

	auto x_sin_theta = x * sin_theta;
	auto y_sin_theta = y * sin_theta;
	auto z_sin_theta = z * sin_theta;

	return {
		{
			x2 * one_sub_cos_theta + cos_theta,
			xy * one_sub_cos_theta + z_sin_theta,
			xz * one_sub_cos_theta - y_sin_theta,
		}, {
			xy * one_sub_cos_theta - z_sin_theta,
			y2 * one_sub_cos_theta + cos_theta,
			yz * one_sub_cos_theta + x_sin_theta,
		}, {
			xz * one_sub_cos_theta + y_sin_theta,
			yz * one_sub_cos_theta - x_sin_theta,
			z2 * one_sub_cos_theta + cos_theta,
		},
	};
}

} // namespace math
