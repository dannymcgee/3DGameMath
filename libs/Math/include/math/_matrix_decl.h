#pragma once

#include <map>
#include <optional>

#include <sized.h>

#include "math/vector.h"


namespace math {
using namespace sized; // NOLINT(*-using-namespace)


template <usize Rows, usize Cols, typename T = f64>
class Matrix {
public:
	using Row = Vector<Cols, T>;
	using Col = Vector<Rows, T>;

	// Constructors
	Matrix() = default;
	Matrix(std::initializer_list<Row> rows);
	explicit Matrix(std::array<Row, Rows> data);

	static constexpr auto identity() -> Matrix;

	// Iterator support
	auto begin() -> detail::RawIterator<T>;
	auto begin() const -> detail::RawConstIterator<T>;

	auto end() -> detail::RawIterator<T>;
	auto end() const -> detail::RawConstIterator<T>;

	// Member access
	/**
	 * Get the matrix value at the 1-based 2D index indicated by the template
	 * argument.
	 *
	 * @tparam Index2D
	 * The 1-based 2D index from which to retrieve the value. The first digit
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
	template <usize Index2D>
	auto m() const -> T;
	/**
	 * Get a mutable reference to the matrix value at the 1-based 2D index
	 * indicated by the template argument.
	 *
	 * @tparam Index2D
	 * The 1-based 2D index from which to retrieve the value. The first digit
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
	template <usize Index2D>
	auto m() -> T&;

	/**
	 * Access a member by the 1-based row and column indices.
	 * @warning This method is **not bounds-checked**!
	 */
	auto m(usize row, usize col) const -> T;
	/**
	 * Access a member by the 1-based row and column indices.
	 * @warning This method is **not bounds-checked**!
	 */
	auto m(usize row, usize col) -> T&;

	/** Get the row at the 0-based index. */
	auto operator[](usize idx) -> Row&;
	/** Get the row at the 0-based index. */
	auto operator[](usize idx) const -> const Row&;

	/**
	 * Get the row at the 1-based index indicated by the template argument.
	 * @tparam Index The 1-based index of the desired row.
	 */
	template <usize Index>
	auto row() const -> const Row&;
	/**
	 * Get the column at the 1-based index indicated by the template argument.
	 * @tparam Index The 1-based index of the desired column.
	 */
	template <usize Index>
	auto col() const -> Col;

	/**
	 * Get the row at the 1-based index indicated by the argument.
	 * @param idx The 1-based index of the desired row.
	 */
	auto row(usize idx) const -> const Row&;
	/**
	 * Get the column at the 1-based index indicated by the argument.
	 * @param idx The 1-based index of the desired column.
	 */
	auto col(usize idx) const -> Col;

	// Matrix transposition
	auto transpose() const -> Matrix<Cols, Rows, T>;

	// Scalar multiplication
	auto operator*(T value) const -> Matrix;
	auto operator*=(T value) -> Matrix&;

	/** Calculate the determinant of the matrix. */
	auto determinant() const -> T;

	/**
	 * Calculate the cofactor for an element of the matrix.
	 * @param r The 1-based row index
	 * @param c The 1-based column index
	 */
	auto cofactor(usize r, usize c) const -> T;

	/**
	 * Calculate the minor determinant for an element of the matrix.
	 * @param r The 1-based row index
	 * @param c The 1-based column index
	 */
	auto minor(usize r, usize c) const -> T;

	/** Compute the matrix's inversion, if possible. */
	auto inverse() const -> std::optional<Matrix<Cols, Rows, T>>;
	/**
	 * Compute the matrix's inversion with a pre-computed determinant. Will abort
	 * in debug builds if `determinant` is zero.
	 *
	 * @param determinant The pre-computed determinant.
	 */
	auto inverse(T determinant) const -> Matrix<Cols, Rows, T>;

	/**
	 * Computes the determinant and writes it to the output parameter, returning
	 * a bool indicating whether or not the matrix is invertible.
	 */
	auto is_invertible(T& out_determinant) const -> bool;

	/** Compute the classical adjoint of the matrix. */
	auto adjoint() const -> Matrix<Cols, Rows, T>;

	/**
	 * Computes the transpose of the matrix and writes it to the output
	 * parameter, returning a bool indicating whether or not the matrix is
	 * orthogonal.
	 */
	auto is_orthogonal(
		Matrix<Cols, Rows, T>& out_transposed,
		T tolerance = std::numeric_limits<T>::epsilon()
	) const -> bool;
	auto is_orthogonal(T tolerance = std::numeric_limits<T>::epsilon()) const -> bool;

	constexpr auto is_identity(T tolerance = std::numeric_limits<T>::epsilon()) const -> bool;

	// Misc / Utility
	auto to_string(usize precision = 3) const -> std::string;

protected:
	std::array<Row, Rows> m_data; // NOLINT(*-non-private-member-*)
};

}
