#pragma once

#include "math/_polar_decl.h"

#include <cmath>

#include <fmt/format.h>

#include "math/literals.h"
#include "math/utility.h"


namespace math {

// PolarCoords =================================================================

template <typename T>
constexpr auto PolarCoords<T>::from_cartesian(const Vector<2,T>& coords) -> PolarCoords
{
	auto [x,y] = coords;
	return PolarCoords::from_cartesian(x, y);
}

template <typename T>
constexpr auto PolarCoords<T>::from_cartesian(T x, T y) -> PolarCoords
{
	if (math::nearly_equal<T>(x, 0) && math::nearly_equal<T>(y, 0))
		return { 0, 0 };

	T radius = std::sqrt(x * x + y * y);
	T angle = std::atan2(y, x);

	return { radius, angle };
}

template <typename T>
constexpr auto PolarCoords<T>::canonical() const -> PolarCoords
{
	using namespace math::literals; // NOLINT

	if (math::nearly_equal<T>(radius, 0))
		return { 0, 0 };

	T r = radius;
	T theta = angle;

	if (r < 0) {
		r = -r;
		theta += 180_deg;
	}

	if (std::abs(theta) > 180_deg) {
		// Offset
		theta += 180_deg;
		// Wrap in range [0, 360°]
		theta -= std::floor(theta / 360_deg) * 360_deg;
		// Revert the offset
		theta -= 180_deg;
	}

	return { r, theta };
}

template <typename T>
inline void PolarCoords<T>::canonicalize()
{
	*this = canonical();
}

template <typename T>
auto PolarCoords<T>::to_string(usize precision) const -> std::string
{
	return ::fmt::format(
		"({0:.{2}}, {1:.{2}}°)",
		radius, math::rad2deg(angle), precision);
}


// SphericalCoords =============================================================

template <typename T>
constexpr auto SphericalCoords<T>::from_cartesian(const Vector<3,T>& coords) -> SphericalCoords
{
	auto [x,y,z] = coords;
	return SphericalCoords::from_cartesian(x, y, z);
}

template <typename T>
constexpr auto SphericalCoords<T>::from_cartesian(T x, T y, T z) -> SphericalCoords
{
	using namespace math::literals; // NOLINT

	T radius = std::sqrt(x * x + y * y + z * z);

	if (math::nearly_equal<T>(radius, 0))
		return { 0, 0, 0 };

	T declination = std::asin(-y / radius);
	T heading = math::nearly_equal<T>(declination, 90_deg, 0.0001)
		? 0
		: std::atan2(x, z);

	return {
		radius,
		heading,
		declination,
	};
}

template <typename T>
constexpr auto SphericalCoords<T>::canonical() const -> SphericalCoords
{
	using namespace math::literals; // NOLINT

	if (math::nearly_equal<T>(radius, 0))
		return { 0, 0, 0 };

	T r = radius;
	T h = heading;
	T p = declination;

	// Clamp radius to positive
	if (r < 0) {
		r = -r;
		h += 180_deg;
		p = -p;
	}

	// Clamp pitch to [-90°, 90°]
	if (std::abs(p) > 90_deg) {
		// Offset
		p += 90_deg;
		// Wrap in range [0, 360°]
		p -= std::floor(p / 360_deg) * 360_deg;

		// (checking against 180° to include the 90° offset from earlier)
		if (p > 180_deg) {
			// Flip the heading
			h += 180_deg;
			// Undo offset and invert
			p = 270_deg - p;
		}
		else {
			// Just revert the offset
			p -= 90_deg;
		}
	}

	// Check for gimbal lock
	if (math::nearly_equal<T>(p, 90_deg, 0.0001))
		return { r, 0, p };

	// Wrap heading, avoiding math when possible to preserve precision
	if (std::abs(h) > 180_deg) {
		// Offset
		h += 180_deg;
		// Wrap in range [0, 360°]
		h -= std::floor(h / 360_deg) * 360_deg;
		// Undo offset
		h -= 180_deg;
	}

	return { r, h, p };
}

template <typename T>
inline void SphericalCoords<T>::canonicalize()
{
	*this = canonical();
}

template <typename T>
auto SphericalCoords<T>::to_string(usize precision) const -> std::string
{
	return ::fmt::format(
		"(r {0:.{3}}, h {1:.{3}}°, p {2:.{3}}°)",
		radius, math::rad2deg(heading), math::rad2deg(declination), precision);
}

} // namespace math
