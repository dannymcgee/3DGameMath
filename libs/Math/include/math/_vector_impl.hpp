#pragma once

#include "math/_vector_decl.h"

#include <cmath>
#include <string>
#include <utility>

#include "math/assert.h"


namespace math {

// Static Zero -----------------------------------------------------------------

template <usize D, typename T>
const Vector<D,T> Vector<D,T>::Zero {};


// Static methods --------------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::all(T value) -> Vector
{
	Vector result;
	for (usize i = 0; i < D; ++i)
		result.components[i] = value;

	return result;
}

template <usize D, typename T>
constexpr auto Vector<D,T>::from_polar(T radius, T angle) -> Vector
{
	static_assert(D == 2, "Polar coordinates are only supported by 2-Dimensional vectors");
	return {
		radius * std::cos(angle),
		radius * std::sin(angle),
	};
}

template <usize D, typename T>
constexpr auto Vector<D,T>::from_polar(const PolarCoords<T>& coords) -> Vector
{
	auto [radius,angle] = coords;
	return from_polar(radius, angle);
}

template <usize D, typename T>
constexpr auto Vector<D,T>::from_polar(T radius, T heading, T declination) -> Vector
{
	static_assert(D == 3, "Polar coordinates are only supported by 2-Dimensional vectors");
	return {
		radius * std::cos(declination) * std::sin(heading),
		-radius * std::sin(declination),
		radius * std::cos(declination) * std::cos(heading),
	};
}

template <usize D, typename T>
constexpr auto Vector<D,T>::from_polar(const SphericalCoords<T>& coords) -> Vector
{
	auto [radius,heading,declination] = coords;
	return from_polar(radius, heading, declination);
}


// Iterator support ------------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::begin() -> detail::RawIterator<T>
{
	return detail::RawIterator(&components[0]);
}

template <usize D, typename T>
inline auto Vector<D,T>::begin() const -> detail::RawConstIterator<T>
{
	return detail::RawConstIterator(&components[0]);
}

template <usize D, typename T>
inline auto Vector<D,T>::end() -> detail::RawIterator<T>
{
	return detail::RawIterator(&components[D]);
}

template <usize D, typename T>
inline auto Vector<D,T>::end() const -> detail::RawConstIterator<T>
{
	return detail::RawConstIterator(&components[D]);
}


// Subscript operator ----------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::operator[](usize idx) -> T&
{
	validate_index(idx);
	return components[idx];
}

template <usize D, typename T>
inline auto Vector<D,T>::operator[](usize idx) const -> T
{
	validate_index(idx);
	return components[idx];
}

template <usize D, typename T>
inline void Vector<D,T>::validate_index(usize idx) const // NOLINT(*-unused-parameters)
{
	ASSERT(idx < D,
		"Index out of range for Vec<{}>: Expected < {}, received {}",
		D, D, idx);
}


// Unary negation --------------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::operator-() const -> Vector
{
	auto result = *this;
	for (usize i = 0; i < D; ++i)
		result.components[i] *= -1;

	return result;
}


// Vector addition -------------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::operator+(const Vector& other) const -> Vector
{
	auto result = *this;
	for (usize i = 0; i < D; ++i)
		result.components[i] += other.components[i];

	return result;
}

template <usize D, typename T>
inline auto Vector<D,T>::operator+=(const Vector& other) -> Vector&
{
	for (usize i = 0; i < D; ++i)
		components[i] += other.components[i];

	return *this;
}


// Vector subtraction ----------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::operator-(const Vector& other) const -> Vector
{
	auto result = *this;
	for (usize i = 0; i < D; ++i)
		result.components[i] -= other.components[i];

	return result;
}

template <usize D, typename T>
inline auto Vector<D,T>::operator-=(const Vector& other) -> Vector&
{
	for (usize i = 0; i < D; ++i)
		components[i] -= other.components[i];

	return *this;
}


// Scalar multiplication -------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::operator*(T magnitude) const -> Vector
{
	auto result = *this;
	for (usize i = 0; i < D; ++i)
		result.components[i] *= magnitude;

	return result;
}

template <usize D, typename T>
inline auto Vector<D,T>::operator*=(T magnitude) -> Vector&
{
	for (usize i = 0; i < D; ++i)
		components[i] *= magnitude;

	return *this;
}
} // namespace math

template <sized::usize D, typename T>
inline auto operator*(T lhs, const math::Vector<D,T>& rhs) -> math::Vector<D,T>
{
	return rhs * lhs;
}

namespace math {


// Scalar division -------------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::operator/(T magnitude) const -> Vector
{
	if (nearly_equal<T>(magnitude, 0))
		return Zero;

	auto result = *this;
	for (usize i = 0; i < D; ++i)
		result.components[i] /= magnitude;

	return result;
}

template <usize D, typename T>
inline auto Vector<D,T>::operator/=(T magnitude) -> Vector&
{
	if (nearly_equal<T>(magnitude, 0)) {
		for (usize i = 0; i < D; ++i)
			components[i] = 0;

		return *this;
	}

	for (usize i = 0; i < D; ++i)
		components[i] /= magnitude;

	return *this;
}


// Equality comparison ---------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::operator==(const Vector& other) const -> bool
{
	if (this == &other)
		return true;

	for (usize i = 0; i < D; ++i)
		if (!nearly_equal<T>(components[i], other.components[i]))
			return false;

	return true;
}

template <usize D, typename T>
inline auto Vector<D,T>::operator!=(const Vector& other) const -> bool
{
	if (this == &other)
		return false;

	for (usize i = 0; i < D; ++i)
		if (!nearly_equal<T>(components[i], other.components[i]))
			return true;

	return false;
}


// Length / Magnitude ----------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::length() const -> T
{
	T result = 0;
	for (usize i = 0; i < D; ++i)
		result += components[i] * components[i];

	return std::sqrt(result);
}

template <usize D, typename T>
inline auto Vector<D,T>::magnitude() const -> T
{
	return length();
}


// Unit-Length Direction -------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::normal() const -> Vector
{
	Vector result;
	auto len = length();

	if (nearly_equal<T>(len, 0))
		return Zero;

	for (usize i = 0; i < D; ++i)
		result.components[i] = components[i] / len;

	return result;
}

template <usize D, typename T>
inline auto Vector<D,T>::direction() const -> Vector
{
	return normal();
}

template <usize D, typename T>
inline auto Vector<D,T>::unit() const -> Vector
{
	return normal();
}

template <usize D, typename T>
inline void Vector<D,T>::normalize()
{
	auto len = length();

	if (nearly_equal<T>(len, 0)) {
		*this = Zero;
		return;
	}

	for (usize i = 0; i < D; ++i)
		components[i] /= len;
}


// Length and Direction --------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::length_and_direction() const -> std::tuple<T, Vector>
{
	Vector normal;
	auto len = length();

	if (nearly_equal<T>(len, 0))
		return { len, Zero };

	for (usize i = 0; i < D; ++i)
		normal.components[i] = components[i] / len;

	return { len, normal };
}


// Distance --------------------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::dist(const Vector& other) const -> T
{
	T result = 0;
	for (usize i = 0; i < D; ++i) {
		auto diff = other.components[i] - components[i];
		result += diff * diff;
	}

	return std::sqrt(result);
}

template <usize D, typename T>
inline auto Vector<D,T>::dist(const Vector& lhs, const Vector& rhs) -> T
{
	return lhs.dist(rhs);
}


// Dot-product -----------------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::dot(const Vector& other) const -> T
{
	T result = 0;
	for (usize i = 0; i < D; ++i)
		result += components[i] * other.components[i];

	return result;
}

template <usize D, typename T>
inline auto Vector<D,T>::operator|(const Vector& other) const -> T
{
	return dot(other);
}


// Cross-product ---------------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::cross(const Vector& other) const -> Vector
{
	static_assert(D == 3, "Cross-product is only valid for 3-dimensional vectors.");

	return Vector{
		this->y * other.z - this->z * other.y,
		this->z * other.x - this->x * other.z,
		this->x * other.y - this->y * other.x,
	};
}

template <usize D, typename T>
inline auto Vector<D,T>::operator^(const Vector& other) const -> Vector
{
	static_assert(D == 3, "Cross-product is only valid for 3-dimensional vectors.");

	return cross(other);
}

template <usize D, typename T>
inline auto Vector<D,T>::operator^=(const Vector& other) -> Vector&
{
	static_assert(D == 3, "Cross-product is only valid for 3-dimensional vectors.");

	T temp_x = this->x;
	T temp_y = this->y;

	this->x = this->y * other.z - this->z * other.y;
	this->y = this->z * other.x - temp_x * other.z;
	this->z = temp_x * other.y - temp_y * other.x;

	return *this;
}


// Misc / Utility --------------------------------------------------------------

template <usize D, typename T>
auto Vector<D,T>::to_string(usize precision) const -> std::string
{
	auto formatter = fmt::AlignedValues(begin(), end(), precision);
	return to_string(formatter);
}

template <usize D, typename T>
inline auto Vector<D,T>::to_string(const fmt::AlignedValues& formatter) const -> std::string
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


// Structured binding support --------------------------------------------------

namespace std {

// NOLINTBEGIN(cert-dcl58-cpp):

// I believe cert-dcl58-cpp here is a spurious warning due to limitations of the
// clang-tidy parser. This is just a template specialization with a user-defined
// type, which is normally permitted by the linter -- but because our type
// is templated, the parser is unable to correctly identify what we're doing.

template <size_t D, typename T>
struct tuple_size<::math::Vector<D,T>> {
	static constexpr size_t value = D;
};

template <size_t Index, size_t D, typename T>
struct tuple_element<Index, ::math::Vector<D,T>> {
	static_assert(Index < D, "Index out of range");
	using type = T;
};

// NOLINTEND(cert-dcl58-cpp)
}
