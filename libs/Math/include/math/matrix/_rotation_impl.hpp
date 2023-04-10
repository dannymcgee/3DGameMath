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

template <typename T>
RotationMatrix<T>::RotationMatrix(T angle, Axis axis)
	: Super(RotationMatrix::construct(angle, axis))
{}

template <typename T>
RotationMatrix<T>::RotationMatrix(T angle, const Vector<3,T>& axis)
	: Super(RotationMatrix::construct(angle, axis))
{}

template <typename T>
constexpr RotationMatrix<T>::RotationMatrix(const Super& super)
	: Super(super)
{}

template <typename T>
constexpr RotationMatrix<T>::RotationMatrix(Super&& super)
	: Super(super)
{}

template <typename T>
constexpr auto RotationMatrix<T>::construct(T angle, Axis axis) -> Super
{
	T sin_angle = std::sin(angle);
	T cos_angle = std::cos(angle);

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

template <typename T>
constexpr auto RotationMatrix<T>::construct(T angle, const Vector<3,T>& axis) -> Super
{
	T cos_theta = std::cos(angle);
	T sin_theta = std::sin(angle);
	T one_sub_cos_theta = 1.0 - cos_theta;

	auto [x,y,z] = axis;

	T x2 = x * x;
	T y2 = y * y;
	T z2 = z * z;

	T xy = x * y;
	T xz = x * z;
	T yz = y * z;

	T x_sin_theta = x * sin_theta;
	T y_sin_theta = y * sin_theta;
	T z_sin_theta = z * sin_theta;

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

template <typename T>
inline auto RotationMatrix<T>::inverse() const -> RotationMatrix
{
	return Super::transpose();
}

template <typename T>
inline auto RotationMatrix<T>::transpose() const -> RotationMatrix
{
	return Super::transpose();
}

template <typename T>
constexpr auto RotationMatrix<T>::operator*(const RotationMatrix& rhs) const -> RotationMatrix
{
	const auto& lhs = static_cast<Super>(*this);
	return lhs * static_cast<Super>(rhs);
}


// Conversion ------------------------------------------------------------------

template <typename T>
inline auto RotationMatrix<T>::euler() const -> Euler<T>
{
	using namespace math::literals; // NOLINT(*-using-namespace)

	// Extract pitch from m32, being careful for domain errors with std::asin().
	// We could have values slightly out of range due to floating point errors.
	T pitch;
	T sin_pitch = -m32;
	if (sin_pitch <= static_cast<T>(-1))
		pitch = -90_deg;
	else if (sin_pitch >= static_cast<T>(1))
		pitch = 90_deg;
	else
		pitch = std::asin(sin_pitch);

	// Check for gimbal lock
	if (math::nearly_equal<T>(std::abs(sin_pitch), 1)) {
		// Pitch is ±90°, so assign all rotation around the vertical axis to yaw.
		T roll = 0;
		T yaw = std::atan2(-m13, m11);

		return Euler<T>{ yaw, pitch, roll };
	}

	T yaw = std::atan2(m31, m33);
	T roll = std::atan2(m12, m22);

	return Euler<T>{ yaw, pitch, roll };
}

template <typename T>
inline auto RotationMatrix<T>::quat() const -> Quat<T>
{
	// Each value here represents:
	//    4n²-1
	// where n = the x, y, z or w component of the resulting quaternion.
	T w_raw = m11 + m22 + m33;
	T x_raw = m11 - m22 - m33;
	T y_raw = m22 - m11 - m33;
	T z_raw = m33 - m11 - m22;

	// Find the largest component, and use it as the basis to derive the others
	T largest_raw = std::max({ w_raw, x_raw, y_raw, z_raw });
	T largest_val = std::sqrt(largest_raw + 1) * 0.5;
	T scale = 0.25 / largest_val;

	if (largest_raw == w_raw)
		return Quat<T>{
			largest_val,
			(m23 - m32) * scale,
			(m31 - m13) * scale,
			(m12 - m21) * scale,
		};

	if (largest_raw == x_raw)
		return Quat<T>{
			(m23 - m32) * scale,
			largest_val,
			(m12 + m21) * scale,
			(m31 + m13) * scale,
		};

	if (largest_raw == y_raw)
		return Quat<T>{
			(m31 - m13) * scale,
			(m12 + m21) * scale,
			largest_val,
			(m23 + m32) * scale,
		};

	if (largest_raw == z_raw)
		return Quat<T>{
			(m12 - m21) * scale,
			(m31 + m13) * scale,
			(m23 + m32) * scale,
			largest_val,
		};

	ASSERT(false, "Value {} not in set: [ {}, {}, {}, {} ]",
		largest_raw, w_raw, x_raw, y_raw, z_raw);

	return Quat<T>{};
}

} // namespace math
