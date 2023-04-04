#pragma once

#include <sized.h>

#include "math/vector.h"
#include "math/matrix.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)


class ScaleMatrix : public Mat3x3 {
private:
	using Super = Mat3x3;
	using Row = Vec3;

public:
	ScaleMatrix() = default;

	explicit ScaleMatrix(f64 scale)
		: Super(Super::identity() * scale)
	{}

	// TODO: Not sure if this is actually what I want
	explicit ScaleMatrix(const Vec3& scale)
		: Super{
			{ scale.x, 0, 0 },
			{ 0, scale.y, 0 },
			{ 0, 0, scale.z },
		}
	{}

	// explicit ScaleMatrix(const Vector<3,T>& scale)
	// 	: Super(construct(scale))
	// {}

private:
	// inline static
	// auto construct(const Vector<3,T>& scale) -> Super;
};


// FIXME: This does not work as expected
// template <typename T>
// auto ScaleMatrix<T>::construct(const Vector<3,T>& scale_vector) -> Super
// {
// 	if (scale_vector.x == scale_vector.y == scale_vector.z)
// 		return Super::identity() * scale_vector.x;

// 	auto [scale,direction] = scale_vector.length_and_direction();
// 	auto [x,y,z] = direction;

// 	auto x2 = x * x;
// 	auto y2 = y * y;
// 	auto z2 = z * z;

// 	auto xy = x * y;
// 	auto xz = x * z;
// 	auto yz = y * z;

// 	auto k_sub_1 = scale - static_cast<T>(1);

// 	return Super(std::array{
// 		Row{ 1 + k_sub_1 * x2,     k_sub_1 * xy,     k_sub_1 * xz },
// 		Row{     k_sub_1 * xy, 1 + k_sub_1 * y2,     k_sub_1 * yz },
// 		Row{     k_sub_1 * xz,     k_sub_1 * yz, 1 + k_sub_1 * z2 },
// 	});
// }

}
