#include "math/matrix/scale.h"


namespace math {

ScaleMatrix::ScaleMatrix(flt scale)
	: Super(Super::identity() * scale)
{}

ScaleMatrix::ScaleMatrix(const Vec3& scale)
	: Super{
		{ scale.x, 0, 0 },
		{ 0, scale.y, 0 },
		{ 0, 0, scale.z },
	}
{}

} // namespace math
