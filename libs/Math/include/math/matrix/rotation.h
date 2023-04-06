#pragma once

#include <sized.h>

#include "math/vector.h"
#include "math/matrix.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

enum class Axis : u8 { X, Y, Z };

class RotationMatrix : public Mat3x3 {
private:
	using Super = Mat3x3;
	using Row = Vec3;

public:
	RotationMatrix(f64 angle, Axis axis);
	RotationMatrix(f64 angle, const Vec3& axis);

private:
	static constexpr auto construct(f64 angle, Axis axis) -> Super;

	static auto construct(f64 angle, const Vec3& axis) -> Super;
};

}
