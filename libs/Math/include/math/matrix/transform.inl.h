#pragma once

#include <vector>

#include <sized.h>

#include "math/matrix.h"
#include "math/vector.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

class RotationMatrix;
class TranslationMatrix;
class Quat;
class Euler;


class TransformMatrix : public Matrix<4,4> {
private:
	using Mat3x3 = Matrix<3,3>;
	using Mat4x4 = Matrix<4,4>;
	using Vec3 = Vector<3>;
	using Vec4 = Vector<4>;

public:
	using Super = Mat4x4;

	// Constructors -------------------------------------------------------------

	/** Create an identity matrix */
	constexpr TransformMatrix();

	/** Create a transform matrix from rotation and translation */
	constexpr TransformMatrix(
		const RotationMatrix& rotation,
		const TranslationMatrix& translation);

	/** Create a transform matrix from rotation */
	explicit constexpr TransformMatrix(const RotationMatrix& rotation);

	/** Create a transform matrix from rotation and translation */
	explicit constexpr TransformMatrix(const Quat& rotation, const Vec3& origin = Vec3::Zero);

private:
	static constexpr auto construct(
		const RotationMatrix& rotation,
		const TranslationMatrix& translation)
		-> Super;

	static constexpr auto construct(const RotationMatrix& rotation) -> Super;
	static constexpr auto construct(const Quat& rotation, const Vec3& origin) -> Super;

public:
	// Transformation -----------------------------------------------------------

	/** Transform a vector representing a point. */
	constexpr auto transform_point(const Vec3& point) const -> Vec3;

	/** Transform a vector representing a direction. */
	constexpr auto transform_vector(const Vec3& vector) const -> Vec3;
};

} // namespace math
