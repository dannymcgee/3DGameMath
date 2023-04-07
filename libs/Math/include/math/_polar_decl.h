#pragma once

#include <string>

#include <sized.h>


namespace math {
using namespace sized; // NOLINT

template <usize D, typename T>
struct Vector;

template <typename T = f64>
struct PolarCoords {
	T radius;
	T angle;

	static constexpr auto from_cartesian(const Vector<2,T>& coords) -> PolarCoords;
	static constexpr auto from_cartesian(T x, T y) -> PolarCoords;

	constexpr auto canonical() const -> PolarCoords;
	void canonicalize();

	auto to_string(usize precision = 3) const -> std::string;
};

template <typename T = f64>
struct SphericalCoords {
	T radius;
	T heading;
	T declination;

	static constexpr auto from_cartesian(const Vector<3,T>& coords) -> SphericalCoords;
	static constexpr auto from_cartesian(T x, T y, T z) -> SphericalCoords;

	constexpr auto canonical() const -> SphericalCoords;
	void canonicalize();

	auto to_string(usize precision = 3) const -> std::string;
};

} // namespace math
