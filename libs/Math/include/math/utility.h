#pragma once

#include <cmath>
#include <limits>
#include <type_traits>

#include <sized.h>

namespace math {

using namespace sized; // NOLINT(*-using-namespace)


/**
 * Compare the equality of floating-point values within a specified tolerance.
 * If no tolerance is provided, the machine epsilon is used.
 */
template <typename T = f64>
inline auto nearly_equal(T lhs, T rhs, T tolerance = std::numeric_limits<T>::epsilon())
{
	return std::abs(lhs - rhs) < tolerance;
}


/** Get the number of non-fractional digits in a number. */
template <typename T>
inline constexpr auto num_digits(T value) -> usize // NOLINT(*-cognitive-complexity)
{
	auto n = static_cast<usize>(std::abs(value));
	return n < 10 ? 1 :
		n < 100ull ? 2 :
		n < 1000ull ? 3 :
		n < 10000ull ? 4 :
		n < 100000ull ? 5 :
		n < 1000000ull ? 6 :
		n < 10000000ull ? 7 :
		n < 100000000ull ? 8 :
		n < 1000000000ull ? 9 :
		n < 10000000000ull ? 10 :
		n < 100000000000ull ? 11 :
		n < 1000000000000ull ? 12 :
		n < 10000000000000ull ? 13 :
		n < 100000000000000ull ? 14 :
		n < 1000000000000000ull ? 15 :
		n < 10000000000000000ull ? 16 :
		n < 100000000000000000ull ? 17 :
		n < 1000000000000000000ull ? 18 :
		n < 10000000000000000000ull ? 19
		  : 20;
}

template <typename T>
inline constexpr auto num_decimal_places(T value, usize max = 20) -> usize
{
	if (std::is_integral<T>::value)
		return 0;

	usize count = 0;
	for (auto n = static_cast<f64>(std::abs(value));
		static_cast<int>(n) != n && count <= max;
		n *= 10.0, ++count);

	return count;
}

}
