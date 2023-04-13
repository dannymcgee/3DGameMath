#pragma once

#include "math/_vector_decl.h"

#include <cmath>
#include <string>
#include <utility>

#include "math/assert.h"
#include "math/polar.h"


namespace math {

// Static Zero -----------------------------------------------------------------

template <usize D>
const Vector<D> Vector<D>::Zero {};


// Static methods --------------------------------------------------------------

template <usize D>
constexpr auto Vector<D>::all(flt value) -> Vector
{
	Vector result;
	for (usize i = 0; i < D; ++i)
		result.components[i] = value;

	return result;
}

template <usize D>
constexpr auto Vector<D>::unit_x() -> Vector
{
	return Vector{ 1 };
}

template <usize D>
constexpr auto Vector<D>::unit_y() -> Vector
{
	static_assert(D >= 2);
	return Vector{ 0, 1 };
}

template <usize D>
constexpr auto Vector<D>::unit_z() -> Vector
{
	static_assert(D >= 3);
	return Vector{ 0, 0, 1 };
}

template <usize D>
constexpr auto Vector<D>::unit_w() -> Vector
{
	static_assert(D >= 4);
	return Vector{ 0, 0, 0, 1 };
}

template <usize D>
constexpr auto Vector<D>::up() -> Vector
{
	static_assert(D == 3);
	return Vector{ 0, 1, 0 };
}

template <usize D>
constexpr auto Vector<D>::right() -> Vector
{
	static_assert(D == 3);
	return Vector{ 1, 0, 0 };
}

template <usize D>
constexpr auto Vector<D>::forward() -> Vector
{
	static_assert(D == 3);
	return Vector{ 0, 0, 1 };
}

template <usize D>
constexpr auto Vector<D>::from_polar(flt radius, flt angle) -> Vector
{
	static_assert(D == 2, "Polar coordinates are only supported by 2-Dimensional vectors");
	return {
		radius * std::cos(angle),
		radius * std::sin(angle),
	};
}

template <usize D>
constexpr auto Vector<D>::from_polar(const PolarCoords& coords) -> Vector
{
	auto [radius, angle] = coords;
	return Vector::from_polar(radius, angle);
}

template <usize D>
constexpr auto Vector<D>::from_polar(flt radius, flt heading, flt pitch) -> Vector
{
	static_assert(D == 3, "Polar coordinates are only supported by 2-Dimensional vectors");
	return {
		radius * std::cos(pitch) * std::sin(heading),
		-radius * std::sin(pitch),
		radius * std::cos(pitch) * std::cos(heading),
	};
}

template <usize D>
constexpr auto Vector<D>::from_polar(const SphericalCoords& coords) -> Vector
{
	auto [radius, heading, declination] = coords;
	return Vector::from_polar(radius, heading, declination);
}


// Iterator support ------------------------------------------------------------

template <usize D>
inline auto Vector<D>::begin() -> detail::RawIterator<flt>
{
	return detail::RawIterator(&components[0]);
}

template <usize D>
inline auto Vector<D>::begin() const -> detail::RawConstIterator<flt>
{
	return detail::RawConstIterator(&components[0]);
}

template <usize D>
inline auto Vector<D>::end() -> detail::RawIterator<flt>
{
	return detail::RawIterator(&components[D]);
}

template <usize D>
inline auto Vector<D>::end() const -> detail::RawConstIterator<flt>
{
	return detail::RawConstIterator(&components[D]);
}


// Subscript operator ----------------------------------------------------------

template <usize D>
inline auto Vector<D>::operator[](usize idx) -> flt&
{
	validate_index(idx);
	return components[idx];
}

template <usize D>
inline auto Vector<D>::operator[](usize idx) const -> flt
{
	validate_index(idx);
	return components[idx];
}

template <usize D>
inline void Vector<D>::validate_index(usize idx) const // NOLINT(*-unused-parameters)
{
	ASSERT(idx < D,
		"Index out of range for Vec<{}>: Expected < {}, received {}",
		D, D, idx);
}


// Unary negation --------------------------------------------------------------

template <usize D>
inline auto Vector<D>::operator-() const -> Vector
{
	auto result = *this;
	for (usize i = 0; i < D; ++i)
		result.components[i] *= -1;

	return result;
}


// Vector addition -------------------------------------------------------------

template <usize D>
inline auto Vector<D>::operator+(const Vector& other) const -> Vector
{
	auto result = *this;
	for (usize i = 0; i < D; ++i)
		result.components[i] += other.components[i];

	return result;
}

template <usize D>
inline auto Vector<D>::operator+=(const Vector& other) -> Vector&
{
	for (usize i = 0; i < D; ++i)
		components[i] += other.components[i];

	return *this;
}


// Vector subtraction ----------------------------------------------------------

template <usize D>
inline auto Vector<D>::operator-(const Vector& other) const -> Vector
{
	auto result = *this;
	for (usize i = 0; i < D; ++i)
		result.components[i] -= other.components[i];

	return result;
}

template <usize D>
inline auto Vector<D>::operator-=(const Vector& other) -> Vector&
{
	for (usize i = 0; i < D; ++i)
		components[i] -= other.components[i];

	return *this;
}


// Scalar multiplication -------------------------------------------------------

template <usize D>
inline auto Vector<D>::operator*(flt magnitude) const -> Vector
{
	auto result = *this;
	for (usize i = 0; i < D; ++i)
		result.components[i] *= magnitude;

	return result;
}

template <usize D>
inline auto Vector<D>::operator*=(flt magnitude) -> Vector&
{
	for (usize i = 0; i < D; ++i)
		components[i] *= magnitude;

	return *this;
}
} // namespace math

template <sized::usize D, typename flt>
inline auto operator*(flt lhs, const math::Vector<D>& rhs) -> math::Vector<D>
{
	return rhs * lhs;
}

namespace math {


// Scalar division -------------------------------------------------------------

template <usize D>
inline auto Vector<D>::operator/(flt magnitude) const -> Vector
{
	if (math::nearly_equal(magnitude, 0))
		return Zero;

	auto result = *this;
	for (usize i = 0; i < D; ++i)
		result.components[i] /= magnitude;

	return result;
}

template <usize D>
inline auto Vector<D>::operator/=(flt magnitude) -> Vector&
{
	if (math::nearly_equal(magnitude, 0)) {
		for (usize i = 0; i < D; ++i)
			components[i] = 0;

		return *this;
	}

	for (usize i = 0; i < D; ++i)
		components[i] /= magnitude;

	return *this;
}


// Equality comparison ---------------------------------------------------------

template <usize D>
inline auto Vector<D>::operator==(const Vector& other) const -> bool
{
	if (this == &other)
		return true;

	for (usize i = 0; i < D; ++i)
		if (!math::nearly_equal(components[i], other.components[i]))
			return false;

	return true;
}

template <usize D>
inline auto Vector<D>::operator!=(const Vector& other) const -> bool
{
	if (this == &other)
		return false;

	for (usize i = 0; i < D; ++i)
		if (!math::nearly_equal(components[i], other.components[i]))
			return true;

	return false;
}


// Length / Magnitude ----------------------------------------------------------

template <usize D>
inline auto Vector<D>::length() const -> flt
{
	return std::sqrt(sq_length());
}

template <usize D>
inline auto Vector<D>::magnitude() const -> flt
{
	return length();
}

template <usize D>
inline auto Vector<D>::sq_length() const -> flt
{
	flt result = 0;
	for (usize i = 0; i < D; ++i)
		result += components[i] * components[i];

	return result;
}


// Unit-Length Direction -------------------------------------------------------

template <usize D>
inline auto Vector<D>::normal() const -> Vector
{
	flt sq_len = sq_length();

	if (math::nearly_equal(sq_len, 0))
		return Zero;

	if (math::nearly_equal(sq_len, 1))
		return *this;

	flt scale = 1.0 / std::sqrt(sq_len);

	Vector result;
	for (usize i = 0; i < D; ++i)
		result.components[i] = components[i] * scale;

	return result;
}

template <usize D>
inline auto Vector<D>::direction() const -> Vector
{
	return normal();
}

template <usize D>
inline auto Vector<D>::unit() const -> Vector
{
	return normal();
}

template <usize D>
inline void Vector<D>::normalize()
{
	flt sq_len = sq_length();

	if (math::nearly_equal(sq_len, 0)) {
		*this = Zero;
		return;
	}

	if (math::nearly_equal(sq_len, 1))
		return;

	flt scale = 1.0 / std::sqrt(sq_len);
	for (usize i = 0; i < D; ++i)
		components[i] *= scale;
}


// Length and Direction --------------------------------------------------------

template <usize D>
inline auto Vector<D>::length_and_direction() const -> std::tuple<flt, Vector>
{
	flt len = length();

	if (math::nearly_equal(len, 0))
		return { len, Zero };

	flt scale = 1 / len;
	Vector normal;
	for (usize i = 0; i < D; ++i)
		normal.components[i] = components[i] * scale;

	return { len, normal };
}


// Distance --------------------------------------------------------------------

template <usize D>
inline auto Vector<D>::dist(const Vector& other) const -> flt
{
	flt result = 0;
	for (usize i = 0; i < D; ++i) {
		auto diff = other.components[i] - components[i];
		result += diff * diff;
	}

	return std::sqrt(result);
}

template <usize D>
inline auto Vector<D>::dist(const Vector& lhs, const Vector& rhs) -> flt
{
	return lhs.dist(rhs);
}


// Dot-product -----------------------------------------------------------------

template <usize D>
inline auto Vector<D>::dot(const Vector& other) const -> flt
{
	flt result = 0;
	for (usize i = 0; i < D; ++i)
		result += components[i] * other.components[i];

	return result;
}

template <usize D>
inline auto Vector<D>::operator|(const Vector& other) const -> flt
{
	return dot(other);
}


// Cross-product ---------------------------------------------------------------

template <usize D>
inline auto Vector<D>::cross(const Vector& other) const -> Vector
{
	static_assert(D == 3, "Cross-product is only valid for 3-dimensional vectors.");

	return Vector{
		this->y * other.z - this->z * other.y,
		this->z * other.x - this->x * other.z,
		this->x * other.y - this->y * other.x,
	};
}

template <usize D>
inline auto Vector<D>::operator^(const Vector& other) const -> Vector
{
	static_assert(D == 3, "Cross-product is only valid for 3-dimensional vectors.");

	return cross(other);
}

template <usize D>
inline auto Vector<D>::operator^=(const Vector& other) -> Vector&
{
	static_assert(D == 3, "Cross-product is only valid for 3-dimensional vectors.");

	flt temp_x = this->x;
	flt temp_y = this->y;

	this->x = this->y * other.z - this->z * other.y;
	this->y = this->z * other.x - temp_x * other.z;
	this->z = temp_x * other.y - temp_y * other.x;

	return *this;
}


// Misc / Utility --------------------------------------------------------------

template <usize D>
auto Vector<D>::to_string(usize precision) const -> std::string
{
	auto formatter = fmt::AlignedValues(begin(), end(), precision);
	return to_string(formatter);
}

template <usize D>
inline auto Vector<D>::to_string(const fmt::AlignedValues& formatter) const -> std::string
{
	std::string result = "[ ";
	for (usize i = 0; i < D; ++i) {
		result += formatter.format(components[i]);

		if (i < D - 1)
			result += "  ";
	}
	result += " ]";

	return result;
}

} // namespace math
