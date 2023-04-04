#pragma once

#include <sized.h>

#include "math/vector.h"
#include "math/matrix.h"


namespace math {
using namespace sized; // NOLINT(*-using-namespace)

class TranslationMatrix : public Mat4x4 {
private:
	using Super = Mat4x4;
	using Row = Vec4;

public:
	TranslationMatrix() = default;

	TranslationMatrix(f64 x, f64 y, f64 z)
		: Super{
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ x, y, z, 1 },
		}
	{}

	explicit TranslationMatrix(const Vec3& delta)
		: Super{
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ delta.x, delta.y, delta.z, 0 },
		}
	{}
};

} // namespace math


inline auto operator*(
	const ::math::Mat3x3& lhs,
	const ::math::TranslationMatrix& rhs
) -> ::math::Mat4x4
{
	auto [m11,m12,m13] = lhs.row<1>();
	auto [m21,m22,m23] = lhs.row<2>();
	auto [m31,m32,m33] = lhs.row<3>();

	::math::Mat4x4 linear {
		{ m11, m12, m13,   0 },
		{ m21, m22, m23,   0 },
		{ m31, m32, m33,   0 },
		{   0,   0,   0,   1 },
	};

	return linear * rhs;
}
