#pragma once

#include <sized.h>

#include "math/utility.h"

namespace math {
using namespace sized; // NOLINT

namespace literals {
	constexpr auto operator""_deg(long double value) -> f64
	{
		return math::deg2rad(static_cast<f64>(value));
	}

	constexpr auto operator""_deg(usize value) -> f64
	{
		return math::deg2rad(static_cast<f64>(value));
	}

	constexpr auto operator""_rad(long double value) -> f64
	{
		return static_cast<f64>(value);
	}

	constexpr auto operator""_rad(usize value) -> f64
	{
		return static_cast<f64>(value);
	}
} // namespace literals
} // namespace math
