#pragma once

#include <sized.h>

#include "math/vector.h"
#include "math/matrix.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

enum class Axis : u8 { X, Y, Z };

template <typename T = f64>
class RotationMatrix : public Matrix<3,3,T> {
private:
	using Super = Matrix<3,3,T>;
	using Row = Vector<3,T>;

public:
	RotationMatrix() = default;

	RotationMatrix(T angle, Axis axis)
		: Super(construct(angle, axis))
	{}

	RotationMatrix(T angle, const Vector<3,T>& axis)
		: Super(construct(angle, axis))
	{}

private:
	static constexpr auto construct(T angle, Axis axis) -> std::array<Row,3>;

	static auto construct(T angle, const Vector<3,T>& axis) -> std::array<Row,3>;
};


template <typename T>
constexpr auto RotationMatrix<T>::construct(T angle, Axis axis) -> std::array<Row,3>
{
	switch (axis) {
		case Axis::X: {
			return std::array{
				Row{ 1.0, 0.0, 0.0 },
				Row{ 0.0, std::cos(angle), std::sin(angle) },
				Row{ 0.0, -std::sin(angle), std::cos(angle) },
			};
		}
		case Axis::Y: {
			return std::array{
				Row{ std::cos(angle), 0.0, -std::sin(angle) },
				Row{ 0.0, 1.0, 0.0 },
				Row{ std::sin(angle), 0.0, std::cos(angle) },
			};
		}
		case Axis::Z: {
			return std::array{
				Row{ std::cos(angle), std::sin(angle), 0.0 },
				Row{ -std::sin(angle), std::cos(angle), 0.0 },
				Row{ 0.0, 0.0, 1.0 },
			};
		}
	}
}

template <typename T>
inline auto RotationMatrix<T>::construct(T angle, const Vector<3,T>& axis) -> std::array<Row,3>
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

	return std::array{
		Row{
			x2 * one_sub_cos_theta + cos_theta,
			xy * one_sub_cos_theta + z_sin_theta,
			xz * one_sub_cos_theta - y_sin_theta,
		},
		Row{
			xy * one_sub_cos_theta - z_sin_theta,
			y2 * one_sub_cos_theta + cos_theta,
			yz * one_sub_cos_theta + x_sin_theta,
		},
		Row{
			xz * one_sub_cos_theta + y_sin_theta,
			yz * one_sub_cos_theta - x_sin_theta,
			z2 * one_sub_cos_theta + cos_theta,
		},
	};
}

}
