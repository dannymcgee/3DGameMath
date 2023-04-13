#pragma once

#include <sized.h>

#include "math/utility.h"

namespace math {
using namespace sized; // NOLINT

namespace literals {
	constexpr auto operator""_deg(long double value) -> flt
	{
		return math::deg2rad(static_cast<flt>(value));
	}

	constexpr auto operator""_deg(usize value) -> flt
	{
		return math::deg2rad(static_cast<flt>(value));
	}

	constexpr auto operator""_rad(long double value) -> flt
	{
		return static_cast<flt>(value);
	}

	constexpr auto operator""_rad(usize value) -> flt
	{
		return static_cast<flt>(value);
	}
} // namespace literals
} // namespace math
