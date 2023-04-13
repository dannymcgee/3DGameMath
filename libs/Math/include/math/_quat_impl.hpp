#pragma once

#include "math/_quat_decl.h"

#include <cmath>
#include <utility>

#include "math/assert.h"
#include "math/euler.h"
#include "math/matrix/rotation.h"
#include "math/utility.h"

namespace math {

// Constructors ----------------------------------------------------------------

inline Quat::Quat() // NOLINT(*-use-equals-default,*-member-init)
{}

constexpr Quat::Quat(flt w, flt x, flt y, flt z)
	: w(w), x(x), y(y), z(z)
{}

constexpr Quat::Quat(flt w, const Vector<3>& xyz) // NOLINT(*-member-init)
	: w(w)
	, vector(xyz)
{}

constexpr auto Quat::identity() -> Quat
{
	return { 1, 0, 0, 0 };
}

constexpr auto Quat::angle_axis(flt angle, const Vector<3>& unit_axis) -> Quat
{
	flt half_angle = 0.5 * angle;
	return {
		std::cos(half_angle),
		std::sin(half_angle) * unit_axis,
	};
}


// Conversion ------------------------------------------------------------------

inline auto Quat::angle_axis() const -> std::tuple<flt, Vector<3>>
{
	flt angle = 2 * std::acos(w);
	if (math::nearly_equal(angle, 0))
		return { 0, Vector<3>::Zero };

	flt scale = 1 / std::sqrt(1 - w * w);
	return { angle, vector * scale };
}

inline auto Quat::euler(Space space) const -> Euler
{
	if (space == Space::Parent2Local)
		return inverse().euler(Space::Local2Parent);

	using namespace math::literals; // NOLINT(*-using-namespace)

	flt sin_pitch = -2 * (y * z - w * x);

	// Check for gimbal lock
	if (math::nearly_equal(std::abs(sin_pitch), 1))
		return Euler{
			/* yaw */   std::atan2(-x * z + w * y, 0.5 - y * y - z * z),
			/* pitch */ 90_deg * sin_pitch,
			/* roll */  0,
		};

	return Euler{
		/* yaw */   std::atan2(x * z + w * y, 0.5 - x * x - y * y),
		/* pitch */ std::asin(sin_pitch),
		/* roll */  std::atan2(x * y + w * z, 0.5 - x * x - z * z),
	};
}

inline auto Quat::matrix(Space space) const -> RotationMatrix
{
	if (space == Space::Parent2Local)
		return inverse().matrix(Space::Local2Parent);

	flt two_x2 = 2 * x * x;
	flt two_y2 = 2 * y * y;
	flt two_z2 = 2 * z * z;

	flt two_xy = 2 * x * y;
	flt two_xz = 2 * x * z;
	flt two_yz = 2 * y * z;

	flt two_wx = 2 * w * x;
	flt two_wy = 2 * w * y;
	flt two_wz = 2 * w * z;

	return RotationMatrix{{
		{ 1-two_y2-two_z2,    two_xy+two_wz,    two_xz-two_wy  },
		{  two_xy-two_wz,    1-two_x2-two_z2,   two_yz+two_wx  },
		{  two_xz+two_wy,     two_yz-two_wx,   1-two_x2-two_y2 },
	}};
}


// Magnitude -------------------------------------------------------------------

inline auto Quat::magnitude() const -> flt
{
	flt sq_mag = sq_magnitude();

	if (math::nearly_equal(sq_mag, 1))
		return 1;

	if (math::nearly_equal(sq_mag, 0))
		return 0;

	return std::sqrt(sq_mag);
}

inline auto Quat::sq_magnitude() const -> flt
{
	return w*w + x*x + y*y + z*z;
}


// Normalize -------------------------------------------------------------------

inline void Quat::normalize()
{
	flt sq_mag = sq_magnitude();

	if (math::nearly_equal(sq_mag, 1))
		return;

	if (math::nearly_equal(sq_mag, 0)) {
		*this = Quat::identity();
		return;
	}

	flt scale = 1.0 / std::sqrt(sq_mag);
	w *= scale;
	vector *= scale;
}


// Conjugate & Inverse ---------------------------------------------------------

constexpr auto Quat::conjugate() const -> Quat
{
	return { w, -vector };
}

constexpr auto Quat::inverse() const -> Quat
{
	flt sq_mag = sq_magnitude();

	if (math::nearly_equal(sq_mag, 1))
		return conjugate();

	flt scale = 1 / std::sqrt(sq_mag);
	return conjugate() * scale;
}


// Unary negation --------------------------------------------------------------

inline auto Quat::operator-() const -> Quat
{
	return { -w, -x, -y, -z };
}


// Quaternion multiplication ---------------------------------------------------

constexpr auto Quat::operator*(const Quat& other) const -> Quat
{
	return {
		(w * other.w) - (vector | other.vector),
		(w * other.vector) + (other.w * vector) + (vector ^ other.vector),
	};
}


// Scalar multiplication -------------------------------------------------------

constexpr auto Quat::operator*(flt scale) const -> Quat
{
	return {
		w * scale,
		vector * scale,
	};
}

constexpr auto Quat::operator*=(flt scale) -> Quat&
{
	w *= scale;
	vector *= scale;

	return *this;
}

} // namespace math

constexpr auto operator*(sized::flt lhs, const math::Quat& rhs) -> math::Quat
{
	return rhs * lhs;
}

namespace math {


// Addition --------------------------------------------------------------------

constexpr auto Quat::operator+(const Quat& rhs) const -> Quat
{
	return {
		w + rhs.w,
		vector + rhs.vector,
	};
}


// Difference ------------------------------------------------------------------

constexpr auto Quat::diff(const Quat& rhs) const -> Quat
{
	return rhs * inverse();
}

constexpr auto Quat::operator-(const Quat& rhs) const -> Quat
{
	return diff(rhs);
}


// Dot product -----------------------------------------------------------------

constexpr auto Quat::dot(const Quat& rhs) const -> flt
{
	return (w * rhs.w) + (vector | rhs.vector);
}

constexpr auto Quat::operator|(const Quat& rhs) const -> flt
{
	return dot(rhs);
}


// Exponentiation --------------------------------------------------------------

constexpr auto Quat::pow(flt exp) const -> Quat
{
	if (math::nearly_equal(w, 1))
		return Quat::identity();

	flt alpha = std::acos(w);
	flt out_alpha = alpha * exp;

	return {
		std::cos(out_alpha),
		vector * std::sin(out_alpha) / std::sin(alpha),
	};
}


// Rotation --------------------------------------------------------------------

constexpr auto Quat::rotate_point(const Vector<3>& point) const -> Vector<3>
{
	Quat result = *this * Quat{ 0, point } * inverse();
	return result.vector;
}


// Spherical interpolation -----------------------------------------------------

inline auto Quat::slerp(const Quat& src, const Quat& dest, flt t) -> Quat
{
	return (dest - src).pow(t) * src;
}

inline auto Quat::slerp(const Quat& dest, flt t) const -> Quat
{
	return Quat::slerp(*this, dest, t);
}


// Misc / Utility --------------------------------------------------------------

inline auto Quat::to_string(usize precision) const -> std::string
{
	auto formatter = fmt::AlignedValues(begin(), end(), precision);
	return to_string(formatter);
}

inline auto Quat::to_string(const fmt::AlignedValues& formatter) const -> std::string
{
	return ::fmt::format("[ {}  ( {}  {}  {} )]",
		formatter.format(w),
		formatter.format(x),
		formatter.format(y),
		formatter.format(z));
}


// Iterator support ------------------------------------------------------------

inline auto Quat::begin() -> detail::RawIterator<flt>
{
	return detail::RawIterator(&w);
}

inline auto Quat::begin() const -> detail::RawConstIterator<flt>
{
	return detail::RawConstIterator(&w);
}

inline auto Quat::end() -> detail::RawIterator<flt>
{
	return detail::RawIterator(&z + 1); // NOLINT(*-pointer-arithmetic)
}

inline auto Quat::end() const -> detail::RawConstIterator<flt>
{
	return detail::RawConstIterator(&z + 1); // NOLINT(*-pointer-arithmetic)
}


// Subscript operator ----------------------------------------------------------

inline auto Quat::operator[](usize idx) -> flt&
{
	switch (idx) {
		case 0: return w;
		default: return vector[idx + 1];
	}
}

inline auto Quat::operator[](usize idx) const -> flt
{
	ASSERT(idx < 4, "Index out of range");

	switch (idx) {
		case 0: return w;
		default: return vector[idx + 1];
	}
}


// Structured binding support --------------------------------------------------

template <usize Index, typename U>
inline auto Quat::get_helper(U&& self) -> auto&&
{
	if constexpr (Index == 0) return std::forward<U>(self).w;
	if constexpr (Index == 1) return std::forward<U>(self).x;
	if constexpr (Index == 2) return std::forward<U>(self).y;
	if constexpr (Index == 3) return std::forward<U>(self).z;
}

} // namespace math

namespace std {

template <>
struct tuple_size<::math::Quat> {
	static constexpr size_t value = 4;
};

template <size_t Index>
struct tuple_element<Index, ::math::Quat> {
	static_assert(Index < 4, "Index out of range");
	using type = sized::flt;
};

}

