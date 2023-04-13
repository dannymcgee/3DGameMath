#pragma once

#include "math/matrix.inl.h"

#include <fmt/format.h>
#include <sized.h>

#include "math/assert.h"
#include "math/utility.h"


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

template <usize R, usize C>
constexpr Matrix<R,C>::Matrix(std::initializer_list<Row> rows)
	: Super()
{
	ASSERT(rows.size() > 0 && rows.size() <= R,
		"Too many arguments for Matrix<{},{}>: Expected {}, received {}",
		R, C, R, rows.size());

	usize i = 0;
	for (const auto& row : rows) {
		m_data[i] = row;
		if (++i >= R) break;
	}
}

template <usize R, usize C>
constexpr auto Matrix<R,C>::identity() -> Matrix<R,C>
{
	static_assert(R == C, "Identity matrix must be square");

	Matrix<R,C> result;
	for (usize r = 0; r < R; ++r)
		for (usize c = 0; c < C; ++c)
			result[r][c] = r == c ? 1 : 0;

	return result;
}


// Iterator support ------------------------------------------------------------

template <usize R, usize C>
inline auto Matrix<R,C>::begin() -> detail::RawIterator<flt>
{
	return m_data[0].begin();
}

template <usize R, usize C>
inline auto Matrix<R,C>::begin() const -> detail::RawConstIterator<flt>
{
	return m_data[0].begin();
}

template <usize R, usize C>
inline auto Matrix<R,C>::end() -> detail::RawIterator<flt>
{
	return m_data[R-1].end();
}

template <usize R, usize C>
inline auto Matrix<R,C>::end() const -> detail::RawConstIterator<flt>
{
	return m_data[R-1].end();
}


// Member access ---------------------------------------------------------------

template <usize R, usize C>
template <usize Index2D>
inline auto Matrix<R,C>::m() const -> flt
{
	VALIDATE_INDEX_2D(Index2D, R, C);
	return m_data[EXPAND_INDEX_2D_SUBSCRIPT(Index2D)];
}

template <usize R, usize C>
template <usize Index2D>
inline auto Matrix<R,C>::m() -> flt&
{
	VALIDATE_INDEX_2D(Index2D, R, C);
	return m_data[EXPAND_INDEX_2D_SUBSCRIPT(Index2D)];
}

template <usize R, usize C>
inline auto Matrix<R,C>::m(usize r, usize c) const -> flt
{
	return m_data[r-1][c-1];
}

template <usize R, usize C>
inline auto Matrix<R,C>::m(usize r, usize c) -> flt&
{
	return m_data[r-1][c-1];
}

template <usize R, usize C>
template <usize Index>
inline auto Matrix<R,C>::row() const -> const Row&
{
	static_assert(Index > 0 && Index <= R);
	return m_data[Index - 1];
}

template <usize R, usize C>
template <usize Index>
inline auto Matrix<R,C>::row() -> Row&
{
	static_assert(Index > 0 && Index <= R);
	return m_data[Index - 1];
}

template <usize R, usize C>
template <usize Index>
inline auto Matrix<R,C>::col() const -> Col
{
	static_assert(Index > 0 && Index <= C);

	Col result;
	for (usize r = 0; r < R; ++r)
		result[r] = m_data[r][Index-1];

	return result;
}

template <usize R, usize C>
inline auto Matrix<R,C>::row(usize idx) const -> const Row&
{
	return m_data[idx-1];
}

template <usize R, usize C>
inline auto Matrix<R,C>::row(usize idx) -> Row&
{
	return m_data[idx-1];
}

template <usize R, usize C>
inline auto Matrix<R,C>::col(usize idx) const -> Col
{
	Col result;
	for (usize r = 0; r < R; ++r)
		result[r] = m_data[r][idx-1];

	return result;
}

template <usize R, usize C>
inline auto Matrix<R,C>::operator[](usize idx) -> Row&
{
	return m_data[idx];
}

template <usize R, usize C>
inline auto Matrix<R,C>::operator[](usize idx) const -> const Row&
{
	return m_data[idx];
}


// Matrix transposition --------------------------------------------------------

template <usize R, usize C>
inline auto Matrix<R,C>::transpose() const -> Matrix<C,R>
{
	Matrix<C,R> result;
	for (usize r = 0; r < C; ++r)
		for (usize c = 0; c < R; ++c)
			result[r][c] = m_data[c][r];

	return result;
}


// Scalar multiplication -------------------------------------------------------

template <usize R, usize C>
constexpr auto Matrix<R,C>::operator*(flt value) const -> Matrix
{
	auto result = *this;
	for (usize r = 0; r < R; ++r)
		for (usize c = 0; c < C; ++c)
			result.m_data[r][c] *= value;

	return result;
}

template <usize R, usize C>
inline auto Matrix<R,C>::operator*=(flt value) -> Matrix&
{
	for (usize r = 0; r < R; ++r)
		for (usize c = 0; c < C; ++c)
			m_data[r][c] *= value;

	return *this;
}
} // namespace math

template <sized::usize R, sized::usize C>
constexpr auto operator*(sized::flt lhs, const math::Matrix<R,C>& rhs) -> math::Matrix<R,C>
{
	return rhs * lhs;
}


// Matrix multiplication -------------------------------------------------------

template <sized::usize R, sized::usize N, sized::usize C>
inline auto operator*(
	const math::Matrix<R,N>& lhs,
	const math::Matrix<N,C>& rhs
) -> math::Matrix<R,C>
{
	math::Matrix<R,C> result;
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
template <sized::usize R, sized::usize C>
inline auto operator*(
	const math::Vector<R>& lhs,
	//    math::Matrix<1,R,T>& lhs,
	const math::Matrix<R,C>& rhs
) -> math::Vector<C>
{
	math::Vector<C> result;
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
template <sized::usize R, sized::usize C>
inline auto operator*(
	const math::Matrix<R,C>& lhs,
	//    math::Matrix<C,1,T>& rhs,
	const math::Vector<C>& rhs
) -> math::Vector<R>
{
	math::Vector<R> result;

	for (sized::usize r = 0; r < R; ++r)
		result[r] = lhs[r] | rhs;

	return result;
}


namespace math {

// Determinant -----------------------------------------------------------------

// TODO: Why can't I partially specialize for `<typename T> Mat<2,2,T>`, etc.??
template <>
inline auto Matrix<2,2>::determinant() const -> flt
{
	return m11 * m22 - m12 * m21;
}

template <>
inline auto Matrix<3,3>::determinant() const -> flt
{
	return (row<1>() ^ row<2>()) | row<3>();
}

template <usize R, usize C>
inline auto Matrix<R,C>::determinant() const -> flt
{
	static_assert(R == C, "Determinant can only be calculated for square matrices");

	flt result = 0;

	constexpr usize r = 1;
	for (usize c = 1; c <= C; ++c)
		result += m(r,c) * cofactor(r,c);

	return result;
}


template <usize R, usize C>
inline auto Matrix<R,C>::cofactor(usize r, usize c) const -> flt
{
	flt factor = ((r + c) % 2) ? -1 : 1;
	return minor(r, c) * factor;
}


template <usize R, usize C>
inline auto Matrix<R,C>::minor(const usize row, const usize col) const -> flt
{
	Matrix<R-1, C-1> submat;

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


// Inversion -------------------------------------------------------------------

template <>
inline auto Matrix<2,2>::inverse(flt determinant) const -> Matrix
{
	return (1 / determinant) * Matrix{
		{  m22, -m12 },
		{ -m21,  m11 },
	};
}

template <>
inline auto Matrix<2,2>::inverse() const -> std::optional<Matrix>
{
	flt determinant;
	if (!is_invertible(determinant))
		return {};

	return inverse(determinant);
}

template <>
inline auto Matrix<3,3>::inverse() const -> std::optional<Matrix>
{
	flt determinant;
	if (!is_invertible(determinant))
		return {};

	return inverse(determinant);
}

template <usize R, usize C>
inline auto Matrix<R,C>::inverse() const -> std::optional<Matrix<C,R>>
{
	{
		Matrix<C,R> transposed;
		if (is_orthogonal(transposed))
			return transposed;
	}

	flt determinant;
	if (!is_invertible(determinant))
		return {};

	return inverse(determinant);
}

template <usize R, usize C>
inline auto Matrix<R,C>::inverse(flt determinant) const -> Matrix<C,R>
{
	ASSERT(!math::nearly_equal(determinant, 0),
		"Cannot invert a matrix whose determinant is zero {}", ' ');

	return (1 / determinant) * adjoint();
}

template <usize R, usize C>
inline auto Matrix<R,C>::adjoint() const -> Matrix<C,R>
{
	Matrix<C,R> result;

	for (usize r = 1; r <= R; ++r)
		for (usize c = 1; c <= C; ++c)
			result.m(r,c) = cofactor(c,r);

	return result;
}


// Orthogonalize ---------------------------------------------------------------

template <>
inline void Matrix<3,3>::orthogonalize()
{
	row<1>().normalize();

	row<2>() -= (row<2>() | row<1>()) / (row<1>() | row<1>()) * row<1>();
	row<2>().normalize();

	row<3>() = (row<1>() ^ row<2>());
}

// ReSharper disable once CppMemberFunctionMayBeStatic
template <usize R, usize C>
inline void Matrix<R,C>::orthogonalize()
{
	static_assert(R == 3 && C == 3,
		"Orthogonalization is only supported for 3x3 square matrices");
}


// Queries ---------------------------------------------------------------------

template <usize R, usize C>
inline auto Matrix<R,C>::is_invertible(flt& out_determinant) const -> bool
{
	out_determinant = determinant();
	return !math::nearly_equal(out_determinant, 0);
}

template <usize R, usize C>
inline auto Matrix<R,C>::is_orthogonal(Matrix<C,R>& out_transposed, flt tolerance) const -> bool
{
	out_transposed = transpose();
	return ((*this) * out_transposed).is_identity(tolerance);
}

template <usize R, usize C>
inline auto Matrix<R,C>::is_orthogonal(flt tolerance) const -> bool
{
	// TODO: Multiplying by the transpose is computationally redundant, since
	// the * operator transposes the RHS to dot the LHS rows by its columns.
	// Maybe add a special-case function that treats the RHS as already
	// transposed?
	return ((*this) * transpose()).is_identity(tolerance);
}

template <usize R, usize C>
constexpr auto Matrix<R,C>::is_identity(flt tolerance) const -> bool
{
	for (usize r = 0; r < R; ++r)
		for (usize c = 0; c < C; ++c)
			if (r != c && !math::nearly_equal(m_data[r][c], 0, tolerance)
				|| r == c && !math::nearly_equal(m_data[r][c], 1, tolerance))

				return false;

	return true;
}


// Misc / Utility --------------------------------------------------------------

template <usize R, usize C>
auto Matrix<R,C>::to_string(usize precision) const -> std::string
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
