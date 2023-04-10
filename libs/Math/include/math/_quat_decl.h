#pragma once

#include <string>

#include <sized.h>
#include <tuple>

#include "math/fmt.h"
#include "math/spaces.h"
#include "math/vector.h"

namespace math {
using namespace sized; // NOLINT

template <typename T> class RotationMatrix;
template <typename T> struct Euler;


template <typename T = f64>
struct Quat {
	T w;
	union {
		Vector<3,T> vector;
		struct { T x, y, z; };
	};

	constexpr Quat();
	constexpr Quat(T w, T x, T y, T z);
	constexpr Quat(T w, const Vector<3,T>& xyz);

	static constexpr auto identity() -> Quat;
	static constexpr auto angle_axis(T angle, const Vector<3,T>& unit_axis) -> Quat;

	// Conversion
	/** Extract the angle and axis of rotation */
	auto angle_axis() const -> std::tuple<T, Vector<3,T>>;
	/** Convert to Euler angles */
	auto euler(Space space = Space::Local2Parent) const -> Euler<T>;
	/** Convert to a RotationMatrix */
	auto matrix(Space space = Space::Local2Parent) const -> RotationMatrix<T>;

	// Magnitude
	auto magnitude() const -> T;
	auto sq_magnitude() const -> T;

	// Normalize
	void normalize();

	// Conjugate & Inverse
	constexpr auto conjugate() const -> Quat;
	constexpr auto inverse() const -> Quat;

	// Unary negation
	auto operator-() const -> Quat;

	// Quaternion multiplication
	constexpr auto operator*(const Quat& other) const -> Quat;

	// Scalar multiplication
	constexpr auto operator*(T scale) const -> Quat;
	constexpr auto operator*=(T scale) -> Quat&;

	// Addition
	constexpr auto operator+(const Quat& rhs) const -> Quat;

	// Difference
	constexpr auto diff(const Quat& rhs) const -> Quat;
	constexpr auto operator-(const Quat& rhs) const -> Quat;

	// Dot product
	constexpr auto dot(const Quat& rhs) const -> T;
	constexpr auto operator|(const Quat& rhs) const -> T;

	// Exponentiation
	constexpr auto pow(T exp) const -> Quat;

	// Rotation
	constexpr auto rotate_point(const Vector<3,T>& point) const -> Vector<3,T>;

	// Spherical interpolation
	static auto slerp(const Quat<T>& src, const Quat<T>& dest, T t) -> Quat;
	auto slerp(const Quat<T>& dest, T t) const -> Quat;

	// Misc / Utility
	auto to_string(usize precision = 3) const -> std::string;
	auto to_string(const fmt::AlignedValues& formatter) const -> std::string;

	// Iterator support
	auto begin() -> detail::RawIterator<T>;
	auto begin() const -> detail::RawConstIterator<T>;

	auto end() -> detail::RawIterator<T>;
	auto end() const -> detail::RawConstIterator<T>;

	// Subscript operator
	auto operator[](usize idx) -> T&;
	auto operator[](usize idx) const -> T;

	// Structured binding support
	template <usize Index> auto get() &       { return get_helper<Index>(*this); }
	template <usize Index> auto get() const&  { return get_helper<Index>(*this); }
	template <usize Index> auto get() &&      { return get_helper<Index>(*this); }
	template <usize Index> auto get() const&& { return get_helper<Index>(*this); }
private:
	template <usize Index, typename U> auto get_helper(U&& self) -> auto&&;

	friend class RotationMatrix<T>;
	friend struct Euler<T>;
};

} // namespace math

// Scalar multiplication with scalar on lhs
template <typename T = sized::f64>
constexpr auto operator*(T lhs, const math::Quat<T>& rhs) -> math::Quat<T>;
