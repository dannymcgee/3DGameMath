#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <string>
#include <utility>

#include <fmt/format.h>
#include <sized.h>

#include "math/utility.h"

namespace math::fmt {
using namespace sized; // NOLINT(*-using-namespace)

/**
 * A formatter for printing rows, columns, or grids of numeric values such that
 * they're all aligned and formatted consistently with one another.
 */
class AlignedValues {
public:
	/**
	 * Configure a formatter for printing the provided set of numeric values, by
	 * taking into account:
	 *
	 * - The max number of non-fractional digits
	 * - Whether any values are negative
	 * - Whether any values are fractional
	 * - The max number of digits after the decimal point (if applicable)
	 *
	 * @param begin The range of values to consider
	 * @param end The range of values to consider
	 * @param precision The maximum number of decimal places to display
	 */
	template <typename Iter>
	AlignedValues(Iter begin, Iter end, usize precision);

	/** Format a value to a `std::string`. */
	template <typename T>
	inline auto format(T value) const -> std::string;

	/** Print a value to `stdout`. */
	template <typename T>
	inline void print(T value) const;

	inline void debug() const
	{
		::fmt::print("AlignedValues{{\n");
		::fmt::print("   .m_fmt_string = \"{}\",\n", m_fmt_string);
		::fmt::print("   .m_width = {},\n", m_width);
		::fmt::print("   .m_precision = {},\n", m_precision);
		::fmt::print("   .m_tolerance = {},\n", m_tolerance);
		::fmt::print("}}\n\n");
	}

private:
	std::string m_fmt_string;
	usize m_width = 0;
	usize m_precision = 0;
	f64 m_tolerance = std::numeric_limits<f64>::epsilon();
};


template <typename Iter>
AlignedValues::AlignedValues(Iter begin, Iter end, usize precision)
	: m_tolerance(std::pow(0.1, precision))
{
	// Iterate through all values to determine:
	// - The max number of non-fractional digits
	// - Whether any values are negative
	// - Whether any values are fractional
	// - The max number of digits after the decimal point (if applicable)
	f64 largest_abs = 0;
	bool needs_sign = false;
	bool fixed_point = false;

	std::for_each(begin, end, [&](auto n) {
		if (n < 0) needs_sign = true;

		largest_abs = std::max(largest_abs, std::abs(n));

		f64 base;
		f64 remainder = std::modf(std::abs(n), &base);

		if (remainder > 0) {
			fixed_point = true;

			m_precision = (m_precision == 0)
				? math::num_decimal_places(remainder, precision)
				: std::max(m_precision, math::num_decimal_places(remainder, precision));
		}
	});

	// Determine a consistent print-width for each value
	m_width = math::num_digits(largest_abs);
	if (needs_sign) m_width += 1;
	if (fixed_point) m_width += m_precision + 1;

	// Build the format string
	m_fmt_string = needs_sign
		? "{0:> {1}.{2}f}"
		: "{0:>{1}.{2}f}";
}

template <typename T>
inline auto AlignedValues::format(T value) const -> std::string
{
	if (math::nearly_equal<T>(value, 0, m_tolerance))
		value = 0;

	return ::fmt::format(m_fmt_string, value, m_width, m_precision);
}

template <typename T>
inline void AlignedValues::print(T value) const
{
	if (math::nearly_equal<T>(value, 0, m_tolerance))
		value = 0;

	::fmt::print(m_fmt_string, value, m_width, m_precision);
}

}
