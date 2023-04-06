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
	explicit ScaleMatrix(f64 scale);
	explicit ScaleMatrix(const Vec3& scale);

private:
	// static auto construct(const Vector<3,T>& scale) -> Super;
};

}
