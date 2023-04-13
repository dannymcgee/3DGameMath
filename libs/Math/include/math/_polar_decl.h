#pragma once

#include <string>

#include <sized.h>


namespace math {
using namespace sized; // NOLINT

template <usize D> struct Vector;


struct PolarCoords {
	flt radius;
	flt angle;

	static constexpr auto from_cartesian(const Vector<2>& coords) -> PolarCoords;
	static constexpr auto from_cartesian(flt x, flt y) -> PolarCoords;

	constexpr auto canonical() const -> PolarCoords;
	void canonicalize();

	auto to_string(usize precision = 3) const -> std::string;
};


struct SphericalCoords {
	flt radius;
	flt heading;
	flt pitch;

	static constexpr auto from_cartesian(const Vector<3>& coords) -> SphericalCoords;
	static constexpr auto from_cartesian(flt x, flt y, flt z) -> SphericalCoords;

	constexpr auto canonical() const -> SphericalCoords;
	void canonicalize();

	auto to_string(usize precision = 3) const -> std::string;
};

} // namespace math
