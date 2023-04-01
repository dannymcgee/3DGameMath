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

struct AlignedValues {
	std::string fmt_string;
	usize width = 0;
	usize precision = 0;
	f64 tolerance = std::numeric_limits<f64>::epsilon();

	AlignedValues() = default;

	AlignedValues(std::string fmt_string, usize width, usize precision)
		: fmt_string(std::move(fmt_string))
		, width(width)
		, precision(precision)
		, tolerance(1.0 / std::pow(10.0, precision))
	{}

	template <typename Iter>
	AlignedValues(Iter begin, Iter end, usize prec);

	template <typename T>
	auto format(T n) const -> std::string;
};


template <typename Iter>
AlignedValues::AlignedValues(Iter begin, Iter end, usize prec)
	: precision(prec)
	, tolerance(1.0 / std::pow(10.0, prec))
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

		f64 base = std::pow(10.0, num_digits(n));
		f64 remainder = std::modf(std::abs(n), &base);

		if (remainder > 0) {
			fixed_point = true;
			precision = std::min(precision, num_decimal_places(remainder, precision));
		}
	});

	// Determine a consistent print-width for each value
	width = num_digits(largest_abs);
	if (needs_sign) width += 1;
	if (fixed_point) width += precision + 1;

	// Build the format string
	fmt_string
		= std::string(needs_sign
			? "{0:> {1}.{2}"
			: "{0:>{1}.{2}")
		+ (fixed_point
			? "f}"
			: "}");
}

template <typename T>
auto AlignedValues::format(T n) const -> std::string
{
	if (nearly_equal<T>(n, 0, tolerance))
		n = 0;

	return ::fmt::format(fmt_string, n, width, precision);
}

}
