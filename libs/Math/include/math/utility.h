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

constexpr f64 pi = 3.141592653589793;

/**
 * Compare the equality of floating-point values within a specified tolerance.
 * If no tolerance is provided, the machine epsilon is used.
 */
template <typename T = f64>
constexpr auto nearly_equal(T lhs, T rhs, T tolerance = std::numeric_limits<T>::epsilon())
{
	return std::abs(lhs - rhs) < tolerance;
}

/** Convert degrees to radians. */
template <typename T = f64>
constexpr auto deg2rad(T deg) -> T
{
	return deg * static_cast<T>(math::pi) / static_cast<T>(180);
}

/** Convert radians to degrees. */
template <typename T = f64>
constexpr auto rad2deg(T rad) -> T
{
	return rad * static_cast<T>(180) / static_cast<T>(math::pi);
}

/** Get the number of non-fractional digits in a number. */
template <typename T>
constexpr auto num_digits(T value) -> usize // NOLINT(*-cognitive-complexity)
{
	usize count = 1;
	for (usize order = 10, n = std::abs(value);
		n >= order && count < 20;
		order *= 10, ++count);

	return count;
}

template <typename T>
constexpr auto num_decimal_places(T value, usize max = 20) -> usize
{
	if (std::is_integral<T>::value)
		return 0;

	usize count = 0;
	for (T n = std::abs(value);
		static_cast<i64>(n) != n && count < max;
		n *= 10.0, ++count);

	return count;
}

template <typename T>
constexpr auto wrap_pi(T angle) -> T
{
	if (std::abs(angle) > math::pi)
		return angle;

	constexpr T two_pi = 2 * pi;
	T revs = std::floor((angle + math::pi) * (1 / two_pi));

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
