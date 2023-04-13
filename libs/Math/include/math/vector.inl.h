#pragma once

#include <string>

#include <sized.h>

#include "math/fmt.h"
#include "math/utility.h"


namespace math {
using namespace sized; // NOLINT(*-using-namespace)

struct PolarCoords;
struct SphericalCoords;

namespace detail {

/**
 * Helper base struct to allow for specializing struct members for specific
 * template instances without needing to redeclare all methods for every
 * specialization.
 */
template <usize D>
struct Vector {
// NOLINTBEGIN(*-pro-type-member-init, *-avoid-c-arrays)
	union {
		flt components[D];
	};
};

template <>
struct Vector<2> {
	union {
		flt components[2] { 0, 0 };
		struct { flt x, y; };
	};
};

template <>
struct Vector<3> {
	union {
		flt components[3] { 0, 0, 0 };
		struct { flt x, y, z; };
	};
};

template <>
struct Vector<4> {
	union {
		flt components[4] { 0, 0, 0, 0 };
		struct { flt x, y, z, w; };
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
template <usize D>
struct Vector : public detail::Vector<D> {
private:
	using detail::Vector<D>::components;

public:
	static const Vector Zero;

	/** Create a vector where all components have the same value. */
	static constexpr auto all(flt value) -> Vector;

	static constexpr auto unit_x() -> Vector;
	static constexpr auto unit_y() -> Vector;
	static constexpr auto unit_z() -> Vector;
	static constexpr auto unit_w() -> Vector;

	static constexpr auto up() -> Vector;
	static constexpr auto right() -> Vector;
	static constexpr auto forward() -> Vector;

	/** Convert polar to cartesian coordinates. */
	static constexpr auto from_polar(flt radius, flt angle) -> Vector;
	/** Convert polar to cartesian coordinates. */
	static constexpr auto from_polar(const PolarCoords& coords) -> Vector;
	/** Convert polar to cartesian coordinates. */
	static constexpr auto from_polar(flt radius, flt heading, flt pitch) -> Vector;
	/** Convert polar to cartesian coordinates. */
	static constexpr auto from_polar(const SphericalCoords& coords) -> Vector;

	// Structured binding support
	template <usize Index> inline auto get() &       { return components[Index]; }
	template <usize Index> inline auto get() const&  { return components[Index]; }
	template <usize Index> inline auto get() &&      { return components[Index]; }
	template <usize Index> inline auto get() const&& { return components[Index]; }

	// Iterator support
	auto begin() -> detail::RawIterator<flt>;
	auto begin() const -> detail::RawConstIterator<flt>;

	auto end() -> detail::RawIterator<flt>;
	auto end() const -> detail::RawConstIterator<flt>;

	// Subscript operator
	auto operator[](usize idx) -> flt&;
	auto operator[](usize idx) const -> flt;

	// Unary negation
	auto operator-() const -> Vector;

	// Vector addition
	auto operator+(const Vector& other) const -> Vector;
	auto operator+=(const Vector& other) -> Vector&;

	// Vector subtraction
	auto operator-(const Vector& other) const -> Vector;
	auto operator-=(const Vector& other) -> Vector&;

	// Scalar multiplication
	auto operator*(flt magnitude) const -> Vector;
	auto operator*=(flt magnitude) -> Vector&;

	// Scalar division
	auto operator/(flt magnitude) const -> Vector;
	auto operator/=(flt magnitude) -> Vector&;

	// Equality comparison
	auto operator==(const Vector& other) const -> bool;

	// Not-equal comparison
	auto operator!=(const Vector& other) const -> bool;

	/** Calculate the length (magnitude) of the vector. */
	auto length() const -> flt;
	/** Calculate the length (magnitude) of the vector. */
	auto magnitude() const -> flt;
	/** Sum of the squares of each component. */
	auto sq_length() const -> flt;

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
	auto length_and_direction() const -> std::tuple<flt, Vector>;

	/** Calculate the distance between two points. */
	auto dist(const Vector& other) const -> flt;
	/** Calculate the distance between two points. */
	static auto dist(const Vector& lhs, const Vector& rhs) -> flt;

	/** Calculate the dot-product of two vectors. */
	auto dot(const Vector& other) const -> flt;
	/** Calculate the dot-product of two vectors. */
	auto operator|(const Vector& other) const -> flt;

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

// Structured binding support --------------------------------------------------

namespace std {

// NOLINTBEGIN(cert-dcl58-cpp):

// I believe cert-dcl58-cpp here is a spurious warning due to limitations of the
// clang-tidy parser. This is just a template specialization with a user-defined
// type, which is normally permitted by the linter -- but because our type
// is templated, the parser is unable to correctly identify what we're doing.

template <size_t D>
struct tuple_size<::math::Vector<D>> {
	static constexpr size_t value = D;
};

template <size_t Index, size_t D>
struct tuple_element<Index, ::math::Vector<D>> {
	static_assert(Index < D, "Index out of range");
	using type = sized::flt;
};

// NOLINTEND(cert-dcl58-cpp)
}
