#pragma once

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
	template <usize Index2D>
	auto m() const -> T;
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
	template <usize Index2D>
	auto m() -> T&;

	/**
	 * Access a member by 2D, 1-based index.
	 * @warning This method is **not bounds-checked**!
	 */
	auto m(usize idx_2d) const -> T;
	/**
	 * Access a member by 2D, 1-based index.
	 * @warning This method is **not bounds-checked**!
	 */
	auto m(usize idx_2d) -> T&;

	/**
	 * Access a member by 2D, 1-based index, with runtime bounds-checking. Use
	 * the templated `Matrix::m<size_t>()` for static, compile-time
	 * bounds-checking.
	 */
	auto m_checked(usize idx_2d) const -> T;
	/**
	 * Access a member by 2D, 1-based index, with runtime bounds-checking. Use
	 * the templated `Matrix::m<size_t>()` for static, compile-time
	 * bounds-checking.
	 */
	auto m_checked(usize idx_2d) -> T&;

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

	/** Get the row at the 0-based index. */
	auto operator[](usize idx) -> Row&;
	/** Get the row at the 0-based index. */
	auto operator[](usize idx) const -> const Row&;

	// Matrix transposition
	auto transpose() const -> Matrix<Cols, Rows, T>;

	// Scalar multiplication
	auto operator*(T value) const -> Matrix;
	auto operator*=(T value) -> Matrix&;

	/** Calculate the determinant of the matrix. */
	auto determinant() const -> T;

	// Misc / Utility
	auto to_string(usize precision = 3) const -> std::string;

protected:
	std::array<Row, Rows> m_data; // NOLINT(*-non-private-member-*)

	/** Throws an exception if the provided 2D, 1-based index is out of range. */
	void validate_index_2d(usize idx_2d) const;
};

}
