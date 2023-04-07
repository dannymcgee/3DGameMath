#pragma once

#include <string>

#include <sized.h>

#include "math/fmt.h"
#include "math/utility.h"


namespace math {
using namespace sized; // NOLINT(*-using-namespace)

template <typename T> struct PolarCoords;
template <typename T> struct SphericalCoords;

namespace detail {

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
} // namespace detail


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

	/** Convert polar to cartesian coordinates. */
	static constexpr auto from_polar(T radius, T angle) -> Vector;
	/** Convert polar to cartesian coordinates. */
	static constexpr auto from_polar(const PolarCoords<T>& coords) -> Vector;
	/** Convert polar to cartesian coordinates. */
	static constexpr auto from_polar(T radius, T heading, T declination) -> Vector;
	/** Convert polar to cartesian coordinates. */
	static constexpr auto from_polar(const SphericalCoords<T>& coords) -> Vector;

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

} // namespace math
