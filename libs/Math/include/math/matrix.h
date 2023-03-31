#pragma once

#include <exception>
#include <initializer_list>
#include <string>

#include <fmt/format.h>
#include <sized.h>

#include "math/vector.h"

namespace math {
using namespace sized; // NOLINT(*-using-namespace)


// Declarations ////////////////////////////////////////////////////////////////

template <usize Rows, usize Cols, typename T = f64>
class Mat {
public:
	using Row = Vec<Cols, T>;
	using Col = Vec<Rows, T>;

	// Constructors
	Mat() = default;
	Mat(std::initializer_list<Row> rows);
	explicit Mat(std::array<Row, Rows> data);

	// Member access
	/**
	 * Get the matrix value at the 1-based two-dimensional index indicated by the
	 * template argument.
	 *
	 * @tparam Index2D
	 * The 1-based, 2D index from which to retrieve the value. The first digit
	 * indicates the row, and the second digit indicates the column.
	 *
	 * @code
	 * int main()
	 * {
	 *     math::Mat<4,3> mat {
	 *         {  1,  2,  3 },
	 *         {  4,  5,  6 },
	 *         {  7,  8,  9 },
	 *         { 10, 11, 12 },
	 *     };
	 *
	 *     double m21 = mat.m<21>(); // m21 == 4
	 * }
	 * @endcode
	 */
	template <usize Index2D> inline auto m() const -> T;
	/**
	 * Get a mutable reference to the matrix value at the 1-based two-dimensional
	 * index indicated by the template argument.
	 *
	 * @tparam Index2D
	 * The 1-based, 2D index from which to retrieve the value. The first digit
	 * indicates the row, and the second digit indicates the column.
	 *
	 * @code
	 * int main()
	 * {
	 *     math::Mat<4,3> mat{};
	 *     mat.m<21>() = 4;
	 *
	 *     // mat now contains:
	 *     // { 0, 0, 0 }
	 *     // { 4, 0, 0 }
	 *     // { 0, 0, 0 }
	 *     // { 0, 0, 0 }
	 * }
	 * @endcode
	 */
	template <usize Index2D> inline auto m() -> T&;

	/**
	 * Get the row at the 1-based index indicated by the template argument.
	 * @tparam Index The 1-based index of the desired row.
	 */
	template <usize Index> inline auto row() const -> Row;
	/**
	 * Get the column at the 1-based index indicated by the template argument.
	 * @tparam Index The 1-based index of the desired column.
	 */
	template <usize Index> inline auto col() const -> Col;

	/**
	 * Get the row at the 1-based index indicated by the argument.
	 * @param idx The 1-based index of the desired row.
	 */
	inline auto row(usize idx) const -> Row;
	/**
	 * Get the column at the 1-based index indicated by the argument.
	 * @param idx The 1-based index of the desired column.
	 */
	inline auto col(usize idx) const -> Col;

	/** Get the row at the 0-based index. */
	inline auto operator[](usize idx) -> Row&;
	/** Get the row at the 0-based index. */
	inline auto operator[](usize idx) const -> const Row&;

	// Matrix transposition
	auto transpose() const -> Mat<Cols, Rows, T>;

	// Scalar multiplication
	inline auto operator*(T value) const -> Mat;
	inline auto operator*=(T value) -> Mat&;

	// Misc / Utility
	auto to_string() const -> std::string;

private:
	std::array<Row, Rows> m_data;
};


// Definitions /////////////////////////////////////////////////////////////////

// Constructors ----------------------------------------------------------------

template <usize R, usize C, typename T>
Mat<R,C,T>::Mat(std::initializer_list<Row> rows)
{
	if (rows.size() > R) {
		auto err_msg = fmt::format(
			"Too many arguments for Mat<{},{}>: Expected {}, received {}",
			R, C,
			R, rows.size()
		);

		throw std::exception{ err_msg.c_str(), 1 };
	}

	usize i = 0;
	for (auto row : rows) {
		m_data[i] = row;
		++i;
	}
}

template <usize R, usize C, typename T>
Mat<R,C,T>::Mat(std::array<Row, R> data)
	: m_data(data)
{}


// Member access ---------------------------------------------------------------

#define VALIDATE_INDEX_2D(index, rows, cols) \
static_assert(((index) % 100) / 10 > 0 && ((index) % 100) / 10 <= (rows)); \
static_assert((index) % 10 > 0 && (index) % 10 <= (cols))

#define EXPAND_INDEX_2D(index) \
((index) % 100) / 10 - 1][((index) % 10) - 1

template <usize R, usize C, typename T>
template <usize Index2D>
inline auto Mat<R,C,T>::m() const -> T
{
	VALIDATE_INDEX_2D(Index2D, R, C);
	return m_data[EXPAND_INDEX_2D(Index2D)];
}

template <usize R, usize C, typename T>
template <usize Index2D>
inline auto Mat<R,C,T>::m() -> T&
{
	VALIDATE_INDEX_2D(Index2D, R, C);
	return m_data[EXPAND_INDEX_2D(Index2D)];
}

#undef VALIDATE_INDEX_2D
#undef EXPAND_INDEX_2D

template <usize R, usize C, typename T>
template <usize Index>
inline auto Mat<R,C,T>::row() const -> Row
{
	static_assert(Index > 0 && Index <= R);
	return m_data[Index - 1];
}

template <usize R, usize C, typename T>
template <usize Index>
inline auto Mat<R,C,T>::col() const -> Col
{
	Col result;
	for (usize r = 0; r < R; ++r)
		result[r] = m_data[r][Index-1];

	return result;
}

template <usize R, usize C, typename T>
inline auto Mat<R,C,T>::row(usize idx) const -> Row
{
	return m_data[idx-1];
}

template <usize R, usize C, typename T>
inline auto Mat<R,C,T>::col(usize idx) const -> Col
{
	Col result;
	for (usize r = 0; r < R; ++r)
		result[r] = m_data[r][idx-1];

	return result;
}

template <usize R, usize C, typename T>
inline auto Mat<R,C,T>::operator[](usize idx) -> Row&
{
	return m_data[idx];
}

template <usize R, usize C, typename T>
inline auto Mat<R,C,T>::operator[](usize idx) const -> const Row&
{
	return m_data[idx];
}


// Matrix transposition --------------------------------------------------------

template <usize R, usize C, typename T>
auto Mat<R,C,T>::transpose() const -> Mat<C,R,T>
{
	Mat<C,R,T> result;
	for (usize r = 0; r < C; ++r)
		for (usize c = 0; c < R; ++c)
			result[r][c] = m_data[c][r];

	return result;
}


// Scalar multiplication -------------------------------------------------------

template <usize R, usize C, typename T>
inline auto Mat<R,C,T>::operator*(T value) const -> Mat
{
	auto result = *this;
	for (usize r = 0; r < R; ++r)
		for (usize c = 0; c < C; ++c)
			result.m_data[r][c] *= value;

	return result;
}

template <usize R, usize C, typename T>
inline auto Mat<R,C,T>::operator*=(T value) -> Mat&
{
	for (usize r = 0; r < R; ++r)
		for (usize c = 0; c < C; ++c)
			m_data[r][c] *= value;

	return *this;
}
} // namespace math

template <sized::usize R, sized::usize C, typename T>
inline auto operator*(T lhs, const math::Mat<R,C,T>& rhs) -> math::Mat<R,C,T>
{
	return rhs * lhs;
}


// Matrix multiplication -------------------------------------------------------

template <sized::usize R, sized::usize N, sized::usize C, typename T>
inline auto operator*(
	const math::Mat<R,N,T>& lhs,
	const math::Mat<N,C,T>& rhs
) -> math::Mat<R,C,T>
{
	math::Mat<R,C,T> result;
	auto rhs_cols = rhs.transpose();

	for (sized::usize r = 0; r < R; ++r)
		for (sized::usize c = 0; c < C; ++c)
			result[r][c] = lhs[r] | rhs_cols[c];

	return result;
}

template <sized::usize R, sized::usize C, typename T>
inline auto operator*(
	//    math::Mat<1,R,T>& lhs,
	const math::Vec<R,T>& lhs,
	const math::Mat<R,C,T>& rhs
) -> math::Vec<C,T>
{
	math::Vec<C,T> result;
	auto rhs_cols = rhs.transpose();

	for (sized::usize c = 0; c < C; ++c)
		result[c] = lhs | rhs_cols[c];

	return result;
}

template <sized::usize R, sized::usize C, typename T>
inline auto operator*(
	const math::Mat<R,C,T>& lhs,
	//    math::Mat<C,1,T>& rhs,
	const math::Vec<C,T>& rhs
) -> math::Vec<R,T>
{
	math::Vec<R,T> result;

	for (sized::usize r = 0; r < R; ++r)
		result[r] = lhs[r] | rhs;

	return result;
}


namespace math {

// Misc / Utility --------------------------------------------------------------

template <usize R, usize C, typename T>
auto Mat<R,C,T>::to_string() const -> std::string
{
	std::string result = "{\n";
	for (usize r = 0; r < R; ++r) {
		result += "\t{ ";
		for (usize c = 0; c < C; ++c) {
			result += fmt::format("{}", m_data[r][c]);
			if (c < C-1)
				result += ", ";
		}
		result += " }\n";
	}
	result += "},";

	return result;
}

}
