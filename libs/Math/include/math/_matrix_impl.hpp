#include "math/_matrix_decl.h"

#include <sized.h>


#define VALIDATE_INDEX_2D(index, rows, cols) \
	static_assert(((index) % 100) / 10 > 0 && ((index) % 100) / 10 <= (rows)); \
	static_assert((index) % 10 > 0 && (index) % 10 <= (cols))

#define EXPAND_INDEX_2D_SUBSCRIPT(index) \
	((index) % 100) / 10 - 1][((index) % 10) - 1

#define EXPAND_INDEX_2D_COMMA(index) \
	((index) % 100) / 10, ((index) % 10)


namespace math {
using namespace sized; // NOLINT

// Constructors ----------------------------------------------------------------

template <usize R, usize C, typename T>
Matrix<R,C,T>::Matrix(std::initializer_list<Row> rows)
{
	if (rows.size() > R) {
		auto err_msg = ::fmt::format(
			"Too many arguments for Mat<{},{}>: Expected {}, received {}",
			R, C,
			R, rows.size()
		);

		throw std::exception(err_msg.c_str());
	}

	usize i = 0;
	for (auto& row : rows) {
		m_data[i] = std::move(row);
		++i;
	}
}

template <usize R, usize C, typename T>
Matrix<R,C,T>::Matrix(std::array<Row, R> data)
	: m_data(data)
{}

template <usize R, usize C, typename T>
constexpr auto Matrix<R,C,T>::identity() -> Matrix<R,C,T>
{
	static_assert(R == C, "Identity matrix must be square");

	Matrix<R,C,T> result;
	for (usize r = 0; r < R; ++r)
		for (usize c = 0; c < C; ++c)
			result[r][c] = r == c ? 1 : 0;

	return result;
}


// Iterator support ------------------------------------------------------------

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::begin() -> detail::RawIterator<T>
{
	return m_data[0].begin();
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::begin() const -> detail::RawConstIterator<T>
{
	return m_data[0].begin();
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::end() -> detail::RawIterator<T>
{
	return m_data[C-1].end();
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::end() const -> detail::RawConstIterator<T>
{
	return m_data[C-1].end();
}


// Member access ---------------------------------------------------------------

template <usize R, usize C, typename T>
template <usize Index2D>
inline auto Matrix<R,C,T>::m() const -> T
{
	VALIDATE_INDEX_2D(Index2D, R, C);
	return m_data[EXPAND_INDEX_2D_SUBSCRIPT(Index2D)];
}

template <usize R, usize C, typename T>
template <usize Index2D>
inline auto Matrix<R,C,T>::m() -> T&
{
	VALIDATE_INDEX_2D(Index2D, R, C);
	return m_data[EXPAND_INDEX_2D_SUBSCRIPT(Index2D)];
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::m(usize idx_2d) const -> T
{
	return m_data[EXPAND_INDEX_2D_SUBSCRIPT(idx_2d)];
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::m(usize idx_2d) -> T&
{
	return m_data[EXPAND_INDEX_2D_SUBSCRIPT(idx_2d)];
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::m(usize r, usize c) const -> T
{
	return m_data[r-1][c-1];
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::m(usize r, usize c) -> T&
{
	return m_data[r-1][c-1];
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::m_checked(usize idx_2d) const -> T
{
	validate_index_2d(idx_2d);
	return m_data[EXPAND_INDEX_2D_SUBSCRIPT(idx_2d)];
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::m_checked(usize idx_2d) -> T&
{
	validate_index_2d(idx_2d);
	return m_data[EXPAND_INDEX_2D_SUBSCRIPT(idx_2d)];
}

template <usize R, usize C, typename T>
inline void Matrix<R,C,T>::validate_index_2d(usize idx_2d) const
{
	usize r = (idx_2d % 100) / 10;
	usize c = (idx_2d % 10);

	if (r <= 0 || r > R || c <= 0 || c > C) {
		auto err_msg = ::fmt::format(
			"2D Index {0} out of bounds for Matrix<R={1},C={2}>: index expands to "
			"r={3},c={4} -- expected 0<r<={1}, 0<c<={2}",
			idx_2d, R, C, r, c);

		throw std::exception(err_msg.c_str());
	}
}

template <usize R, usize C, typename T>
template <usize Index>
inline auto Matrix<R,C,T>::row() const -> const Row&
{
	static_assert(Index > 0 && Index <= R);
	return m_data[Index - 1];
}

template <usize R, usize C, typename T>
template <usize Index>
inline auto Matrix<R,C,T>::col() const -> Col
{
	Col result;
	for (usize r = 0; r < R; ++r)
		result[r] = m_data[r][Index-1];

	return result;
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::row(usize idx) const -> const Row&
{
	return m_data[idx-1];
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::col(usize idx) const -> Col
{
	Col result;
	for (usize r = 0; r < R; ++r)
		result[r] = m_data[r][idx-1];

	return result;
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::operator[](usize idx) -> Row&
{
	return m_data[idx];
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::operator[](usize idx) const -> const Row&
{
	return m_data[idx];
}


// Matrix transposition --------------------------------------------------------

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::transpose() const -> Matrix<C,R,T>
{
	Matrix<C,R,T> result;
	for (usize r = 0; r < C; ++r)
		for (usize c = 0; c < R; ++c)
			result[r][c] = m_data[c][r];

	return result;
}


// Scalar multiplication -------------------------------------------------------

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::operator*(T value) const -> Matrix
{
	auto result = *this;
	for (usize r = 0; r < R; ++r)
		for (usize c = 0; c < C; ++c)
			result.m_data[r][c] *= value;

	return result;
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::operator*=(T value) -> Matrix&
{
	for (usize r = 0; r < R; ++r)
		for (usize c = 0; c < C; ++c)
			m_data[r][c] *= value;

	return *this;
}
} // namespace math

template <sized::usize R, sized::usize C, typename T>
inline auto operator*(T lhs, const math::Matrix<R,C,T>& rhs) -> math::Matrix<R,C,T>
{
	return rhs * lhs;
}


// Matrix multiplication -------------------------------------------------------

template <sized::usize R, sized::usize N, sized::usize C, typename T>
inline auto operator*(
	const math::Matrix<R,N,T>& lhs,
	const math::Matrix<N,C,T>& rhs
) -> math::Matrix<R,C,T>
{
	math::Matrix<R,C,T> result;
	auto rhs_cols = rhs.transpose();

	for (sized::usize r = 0; r < R; ++r)
		for (sized::usize c = 0; c < C; ++c)
			result[r][c] = lhs[r] | rhs_cols[c];

	return result;
}

/**
 * Multiply a row-vector by a matrix.
 *
 * @param lhs
 * The row-vector, interpreted as a Mat1xN, where N must match the number of
 * rows in `rhs`.
 *
 * @param rhs
 * The matrix, where the number of rows must match the number of elements in
 * `lhs`.
 *
 * @return The result, as a row-vector.
 */
template <sized::usize R, sized::usize C, typename T>
inline auto operator*(
	const math::Vector<R,T>& lhs,
	//    math::Matrix<1,R,T>& lhs,
	const math::Matrix<R,C,T>& rhs
) -> math::Vector<C,T>
{
	math::Vector<C,T> result;
	auto rhs_cols = rhs.transpose();

	for (sized::usize c = 0; c < C; ++c)
		result[c] = lhs | rhs_cols[c];

	return result;
}

/**
 * Multiply a matrix by a column-vector.
 *
 * @param lhs
 * The matrix, where the number of columns must match the number of elements in
 * `rhs`.
 *
 * @param rhs
 * The column-vector, interpreted as a MatNx1, where N must match the number of
 * columns in `lhs`.
 *
 * @return The result, as a column-vector.
 */
template <sized::usize R, sized::usize C, typename T>
inline auto operator*(
	const math::Matrix<R,C,T>& lhs,
	//    math::Matrix<C,1,T>& rhs,
	const math::Vector<C,T>& rhs
) -> math::Vector<R,T>
{
	math::Vector<R,T> result;

	for (sized::usize r = 0; r < R; ++r)
		result[r] = lhs[r] | rhs;

	return result;
}


namespace math {

// Determinant -----------------------------------------------------------------

// TODO: Why can't I partially specialize for `<typename T> Mat<2,2,T>`, etc.??
template <>
inline auto Matrix<2,2,f64>::determinant() const -> f64
{
	return m<11>() * m<22>() - m<12>() * m<21>();
}

template <>
inline auto Matrix<2,2,f32>::determinant() const -> f32
{
	return m<11>() * m<22>() - m<12>() * m<21>();
}

template <>
inline auto Matrix<3,3,f64>::determinant() const -> f64
{
	return (row<1>() ^ row<2>()) | row<3>();
}

template <>
inline auto Matrix<3,3,f32>::determinant() const -> f32
{
	return (row<1>() ^ row<2>()) | row<3>();
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::determinant() const -> T
{
	static_assert(R == C, "Determinant can only be calculated for square matrices");

	T result = 0;

	constexpr usize r = 1;
	for (usize c = 1; c <= C; ++c)
		result += m(r,c) * cofactor(r,c);

	return result;
}


// Minor -----------------------------------------------------------------------

template <usize R, usize C, typename T>
template <usize Index2D>
inline auto Matrix<R,C,T>::minor() const -> T
{
	VALIDATE_INDEX_2D(Index2D, R, C);
	return minor(EXPAND_INDEX_2D_COMMA(Index2D));
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::minor(usize index_2d) const -> T
{
	return minor(EXPAND_INDEX_2D_COMMA(index_2d));
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::minor(const usize row, const usize col) const -> T
{
	Matrix<R-1, C-1, T> submat;

	for (usize r = 1; r <= R; ++r) {
		if (r == row) continue;

		for (usize c = 1; c <= C; ++c) {
			if (c == col) continue;

			usize r_dest = r > row ? r - 1 : r;
			usize c_dest = c > col ? c - 1 : c;

			submat.m(r_dest, c_dest) = m(r, c);
		}
	}

	return submat.determinant();
}


// Cofactor --------------------------------------------------------------------

template <usize R, usize C, typename T>
template <usize Index2D>
inline auto Matrix<R,C,T>::cofactor() const -> T
{
	VALIDATE_INDEX_2D(Index2D, R, C);
	return cofactor(EXPAND_INDEX_2D_COMMA(Index2D));
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::cofactor(usize index_2d) const -> T
{
	return cofactor(EXPAND_INDEX_2D_COMMA(index_2d));
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::cofactor(usize r, usize c) const -> T
{
	T factor = ((r + c) % 2) ? -1 : 1;
	return minor(r, c) * factor;
}


// Misc / Utility --------------------------------------------------------------

template <usize R, usize C, typename T>
auto Matrix<R,C,T>::to_string(usize precision) const -> std::string
{
	auto formatter = fmt::AlignedValues(begin(), end(), precision);

	std::string result;
	for (usize r = 0; r < R; ++r) {
		result += "| ";
		for (usize c = 0; c < C; ++c) {
			result += formatter.format(m_data[r][c]);

			if (c < C - 1)
				result += "  ";
		}
		result += " |\n";
	}

	return result;
}

}


#undef VALIDATE_INDEX_2D
#undef EXPAND_INDEX_2D_SUBSCRIPT
#undef EXPAND_INDEX_2D_COMMA
