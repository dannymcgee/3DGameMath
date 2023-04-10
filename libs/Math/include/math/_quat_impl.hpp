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

template <typename T>
constexpr Quat<T>::Quat() // NOLINT(*-use-equals-default)
{}

template <typename T>
constexpr Quat<T>::Quat(T w, T x, T y, T z)
	: w(w), x(x), y(y), z(z)
{}

template <typename T>
constexpr Quat<T>::Quat(T w, const Vector<3,T>& xyz)
	: w(w)
	, vector(xyz)
{}

template <typename T>
constexpr auto Quat<T>::identity() -> Quat
{
	return { 1, 0, 0, 0 };
}

template <typename T>
constexpr auto Quat<T>::angle_axis(T angle, const Vector<3,T>& unit_axis) -> Quat
{
	T half_angle = 0.5 * angle;
	return {
		std::cos(half_angle),
		std::sin(half_angle) * unit_axis,
	};
}


// Conversion ------------------------------------------------------------------

template <typename T>
inline auto Quat<T>::angle_axis() const -> std::tuple<T, Vector<3,T>>
{
	T angle = 2 * std::acos(w);
	if (math::nearly_equal<T>(angle, 0))
		return { 0, Vector<3,T>::Zero };

	T scale = 1 / std::sqrt(1 - w * w);
	return { angle, vector * scale };
}

template <typename T>
inline auto Quat<T>::euler(Space space) const -> Euler<T>
{
	if (space == Space::Parent2Local)
		return inverse().euler(Space::Local2Parent);

	using namespace math::literals; // NOLINT(*-using-namespace)

	T sin_pitch = -2 * (y * z - w * x);

	// Check for gimbal lock
	if (math::nearly_equal<T>(std::abs(sin_pitch), 1))
		return Euler<T>{
			/* yaw */   std::atan2(-x * z + w * y, 0.5 - y * y - z * z),
			/* pitch */ 90_deg * sin_pitch,
			/* roll */  0,
		};

	return Euler<T>{
		/* yaw */   std::atan2(x * z + w * y, 0.5 - x * x - y * y),
		/* pitch */ std::asin(sin_pitch),
		/* roll */  std::atan2(x * y + w * z, 0.5 - x * x - z * z),
	};
}

template <typename T>
inline auto Quat<T>::matrix(Space space) const -> RotationMatrix<T>
{
	if (space == Space::Parent2Local)
		return inverse().matrix(Space::Local2Parent);

	T two_x2 = 2 * x * x;
	T two_y2 = 2 * y * y;
	T two_z2 = 2 * z * z;

	T two_xy = 2 * x * y;
	T two_xz = 2 * x * z;
	T two_yz = 2 * y * z;

	T two_wx = 2 * w * x;
	T two_wy = 2 * w * y;
	T two_wz = 2 * w * z;

	return RotationMatrix<T>{{
		{ 1-two_y2-two_z2,    two_xy+two_wz,    two_xz-two_wy  },
		{  two_xy-two_wz,    1-two_x2-two_z2,   two_yz+two_wx  },
		{  two_xz+two_wy,     two_yz-two_wx,   1-two_x2-two_y2 },
	}};
}


// Magnitude -------------------------------------------------------------------

template <typename T>
inline auto Quat<T>::magnitude() const -> T
{
	T sq_mag = sq_magnitude();

	if (math::nearly_equal<T>(sq_mag, 1))
		return 1;

	if (math::nearly_equal<T>(sq_mag, 0))
		return 0;

	return std::sqrt(sq_mag);
}

template <typename T>
inline auto Quat<T>::sq_magnitude() const -> T
{
	return w*w + x*x + y*y + z*z;
}


// Normalize -------------------------------------------------------------------

template <typename T>
inline void Quat<T>::normalize()
{
	T sq_mag = sq_magnitude();

	if (math::nearly_equal<T>(sq_mag, 1))
		return;

	if (math::nearly_equal<T>(sq_mag, 0)) {
		*this = Quat::identity();
		return;
	}

	T scale = 1.0 / std::sqrt(sq_mag);
	w *= scale;
	vector *= scale;
}


// Conjugate & Inverse ---------------------------------------------------------

template <typename T>
constexpr auto Quat<T>::conjugate() const -> Quat
{
	return { w, -vector };
}

template <typename T>
constexpr auto Quat<T>::inverse() const -> Quat
{
	T sq_mag = sq_magnitude();

	if (math::nearly_equal<T>(sq_mag, 1))
		return conjugate();

	T scale = 1 / std::sqrt(sq_mag);
	return conjugate() * scale;
}


// Unary negation --------------------------------------------------------------

template <typename T>
inline auto Quat<T>::operator-() const -> Quat
{
	return { -w, -x, -y, -z };
}


// Quaternion multiplication ---------------------------------------------------

template <typename T>
constexpr auto Quat<T>::operator*(const Quat& other) const -> Quat
{
	return {
		(w * other.w) - (vector | other.vector),
		(w * other.vector) + (other.w * vector) + (vector ^ other.vector),
	};
}


// Scalar multiplication -------------------------------------------------------

template <typename T>
constexpr auto Quat<T>::operator*(T scale) const -> Quat
{
	return {
		w * scale,
		vector * scale,
	};
}

template <typename T>
constexpr auto Quat<T>::operator*=(T scale) -> Quat&
{
	w *= scale;
	vector *= scale;

	return *this;
}

} // namespace math

template <typename T>
constexpr auto operator*(T lhs, const math::Quat<T>& rhs) -> math::Quat<T>
{
	return rhs * lhs;
}

namespace math {


// Addition --------------------------------------------------------------------

template <typename T>
constexpr auto Quat<T>::operator+(const Quat& rhs) const -> Quat
{
	return {
		w + rhs.w,
		vector + rhs.vector,
	};
}


// Difference ------------------------------------------------------------------

template <typename T>
constexpr auto Quat<T>::diff(const Quat& rhs) const -> Quat
{
	return rhs * inverse();
}

template <typename T>
constexpr auto Quat<T>::operator-(const Quat& rhs) const -> Quat
{
	return diff(rhs);
}


// Dot product -----------------------------------------------------------------

template <typename T>
constexpr auto Quat<T>::dot(const Quat& rhs) const -> T
{
	return (w * rhs.w) + (vector | rhs.vector);
}

template <typename T>
constexpr auto Quat<T>::operator|(const Quat& rhs) const -> T
{
	return dot(rhs);
}


// Exponentiation --------------------------------------------------------------

template <typename T>
constexpr auto Quat<T>::pow(T exp) const -> Quat
{
	if (math::nearly_equal<T>(w, 1))
		return Quat::identity();

	T alpha = std::acos(w);
	T out_alpha = alpha * exp;

	return {
		std::cos(out_alpha),
		vector * std::sin(out_alpha) / std::sin(alpha),
	};
}


// Rotation --------------------------------------------------------------------

template <typename T>
constexpr auto Quat<T>::rotate_point(const Vector<3,T>& point) const -> Vector<3,T>
{
	Quat result = *this * Quat{ 0, point } * inverse();
	return result.vector;
}


// Spherical interpolation -----------------------------------------------------

template <typename T>
inline auto Quat<T>::slerp(const Quat<T>& src, const Quat<T>& dest, T t) -> Quat
{
	return (dest - src).pow(t) * src;
}

template <typename T>
inline auto Quat<T>::slerp(const Quat<T>& dest, T t) const -> Quat
{
	return Quat::slerp(*this, dest, t);
}


// Misc / Utility --------------------------------------------------------------

template <typename T>
auto Quat<T>::to_string(usize precision) const -> std::string
{
	auto formatter = fmt::AlignedValues(begin(), end(), precision);
	return to_string(formatter);
}

template <typename T>
inline auto Quat<T>::to_string(const fmt::AlignedValues& formatter) const -> std::string
{
	return ::fmt::format("[ {}  ( {}  {}  {} )]",
		formatter.format(w),
		formatter.format(x),
		formatter.format(y),
		formatter.format(z));
}


// Iterator support ------------------------------------------------------------

template <typename T>
inline auto Quat<T>::begin() -> detail::RawIterator<T>
{
	return detail::RawIterator(&w);
}

template <typename T>
inline auto Quat<T>::begin() const -> detail::RawConstIterator<T>
{
	return detail::RawConstIterator(&w);
}

template <typename T>
inline auto Quat<T>::end() -> detail::RawIterator<T>
{
	return detail::RawIterator(&z + 1);
}

template <typename T>
inline auto Quat<T>::end() const -> detail::RawConstIterator<T>
{
	return detail::RawConstIterator(&z + 1);
}


// Subscript operator ----------------------------------------------------------

template <typename T>
inline auto Quat<T>::operator[](usize idx) -> T&
{
	switch (idx) {
		case 0: return w;
		default: return vector[idx + 1];
	}
}

template <typename T>
inline auto Quat<T>::operator[](usize idx) const -> T
{
	ASSERT(idx < 4, "Index out of range");

	switch (idx) {
		case 0: return w;
		default: return vector[idx + 1];
	}
}


// Structured binding support --------------------------------------------------

template <typename T>
template <usize Index, typename U>
inline auto Quat<T>::get_helper(U&& self) -> auto&&
{
	if constexpr (Index == 0) return std::forward<U>(self).w;
	if constexpr (Index == 1) return std::forward<U>(self).x;
	if constexpr (Index == 2) return std::forward<U>(self).y;
	if constexpr (Index == 3) return std::forward<U>(self).z;
}

} // namespace math

namespace std {

// NOLINTBEGIN(cert-dcl58-cpp):

// I believe cert-dcl58-cpp here is a spurious warning due to limitations of the
// clang-tidy parser. This is just a template specialization with a user-defined
// type, which is normally permitted by the linter -- but because our type
// is templated, the parser is unable to correctly identify what we're doing.

template <typename T>
struct tuple_size<::math::Quat<T>> {
	static constexpr size_t value = 4;
};

template <size_t Index, typename T>
struct tuple_element<Index, ::math::Quat<T>> {
	static_assert(Index < 4, "Index out of range");
	using type = T;
};

// NOLINTEND(cert-dcl58-cpp)
}

