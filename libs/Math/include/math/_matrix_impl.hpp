#pragma once

#include "math/_matrix_decl.h"

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

template <usize R, usize C, typename T>
constexpr Matrix<R,C,T>::Matrix(std::initializer_list<Row> rows)
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
	return m_data[R-1].end();
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::end() const -> detail::RawConstIterator<T>
{
	return m_data[R-1].end();
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
template <usize Index>
inline auto Matrix<R,C,T>::row() const -> const Row&
{
	static_assert(Index > 0 && Index <= R);
	return m_data[Index - 1];
}

template <usize R, usize C, typename T>
template <usize Index>
inline auto Matrix<R,C,T>::row() -> Row&
{
	static_assert(Index > 0 && Index <= R);
	return m_data[Index - 1];
}

template <usize R, usize C, typename T>
template <usize Index>
inline auto Matrix<R,C,T>::col() const -> Col
{
	static_assert(Index > 0 && Index <= C);

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
inline auto Matrix<R,C,T>::row(usize idx) -> Row&
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
constexpr auto Matrix<R,C,T>::operator*(T value) const -> Matrix
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
constexpr auto operator*(T lhs, const math::Matrix<R,C,T>& rhs) -> math::Matrix<R,C,T>
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
	return m11 * m22 - m12 * m21;
}

template <>
inline auto Matrix<2,2,f32>::determinant() const -> f32
{
	return m11 * m22 - m12 * m21;
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


template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::cofactor(usize r, usize c) const -> T
{
	T factor = ((r + c) % 2) ? -1 : 1;
	return minor(r, c) * factor;
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


// Inversion -------------------------------------------------------------------

template <>
inline auto Matrix<2,2,f32>::inverse(f32 determinant) const -> Matrix
{
	return (1 / determinant) * Matrix{
		{  m22, -m12 },
		{ -m21,  m11 },
	};
}

template <>
inline auto Matrix<2,2,f64>::inverse(f64 determinant) const -> Matrix
{
	return (1 / determinant) * Matrix{
		{  m22, -m12 },
		{ -m21,  m11 },
	};
}

template <>
inline auto Matrix<2,2,f32>::inverse() const -> std::optional<Matrix>
{
	f32 determinant;
	if (!is_invertible(determinant))
		return {};

	return inverse(determinant);
}

template <>
inline auto Matrix<2,2,f64>::inverse() const -> std::optional<Matrix>
{
	f64 determinant;
	if (!is_invertible(determinant))
		return {};

	return inverse(determinant);
}

template <>
inline auto Matrix<3,3,f32>::inverse() const -> std::optional<Matrix>
{
	f32 determinant;
	if (!is_invertible(determinant))
		return {};

	return inverse(determinant);
}

template <>
inline auto Matrix<3,3,f64>::inverse() const -> std::optional<Matrix>
{
	f64 determinant;
	if (!is_invertible(determinant))
		return {};

	return inverse(determinant);
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::inverse() const -> std::optional<Matrix<C,R,T>>
{
	{
		Matrix<C,R,T> transposed;
		if (is_orthogonal(transposed))
			return transposed;
	}

	T determinant;
	if (!is_invertible(determinant))
		return {};

	return inverse(determinant);
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::inverse(T determinant) const -> Matrix<C,R,T>
{
	ASSERT(!math::nearly_equal<T>(determinant, 0),
		"Cannot invert a matrix whose determinant is zero {}", ' ');

	return (1 / determinant) * adjoint();
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::adjoint() const -> Matrix<C,R,T>
{
	Matrix<C,R,T> result;

	for (usize r = 1; r <= R; ++r)
		for (usize c = 1; c <= C; ++c)
			result.m(r,c) = cofactor(c,r);

	return result;
}


// Orthogonalize ---------------------------------------------------------------

// FIXME: These methods are not really working as expected
template <>
inline void Matrix<3,3,f64>::orthogonalize()
{
	row<1>().normalize();

	row<2>() -= (row<2>() | row<1>()) / (row<1>() | row<1>()) * row<1>();
	row<2>().normalize();

	row<3>() = (row<1>() ^ row<2>());
}

// FIXME: These methods are not really working as expected
template <>
inline void Matrix<3,3,f32>::orthogonalize()
{
	row<1>().normalize();

	row<2>() -= (row<2>() | row<1>()) / (row<1>() | row<1>()) * row<1>();
	row<2>().normalize();

	row<3>() = (row<1>() ^ row<2>());
}

// ReSharper disable once CppMemberFunctionMayBeStatic
template <usize R, usize C, typename T>
inline void Matrix<R,C,T>::orthogonalize()
{
	static_assert(R == 3 && C == 3,
		"Orthogonalization is only supported for 3x3 square matrices");
}


// Queries ---------------------------------------------------------------------

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::is_invertible(T& out_determinant) const -> bool
{
	out_determinant = determinant();
	return !math::nearly_equal<T>(out_determinant, 0);
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::is_orthogonal(Matrix<C,R,T>& out_transposed, T tolerance) const -> bool
{
	out_transposed = transpose();
	return ((*this) * out_transposed).is_identity(tolerance);
}

template <usize R, usize C, typename T>
inline auto Matrix<R,C,T>::is_orthogonal(T tolerance) const -> bool
{
	// TODO: Multiplying by the transpose is computationally redundant, since
	// the * operator transposes the RHS to dot the LHS rows by its columns.
	// Maybe add a special-case function that treats the RHS as already
	// transposed?
	return ((*this) * transpose()).is_identity(tolerance);
}

template <usize R, usize C, typename T>
constexpr auto Matrix<R,C,T>::is_identity(T tolerance) const -> bool
{
	for (usize r = 0; r < R; ++r)
		for (usize c = 0; c < C; ++c)
			if (r != c && !math::nearly_equal<T>(m_data[r][c], 0, tolerance)
				|| r == c && !math::nearly_equal<T>(m_data[r][c], 1, tolerance))

				return false;

	return true;
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
