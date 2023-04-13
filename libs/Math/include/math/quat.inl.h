#pragma once

#include <string>

#include <sized.h>
#include <tuple>

#include "math/fmt.h"
#include "math/spaces.h"
#include "math/vector.h"

namespace math {
using namespace sized; // NOLINT

class RotationMatrix;
struct Euler;


struct Quat {
	flt w;
	union { // NOLINT(*-member-init)
		Vector<3> vector;
		struct { flt x, y, z; };
	};

	Quat();
	constexpr Quat(flt w, flt x, flt y, flt z);
	constexpr Quat(flt w, const Vector<3>& xyz);

	static constexpr auto identity() -> Quat;
	static constexpr auto angle_axis(flt angle, const Vector<3>& unit_axis) -> Quat;

	// Conversion
	/** Extract the angle and axis of rotation */
	auto angle_axis() const -> std::tuple<flt, Vector<3>>;
	/** Convert to Euler angles */
	auto euler(Space space = Space::Local2Parent) const -> Euler;
	/** Convert to a RotationMatrix */
	auto matrix(Space space = Space::Local2Parent) const -> RotationMatrix;

	// Magnitude
	auto magnitude() const -> flt;
	auto sq_magnitude() const -> flt;

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
	constexpr auto operator*(flt scale) const -> Quat;
	constexpr auto operator*=(flt scale) -> Quat&;

	// Addition
	constexpr auto operator+(const Quat& rhs) const -> Quat;

	// Difference
	constexpr auto diff(const Quat& rhs) const -> Quat;
	constexpr auto operator-(const Quat& rhs) const -> Quat;

	// Dot product
	constexpr auto dot(const Quat& rhs) const -> flt;
	constexpr auto operator|(const Quat& rhs) const -> flt;

	// Exponentiation
	constexpr auto pow(flt exp) const -> Quat;

	// Rotation
	constexpr auto rotate_point(const Vector<3>& point) const -> Vector<3>;

	// Spherical interpolation
	static auto slerp(const Quat& src, const Quat& dest, flt t) -> Quat;
	auto slerp(const Quat& dest, flt t) const -> Quat;

	// Misc / Utility
	auto to_string(usize precision = 3) const -> std::string;
	auto to_string(const fmt::AlignedValues& formatter) const -> std::string;

	// Iterator support
	auto begin() -> detail::RawIterator<flt>;
	auto begin() const -> detail::RawConstIterator<flt>;

	auto end() -> detail::RawIterator<flt>;
	auto end() const -> detail::RawConstIterator<flt>;

	// Subscript operator
	auto operator[](usize idx) -> flt&;
	auto operator[](usize idx) const -> flt;

	// Structured binding support
	template <usize Index> auto get() &       { return get_helper<Index>(*this); }
	template <usize Index> auto get() const&  { return get_helper<Index>(*this); }
	template <usize Index> auto get() &&      { return get_helper<Index>(*this); }
	template <usize Index> auto get() const&& { return get_helper<Index>(*this); }
private:
	template <usize Index, typename U> auto get_helper(U&& self) -> auto&&;

	friend class RotationMatrix;
	friend struct Euler;
};

} // namespace math

// Scalar multiplication with scalar on lhs
constexpr auto operator*(sized::flt lhs, const math::Quat& rhs) -> math::Quat;
