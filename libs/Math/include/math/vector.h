#pragma once

#include <array>
#include <cmath>
#include <cstdlib>
#include <utility>

#include <fmt/format.h>
#include <sized.h>

#include "math/assert.h"
#include "math/fmt.h"
#include "math/utility.h"

namespace math {

using namespace sized; // NOLINT(*-using-namespace)


// Declarations ////////////////////////////////////////////////////////////////

namespace detail {


// math::detail::Vector ========================================================

/**
 * Helper base struct to allow for specializing struct members for specific
 * template instances without needing to redeclare all methods for every
 * specialization.
 */
template <usize D, typename T = f64>
struct Vector {
// NOLINTBEGIN(*-pro-type-member-init, *-avoid-c-arrays)
	union {
		T components[D];
	};
};

template <typename T>
struct Vector<2, T> {
	union {
		T components[2] { 0, 0 };
		struct { T x, y; };
	};
};

template <typename T>
struct Vector<3, T> {
	union {
		T components[3] { 0, 0, 0 };
		struct { T x, y, z; };
	};
};

template <typename T>
struct Vector<4, T> {
	union {
		T components[4] { 0, 0, 0, 0 };
		struct { T x, y, z, w; };
	};
// NOLINTEND(*-pro-type-member-init, *-avoid-c-arrays)
};
}


// math::Vector ================================================================

/**
 * A simple data structure that can be used to represent a geometric point or
 * vector in 2D or 3D space.
 *
 * @tparam D The dimensionality of the vector. Full support for `2`, `3`, or `4`.
 * @tparam T The scalar type of the components. Full support for `float` or `double`.
 */
template <usize D, typename T = f64>
struct Vector : public detail::Vector<D,T> {
private:
	using detail::Vector<D,T>::components;

public:
	static const Vector Zero;

	/** Create a vector where all components have the same value. */
	static auto all(T value) -> Vector;

	// Structured binding support
	template <usize Index> inline auto get() &       { return components[Index]; }
	template <usize Index> inline auto get() const&  { return components[Index]; }
	template <usize Index> inline auto get() &&      { return components[Index]; }
	template <usize Index> inline auto get() const&& { return components[Index]; }

	// Iterator support
	auto begin() -> detail::RawIterator<T>;
	auto begin() const -> detail::RawConstIterator<T>;

	auto end() -> detail::RawIterator<T>;
	auto end() const -> detail::RawConstIterator<T>;

	// Subscript operator
	auto operator[](usize idx) -> T&;
	auto operator[](usize idx) const -> T;

	// Unary negation
	auto operator-() const -> Vector;

	// Vector addition
	auto operator+(const Vector& other) const -> Vector;
	auto operator+=(const Vector& other) -> Vector&;

	// Vector subtraction
	auto operator-(const Vector& other) const -> Vector;
	auto operator-=(const Vector& other) -> Vector&;

	// Scalar multiplication
	auto operator*(T magnitude) const -> Vector;
	auto operator*=(T magnitude) -> Vector&;

	// Scalar division
	auto operator/(T magnitude) const -> Vector;
	auto operator/=(T magnitude) -> Vector&;

	// Equality comparison
	auto operator==(const Vector& other) const -> bool;

	// Not-equal comparison
	auto operator!=(const Vector& other) const -> bool;

	/** Calculate the length (magnitude) of the vector. */
	auto length() const -> T;
	/** Calculate the length (magnitude) of the vector. */
	auto magnitude() const -> T;

	/** Calculate the unit-length direction of the vector. */
	auto unit() const -> Vector;
	/** Calculate the unit-length direction of the vector. */
	auto direction() const -> Vector;
	/** Calculate the unit-length direction of the vector. */
	auto normal() const -> Vector;
	/** Normalize the vector in place. */
	void normalize();

	/**
	 * Calculate the magnitude and unit-length direction of the vector in a
	 * single operation.
	 */
	auto length_and_direction() const -> std::tuple<T, Vector>;

	/** Calculate the distance between two points. */
	auto dist(const Vector& other) const -> T;
	/** Calculate the distance between two points. */
	static auto dist(const Vector& lhs, const Vector& rhs) -> T;

	/** Calculate the dot-product of two vectors. */
	auto dot(const Vector& other) const -> T;
	/** Calculate the dot-product of two vectors. */
	auto operator|(const Vector& other) const -> T;

	/** Calculate the cross-product of two vectors. */
	auto cross(const Vector& other) const -> Vector;
	/** Calculate the cross-product of two vectors. */
	auto operator^(const Vector& other) const -> Vector;
	/** Cross this vector with another in-place. */
	auto operator^=(const Vector& other) -> Vector&;

	// Misc / Utility
	auto to_string(usize precision = 3) const -> std::string;
	auto to_string(const fmt::AlignedValues& formatter) const -> std::string;

private:
	void validate_index(usize idx) const;
};


// Definitions /////////////////////////////////////////////////////////////////

// math::Vector<D,T> ===========================================================

// Static Zero -----------------------------------------------------------------

template <usize D, typename T>
const Vector<D,T> Vector<D,T>::Zero {};


// Static All(T) ---------------------------------------------------------------

template <usize D, typename T>
inline auto Vector<D,T>::all(T value) -> Vector
{
	Vector result;
	for (usize i = 0; i < D; ++i)
		result.components[i] = value;

	return result;
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
inline void Vector<D,T>::validate_index(usize idx) const
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


// Not-equal comparison --------------------------------------------------------

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
