#pragma once

#include <sized.h>

#include "math/vector.h"
#include "math/matrix.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

// Forward declarations
struct Euler;
struct Quat;


enum class Axis : u8 {
	X, Y, Z,
	Right = X,
	Up = Y,
	Forward = Z,
};

class RotationMatrix : public Matrix<3,3> {
private:
	using Super = Matrix<3,3>;
	using Base = detail::Matrix<3,3>;
	using Row = Vector<3>;

public:
	using Base::m11;
	using Base::m12;
	using Base::m13;
	using Base::m21;
	using Base::m22;
	using Base::m23;
	using Base::m31;
	using Base::m32;
	using Base::m33;

	// Constructors
	/** Construct a rotation matrix from an angle and a cardinal axis */
	constexpr RotationMatrix(flt angle, Axis axis);
	/** Construct a rotation matrix from an angle and an arbitrary axis */
	constexpr RotationMatrix(flt angle, const Vector<3>& axis);

	// Operations
	/** Get the inverse of the matrix (equivalent to transpose) */
	auto inverse() const -> RotationMatrix;
	/** Get the transpose of the matrix */
	auto transpose() const -> RotationMatrix;

	constexpr auto operator*(const RotationMatrix& rhs) const -> RotationMatrix;

	// Conversion
	/** Convert to Euler axes */
	auto euler() const -> Euler;
	/** Convert to a quaternion */
	auto quat() const -> Quat;

private:
	// Not every Mat3x3 is a valid RotationMatrix, so we'll keep these private
	// and declare friends when necessary.
	constexpr RotationMatrix(const Super& super); // NOLINT(*-explicit-constructor)
	constexpr RotationMatrix(Super&& super); // NOLINT(*-explicit-constructor)

	// Constructor helpers
	static constexpr auto construct(flt angle, Axis axis) -> Super;
	static constexpr auto construct(flt angle, const Vector<3>& axis) -> Super;

	friend struct math::Euler;
	friend struct math::Quat;
};

} // namespace math
