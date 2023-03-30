#pragma once

#include <cmath>
#include <cstdlib>
#include <exception>
#include <type_traits>
#include <utility>

#include <fmt/format.h>
#include <sized.h>

#include "math/utility.h"

namespace math {

using namespace sized; // NOLINT(*-using-namespace)


// Declarations ////////////////////////////////////////////////////////////////

namespace internal {


// math::internal::Vec ---------------------------------------------------------

/**
 * Helper base struct to allow for specializing struct members for specific
 * template instances without needing to redeclare all methods for every
 * specialization.
 */
template <usize D, typename T>
struct Vec {
// NOLINTBEGIN(*-avoid-c-arrays)
	union {
		T components[D];
	};
};

template <typename T>
struct Vec<2, T> {
	union {
		T components[2] { 0, 0 };
		struct { T x, y; };
	};
};

template <typename T>
struct Vec<3, T> {
	union {
		T components[3] { 0, 0, 0 };
		struct { T x, y, z; };
	};

	/** Calculate the cross-product of two vectors. */
	inline auto cross(const Vec& other) const -> Vec;

	/** Calculate the cross-product of two vectors. */
	inline auto operator^(const Vec& other) const -> Vec;

	/** Cross this vector with another in-place. */
	inline auto operator^=(const Vec& other) -> Vec&;
};

template <typename T>
struct Vec<4, T> {
	union {
		T components[4] { 0, 0, 0, 0 };
		struct { T x, y, z, w; };
	};
// NOLINTEND(*-avoid-c-arrays)
};
}


// math::Vec -------------------------------------------------------------------

/**
 * A simple data structure that can be used to represent a geometric point or
 * vector in 2D or 3D space.
 *
 * @tparam D The dimensionality of the vector. Full support for `2`, `3`, or `4`.
 * @tparam T The scalar type of the components. Full support for `float` or `double`.
 */
template <usize D, typename T>
struct Vec : public internal::Vec<D, T> {
private:
	using internal::Vec<D, T>::components;

public:
	static const Vec Zero;

	/** Create a vector where all components have the same value. */
	static auto all(T value) -> Vec;

	// Structured binding support
	template <usize Index> inline auto get() &       { return components[Index]; }
	template <usize Index> inline auto get() const&  { return components[Index]; }
	template <usize Index> inline auto get() &&      { return components[Index]; }
	template <usize Index> inline auto get() const&& { return components[Index]; }

	// Subscript operator
	inline auto operator[](usize idx) -> T&;

	// Unary negation
	inline auto operator-() const -> Vec;

	// Vector addition
	inline auto operator+(const Vec& other) const -> Vec;
	inline auto operator+=(const Vec& other) -> Vec&;

	// Vector subtraction
	inline auto operator-(const Vec& other) const -> Vec;
	inline auto operator-=(const Vec& other) -> Vec&;

	// Scalar multiplication
	inline auto operator*(T magnitude) const -> Vec;
	inline auto operator*=(T magnitude) -> Vec&;

	// Scalar division
	inline auto operator/(T magnitude) const -> Vec;
	inline auto operator/=(T magnitude) -> Vec&;

	// Equality comparison
	inline auto operator==(const Vec& other) const -> bool;

	// Not-equal comparison
	inline auto operator!=(const Vec& other) const -> bool;

	/** Calculate the length (magnitude) of the vector. */
	inline auto length() const -> T;
	/** Calculate the length (magnitude) of the vector. */
	inline auto magnitude() const -> T;

	/** Calculate the unit-length direction of the vector. */
	inline auto unit() const -> Vec;
	/** Calculate the unit-length direction of the vector. */
	inline auto direction() const -> Vec;
	/** Calculate the unit-length direction of the vector. */
	inline auto normal() const -> Vec;
	/** Normalize the vector in place. */
	inline void normalize();

	/** Calculate the distance between two points. */
	inline auto dist(const Vec& other) const -> T;
	/** Calculate the distance between two points. */
	static inline auto dist(const Vec& lhs, const Vec& rhs) -> T;

	/** Calculate the dot-product of two vectors. */
	inline auto dot(const Vec& other) const -> T;
	/** Calculate the dot-product of two vectors. */
	inline auto operator|(const Vec& other) const -> T;
};


// Definitions /////////////////////////////////////////////////////////////////

// math::internal::Vec<3, T> ===================================================

namespace internal {

// Cross-Product ---------------------------------------------------------------

template <typename T>
inline auto Vec<3, T>::cross(const Vec& other) const -> Vec
{
	return Vec{
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x,
	};
}

template <typename T>
inline auto Vec<3, T>::operator^(const Vec& other) const -> Vec
{
	return cross(other);
}

template <typename T>
inline auto Vec<3, T>::operator^=(const Vec& other) -> Vec&
{
	auto temp_x = x;
	auto temp_y = y;

	x = y * other.z - z * other.y;
	y = z * other.x - temp_x * other.z;
	z = temp_x * other.y - temp_y * other.x;

	return *this;
}

}

// math::Vec<D, T> =============================================================


// Static Zero -----------------------------------------------------------------

template <usize D, typename T>
const Vec<D, T> Vec<D, T>::Zero {};


// Static All(T) ---------------------------------------------------------------

template <usize D, typename T>
inline auto Vec<D, T>::all(T value) -> Vec
{
	Vec result;
	for (auto i = 0; i < D; ++i)
		result.components[i] = value;

	return result;
}


// Subscript operator ----------------------------------------------------------

template <usize D, typename T>
inline auto Vec<D, T>::operator[](usize idx) -> T&
{
	if (idx > D) {
		auto err_msg = fmt::format(
			"Index out of range for Vec<{}>: Expected < {}, received {}",
			D, D, idx
		);

		throw std::exception{ err_msg.c_str(), 1 };
	}

	return components[idx];
}


// Unary negation --------------------------------------------------------------

template <usize D, typename T>
inline auto Vec<D, T>::operator-() const -> Vec
{
	auto result = *this;
	for (auto i = 0; i < D; ++i)
		result.components[i] *= -1;

	return result;
}


// Vector addition -------------------------------------------------------------

template <usize D, typename T>
inline auto Vec<D, T>::operator+(const Vec& other) const -> Vec
{
	auto result = *this;
	for (auto i = 0; i < D; ++i)
		result.components[i] += other.components[i];

	return result;
}

template <usize D, typename T>
inline auto Vec<D, T>::operator+=(const Vec& other) -> Vec&
{
	for (auto i = 0; i < D; ++i)
		components[i] += other.components[i];

	return *this;
}


// Vector subtraction ----------------------------------------------------------

template <usize D, typename T>
inline auto Vec<D, T>::operator-(const Vec& other) const -> Vec
{
	auto result = *this;
	for (auto i = 0; i < D; ++i)
		result.components[i] -= other.components[i];

	return result;
}

template <usize D, typename T>
inline auto Vec<D, T>::operator-=(const Vec& other) -> Vec&
{
	for (auto i = 0; i < D; ++i)
		components[i] -= other.components[i];

	return *this;
}


// Scalar multiplication -------------------------------------------------------

template <usize D, typename T>
inline auto Vec<D, T>::operator*(T magnitude) const -> Vec
{
	auto result = *this;
	for (auto i = 0; i < D; ++i)
		result.components[i] *= magnitude;

	return result;
}

template <usize D, typename T>
inline auto Vec<D, T>::operator*=(T magnitude) -> Vec&
{
	for (auto i = 0; i < D; ++i)
		components[i] *= magnitude;

	return *this;
}
} // namespace math

template <sized::usize D, typename T>
inline auto operator*(T lhs, const math::Vec<D, T>& rhs) -> math::Vec<D, T>
{
	return rhs * lhs;
}

namespace math {


// Scalar division -------------------------------------------------------------

template <usize D, typename T>
inline auto Vec<D, T>::operator/(T magnitude) const -> Vec
{
	if (nearly_equal<T>(magnitude, 0))
		return Vec::all(0);

	auto result = *this;
	for (auto i = 0; i < D; ++i)
		result.components[i] /= magnitude;

	return result;
}

template <usize D, typename T>
inline auto Vec<D, T>::operator/=(T magnitude) -> Vec&
{
	if (nearly_equal<T>(magnitude, 0)) {
		for (auto i = 0; i < D; ++i)
			components[i] = 0;

		return *this;
	}

	for (auto i = 0; i < D; ++i)
		components[i] /= magnitude;

	return *this;
}


// Equality comparison ---------------------------------------------------------

template <usize D, typename T>
inline auto Vec<D, T>::operator==(const Vec& other) const -> bool
{
	if (this == &other)
		return true;

	for (auto i = 0; i < D; ++i)
		if (!nearly_equal<T>(components[i], other.components[i]))
			return false;

	return true;
}


// Not-equal comparison --------------------------------------------------------

template <usize D, typename T>
inline auto Vec<D, T>::operator!=(const Vec& other) const -> bool
{
	if (this == &other)
		return false;

	for (auto i = 0; i < D; ++i)
		if (!nearly_equal<T>(components[i], other.components[i]))
			return true;

	return false;
}


// Length / Magnitude ----------------------------------------------------------

template <usize D, typename T>
inline auto Vec<D, T>::length() const -> T
{
	T result = 0;
	for (auto i = 0; i < D; ++i)
		result += components[i] * components[i];

	return std::sqrt(result);
}

template <usize D, typename T>
inline auto Vec<D, T>::magnitude() const -> T
{
	return length();
}


// Unit-Length Direction -------------------------------------------------------

template <usize D, typename T>
inline auto Vec<D, T>::unit() const -> Vec
{
	return *this / length();
}

template <usize D, typename T>
inline auto Vec<D, T>::direction() const -> Vec
{
	return unit();
}

template <usize D, typename T>
inline auto Vec<D, T>::normal() const -> Vec
{
	return unit();
}

template <usize D, typename T>
inline void Vec<D, T>::normalize()
{
	*this /= length();
}


// Distance --------------------------------------------------------------------

template <usize D, typename T>
inline auto Vec<D, T>::dist(const Vec& other) const -> T
{
	T result = 0;
	for (auto i = 0; i < D; ++i) {
		auto diff = other.components[i] - components[i];
		result += diff * diff;
	}

	return std::sqrt(result);
}

template <usize D, typename T>
inline auto Vec<D, T>::dist(const Vec& lhs, const Vec& rhs) -> T
{
	return lhs.dist(rhs);
}


// Dot-product -----------------------------------------------------------------

template <usize D, typename T>
inline auto Vec<D, T>::dot(const Vec& other) const -> T
{
	T result = 0;
	for (auto i = 0; i < D; ++i)
		result += components[i] * other.components[i];

	return result;
}

template <usize D, typename T>
inline auto Vec<D, T>::operator|(const Vec& other) const -> T
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
struct tuple_size<::math::Vec<D, T>> {
	static constexpr size_t value = D;
};

template <size_t Index, size_t D, typename T>
struct tuple_element<Index, ::math::Vec<D, T>> {
	static_assert(Index < D, "Index out of range");
	using type = T;
};

// NOLINTEND(cert-dcl58-cpp)
}
