#pragma once

#include "math/matrix/_rotation_decl.h"

#include <cmath>
#include <numeric>

#include "math/assert.h"
#include "math/euler.h"
#include "math/literals.h"
#include "math/quat.h"

namespace math {


// Constructors ----------------------------------------------------------------

constexpr RotationMatrix::RotationMatrix(flt angle, Axis axis)
	: Super(RotationMatrix::construct(angle, axis))
{}

constexpr RotationMatrix::RotationMatrix(flt angle, const Vector<3>& axis)
	: Super(RotationMatrix::construct(angle, axis))
{}

constexpr RotationMatrix::RotationMatrix(const Super& super)
	: Super(super)
{}

constexpr RotationMatrix::RotationMatrix(Super&& super)
	: Super(super)
{}

constexpr auto RotationMatrix::construct(flt angle, Axis axis) -> Super
{
	flt sin_angle = std::sin(angle);
	flt cos_angle = std::cos(angle);

	switch (axis) {
		case Axis::X: {
			return {
				{ 1, 0, 0 },
				{ 0, cos_angle, sin_angle },
				{ 0, -sin_angle, cos_angle },
			};
		}
		case Axis::Y: {
			return {
				{ cos_angle, 0, -sin_angle },
				{ 0, 1, 0 },
				{ sin_angle, 0, cos_angle },
			};
		}
		case Axis::Z: {
			return {
				{ cos_angle, sin_angle, 0 },
				{ -sin_angle, cos_angle, 0 },
				{ 0, 0, 1 },
			};
		}
	}
	return Super{};
}

constexpr auto RotationMatrix::construct(flt angle, const Vector<3>& axis) -> Super
{
	flt cos_theta = std::cos(angle);
	flt sin_theta = std::sin(angle);
	flt one_sub_cos_theta = 1.0 - cos_theta;

	auto [x,y,z] = axis;

	flt x2 = x * x;
	flt y2 = y * y;
	flt z2 = z * z;

	flt xy = x * y;
	flt xz = x * z;
	flt yz = y * z;

	flt x_sin_theta = x * sin_theta;
	flt y_sin_theta = y * sin_theta;
	flt z_sin_theta = z * sin_theta;

	return {
		{
			x2 * one_sub_cos_theta + cos_theta,
			xy * one_sub_cos_theta + z_sin_theta,
			xz * one_sub_cos_theta - y_sin_theta,
		}, {
			xy * one_sub_cos_theta - z_sin_theta,
			y2 * one_sub_cos_theta + cos_theta,
			yz * one_sub_cos_theta + x_sin_theta,
		}, {
			xz * one_sub_cos_theta + y_sin_theta,
			yz * one_sub_cos_theta - x_sin_theta,
			z2 * one_sub_cos_theta + cos_theta,
		},
	};
}


// Operations ------------------------------------------------------------------

inline auto RotationMatrix::inverse() const -> RotationMatrix
{
	return Super::transpose();
}

inline auto RotationMatrix::transpose() const -> RotationMatrix
{
	return Super::transpose();
}

constexpr auto RotationMatrix::operator*(const RotationMatrix& rhs) const -> RotationMatrix
{
	const auto& lhs = static_cast<Super>(*this);
	return lhs * static_cast<Super>(rhs);
}


// Conversion ------------------------------------------------------------------

inline auto RotationMatrix::euler() const -> Euler
{
	using namespace math::literals; // NOLINT(*-using-namespace)

	// Extract pitch from m32, being careful for domain errors with std::asin().
	// We could have values slightly out of range due to floating point errors.
	flt pitch;
	flt sin_pitch = -m32;
	if (sin_pitch <= -1)
		pitch = -90_deg;
	else if (sin_pitch >= 1)
		pitch = 90_deg;
	else
		pitch = std::asin(sin_pitch);

	// Check for gimbal lock
	if (math::nearly_equal(std::abs(sin_pitch), 1)) {
		// Pitch is ±90°, so assign all rotation around the vertical axis to yaw.
		flt roll = 0;
		flt yaw = std::atan2(-m13, m11);

		return Euler{ yaw, pitch, roll };
	}

	flt yaw = std::atan2(m31, m33);
	flt roll = std::atan2(m12, m22);

	return Euler{ yaw, pitch, roll };
}

inline auto RotationMatrix::quat() const -> Quat
{
	// Each value here represents:
	//    4n²-1
	// where n = the x, y, z or w component of the resulting quaternion.
	flt w_raw = m11 + m22 + m33;
	flt x_raw = m11 - m22 - m33;
	flt y_raw = m22 - m11 - m33;
	flt z_raw = m33 - m11 - m22;

	// Find the largest component, and use it as the basis to derive the others
	flt largest_raw = std::max({ w_raw, x_raw, y_raw, z_raw });
	flt largest_val = std::sqrt(largest_raw + 1) * 0.5;
	flt scale = 0.25 / largest_val;

	if (largest_raw == w_raw)
		return Quat{
			largest_val,
			(m23 - m32) * scale,
			(m31 - m13) * scale,
			(m12 - m21) * scale,
		};

	if (largest_raw == x_raw)
		return Quat{
			(m23 - m32) * scale,
			largest_val,
			(m12 + m21) * scale,
			(m31 + m13) * scale,
		};

	if (largest_raw == y_raw)
		return Quat{
			(m31 - m13) * scale,
			(m12 + m21) * scale,
			largest_val,
			(m23 + m32) * scale,
		};

	if (largest_raw == z_raw)
		return Quat{
			(m12 - m21) * scale,
			(m31 + m13) * scale,
			(m23 + m32) * scale,
			largest_val,
		};

	ASSERT(false, "Value {} not in set: [ {}, {}, {}, {} ]",
		largest_raw, w_raw, x_raw, y_raw, z_raw);

	return Quat{};
}

} // namespace math
