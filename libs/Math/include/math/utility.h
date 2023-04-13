#pragma once

#include <cmath>
#include <cstddef>
#include <iterator>
#include <limits>
#include <type_traits>

#include <fmt/format.h>

#include <sized.h>

namespace math {

using namespace sized; // NOLINT(*-using-namespace)

constexpr flt pi = 3.141592653589793;

/**
 * Compare the equality of floating-point values within a specified tolerance.
 * If no tolerance is provided, the machine epsilon is used.
 */
constexpr auto nearly_equal(flt lhs, flt rhs, flt tolerance = std::numeric_limits<flt>::epsilon())
{
	return std::abs(lhs - rhs) < tolerance;
}

/** Convert degrees to radians. */
constexpr auto deg2rad(flt deg) -> flt
{
	return deg * math::pi / 180_flt;
}

/** Convert radians to degrees. */
constexpr auto rad2deg(flt rad) -> flt
{
	return rad * 180_flt / math::pi;
}

/** Get the number of non-fractional digits in a number. */
constexpr auto num_digits(flt value) -> usize
{
	usize count = 1;
	for (usize order = 10, n = static_cast<usize>(std::abs(value));
		n >= order && count < 20;
		order *= 10, ++count);

	return count;
}

constexpr auto num_decimal_places(flt value, usize max = 20) -> usize
{
	// if (std::is_integral<flt>::value)
	// 	return 0;

	usize count = 0;
	for (flt n = std::abs(value);
		static_cast<i64>(n) != n && count < max; // NOLINT(*-narrowing-conversions)
		n *= 10.0, ++count);

	return count;
}

constexpr auto wrap_pi(flt angle) -> flt
{
	if (std::abs(angle) > math::pi)
		return angle;

	constexpr flt two_pi = 2_flt * pi;
	flt revs = std::floor((angle + math::pi) * (1_flt / two_pi));

	return angle - (revs * two_pi);
}


namespace detail {

template <typename T>
class RawIterator {
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T*;
	using reference = T&;

private:
	using Self = RawIterator<T>;

public:
	explicit RawIterator(T* ptr) : m_ptr(ptr) {}
	inline auto operator*() -> reference { return *m_ptr; }
	inline auto operator->() -> pointer { return m_ptr; }
	inline auto operator++() -> Self& { ++m_ptr; return *this; }
	inline auto operator++(int) -> Self { auto pre_op = *this; ++m_ptr; return pre_op; }
	inline auto operator==(const Self& other) const -> bool { return m_ptr == other.m_ptr; }
	inline auto operator!=(const Self& other) const -> bool { return m_ptr != other.m_ptr; }

private:
	T* m_ptr = nullptr;
};


template <typename T>
class RawConstIterator {
public:
	using iterator_category = std::input_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = const T*;
	using reference = const T&;

private:
	using Self = RawConstIterator<T>;

public:
	explicit RawConstIterator(const T* ptr) : m_ptr(ptr) {}
	inline auto operator*() const -> reference { return *m_ptr; }
	inline auto operator->() const -> pointer { return m_ptr; }
	inline auto operator++() -> Self& { ++m_ptr; return *this; }
	inline auto operator++(int) -> Self { auto pre_op = *this; ++m_ptr; return pre_op; }
	inline auto operator==(const Self& other) const -> bool { return m_ptr == other.m_ptr; }
	inline auto operator!=(const Self& other) const -> bool { return m_ptr != other.m_ptr; }

private:
	const T* m_ptr = nullptr;
};

} // namespace detail
} // namespace math
