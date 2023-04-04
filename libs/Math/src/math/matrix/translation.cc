#include "math/matrix/translation.h"


namespace math {

TranslationMatrix::TranslationMatrix(f64 x, f64 y, f64 z)
	: Super{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ x, y, z, 1 },
	}
{}

TranslationMatrix::TranslationMatrix(const Vec3& delta)
	: Super{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ delta.x, delta.y, delta.z, 0 },
	}
{}

} // namespace math
