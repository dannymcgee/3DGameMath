#pragma once

#include <array>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <utility>

#include <fmt/format.h>
#include <sized.h>

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
// NOLINTBEGIN(*-pro-type-member-init)
	union {
		std::array<T,D> components;
	};
};

template <typename T>
struct Vector<2, T> {
	union {
		std::array<T,2> components { 0, 0 };
		struct { T x, y; };
	};
};

template <typename T>
struct Vector<3, T> {
	union {
		std::array<T,3> components { 0, 0, 0 };
		struct { T x, y, z; };
	};

	/** Calculate the cross-product of two vectors. */
	inline auto cross(const Vector& other) const -> Vector;

	/** Calculate the cross-product of two vectors. */
	inline auto operator^(const Vector& other) const -> Vector;

	/** Cross this vector with another in-place. */
	inline auto operator^=(const Vector& other) -> Vector&;
};

template <typename T>
struct Vector<4, T> {
	union {
		std::array<T,4> components { 0, 0, 0, 0 };
		struct { T x, y, z, w; };
	};
// NOLINTEND(*-pro-type-member-init)
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
	inline auto begin() const { return components.begin(); }
	inline auto end() const { return components.end(); }

	// Subscript operator
	inline auto operator[](usize idx) -> T&;
	inline auto operator[](usize idx) const -> T;

	// Unary negation
	inline auto operator-() const -> Vector;

	// Vector addition
	inline auto operator+(const Vector& other) const -> Vector;
	inline auto operator+=(const Vector& other) -> Vector&;

	// Vector subtraction
	inline auto operator-(const Vector& other) const -> Vector;
	inline auto operator-=(const Vector& other) -> Vector&;

	// Scalar multiplication
	inline auto operator*(T magnitude) const -> Vector;
	inline auto operator*=(T magnitude) -> Vector&;

	// Scalar division
	inline auto operator/(T magnitude) const -> Vector;
	inline auto operator/=(T magnitude) -> Vector&;

	// Equality comparison
	inline auto operator==(const Vector& other) const -> bool;

	// Not-equal comparison
	inline auto operator!=(const Vector& other) const -> bool;

	/** Calculate the length (magnitude) of the vector. */
	inline auto length() const -> T;
	/** Calculate the length (magnitude) of the vector. */
	inline auto magnitude() const -> T;

	/** Calculate the unit-length direction of the vector. */
	inline auto unit() const -> Vector;
	/** Calculate the unit-length direction of the vector. */
	inline auto direction() const -> Vector;
	/** Calculate the unit-length direction of the vector. */
	inline auto normal() const -> Vector;
	/** Normalize the vector in place. */
	inline void normalize();

	/**
	 * Calculate the magnitude and unit-length direction of the vector in a
	 * single operation.
	 */
	inline auto length_and_direction() const -> std::tuple<T, Vector>;

	/** Calculate the distance between two points. */
	inline auto dist(const Vector& other) const -> T;
	/** Calculate the distance between two points. */
	static inline auto dist(const Vector& lhs, const Vector& rhs) -> T;

	/** Calculate the dot-product of two vectors. */
	inline auto dot(const Vector& other) const -> T;
	/** Calculate the dot-product of two vectors. */
	inline auto operator|(const Vector& other) const -> T;

private:
	inline void validate_index(usize idx) const;
};


// Definitions /////////////////////////////////////////////////////////////////

// math::detail::Vector<3, T> ==================================================

namespace detail {

// Cross-Product ---------------------------------------------------------------

template <typename T>
inline auto Vector<3, T>::cross(const Vector& other) const -> Vector
{
	return Vector{
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x,
	};
}

template <typename T>
inline auto Vector<3, T>::operator^(const Vector& other) const -> Vector
{
	return cross(other);
}

template <typename T>
inline auto Vector<3, T>::operator^=(const Vector& other) -> Vector&
{
	auto temp_x = x;
	auto temp_y = y;

	x = y * other.z - z * other.y;
	y = z * other.x - temp_x * other.z;
	z = temp_x * other.y - temp_y * other.x;

	return *this;
}

}


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
	if (idx > D) {
		auto err_msg = fmt::format(
			"Index out of range for Vec<{}>: Expected < {}, received {}",
			D, D, idx
		);

		throw std::exception{ err_msg.c_str(), 1 };
	}
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
		return Vector::all(0);

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

}


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
