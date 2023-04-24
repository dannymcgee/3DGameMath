#include "math/matrix/translation.h"


namespace math {

TranslationMatrix::TranslationMatrix(flt x, flt y, flt z)
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
		{ delta.x, delta.y, delta.z, 1 },
	}
{}

} // namespace math
