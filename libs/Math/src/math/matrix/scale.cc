#include "math/matrix/scale.h"


namespace math {

ScaleMatrix::ScaleMatrix(f64 scale)
	: Super(Super::identity() * scale)
{}

// TODO: Not sure if this is actually what I want
ScaleMatrix::ScaleMatrix(const Vec3& scale)
	: Super{
		{ scale.x, 0, 0 },
		{ 0, scale.y, 0 },
		{ 0, 0, scale.z },
	}
{}

// ScaleMatrix::ScaleMatrix(const Vec3& scale)
// 	: Super(construct(scale))
// {}

// FIXME: This does not work as expected
// auto ScaleMatrix::construct(const Vec3& scale_vector) -> Super
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

// 	return {
// 		{ 1 + k_sub_1 * x2,     k_sub_1 * xy,     k_sub_1 * xz },
// 		{     k_sub_1 * xy, 1 + k_sub_1 * y2,     k_sub_1 * yz },
// 		{     k_sub_1 * xz,     k_sub_1 * yz, 1 + k_sub_1 * z2 },
// 	};
// }

} // namespace math
