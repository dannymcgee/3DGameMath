#pragma once

#include <array>
#include <limits>
#include <optional>
#include <string>

#include <sized.h>

#include "math/vector.h"


namespace math {
using namespace sized; // NOLINT(*-using-namespace)

namespace detail {

template <usize Rows, usize Cols>
class Matrix {
public:
	using Row = ::math::Vector<Cols>;

	union {
		std::array<Row,Rows> m_data {};
	};
};

template <>
class Matrix<2,2> {
public:
	using Row = ::math::Vector<2>;

	union {
		std::array<Row,2> m_data {
			Row{ 0, 0 },
			Row{ 0, 0 },
		};
		struct {
			flt m11, m12,
			    m21, m22;
		};
	};
};

template <>
class Matrix<3,3> {
public:
	using Row = ::math::Vector<3>;

	union {
		std::array<Row,3> m_data {
			Row{ 0, 0, 0 },
			Row{ 0, 0, 0 },
			Row{ 0, 0, 0 },
		};
		struct {
			flt m11, m12, m13,
			    m21, m22, m23,
			    m31, m32, m33;
		};
	};
};

template <>
class Matrix<4,3> {
public:
	using Row = ::math::Vector<3>;

	union {
		std::array<Row,4> m_data {
			Row{ 0, 0, 0 },
			Row{ 0, 0, 0 },
			Row{ 0, 0, 0 },
			Row{ 0, 0, 0 },
		};
		struct {
			flt m11, m12, m13,
			    m21, m22, m23,
			    m31, m32, m33,
			    m41, m42, m43;
		};
	};
};

template <>
class Matrix<3,4> {
public:
	using Row = ::math::Vector<4>;

	union {
		std::array<Row,3> m_data {
			Row{ 0, 0, 0, 0 },
			Row{ 0, 0, 0, 0 },
			Row{ 0, 0, 0, 0 },
		};
		struct {
			flt m11, m12, m13, m14,
			    m21, m22, m23, m24,
			    m31, m32, m33, m34;
		};
	};
};

template <>
class Matrix<4,4> {
public:
	using Row = ::math::Vector<4>;

	union {
		std::array<Row,4> m_data {
			Row{ 0, 0, 0, 0 },
			Row{ 0, 0, 0, 0 },
			Row{ 0, 0, 0, 0 },
			Row{ 0, 0, 0, 0 },
		};
		struct {
			flt m11, m12, m13, m14,
			    m21, m22, m23, m24,
			    m31, m32, m33, m34,
			    m41, m42, m43, m44;
		};
	};
};

} // namespace detail

template <usize Rows, usize Cols>
class Matrix : public detail::Matrix<Rows,Cols> {
public:
	using Super = detail::Matrix<Rows,Cols>;
	using Row = Vector<Cols>;
	using Col = Vector<Rows>;

protected:
	using Super::m_data;

public:
	// Constructors
	Matrix() = default;
	constexpr Matrix(std::initializer_list<Row> rows);

	static constexpr auto identity() -> Matrix;

	// Iterator support
	auto begin() -> detail::RawIterator<flt>;
	auto begin() const -> detail::RawConstIterator<flt>;

	auto end() -> detail::RawIterator<flt>;
	auto end() const -> detail::RawConstIterator<flt>;

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
	auto m() const -> flt;
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
	auto m() -> flt&;

	/**
	 * Access a member by the 1-based row and column indices.
	 * @warning This method is **not bounds-checked**!
	 */
	auto m(usize row, usize col) const -> flt;
	/**
	 * Access a member by the 1-based row and column indices.
	 * @warning This method is **not bounds-checked**!
	 */
	auto m(usize row, usize col) -> flt&;

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
	 * Get a mutable reference to the row at the 1-based index indicated by the
	 * template argument.
	 * @tparam Index The 1-based index of the desired row.
	 */
	template <usize Index>
	auto row() -> Row&;
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
	 * Get a mutable reference to the row at the 1-based index indicated by the
	 * argument.
	 * @param idx The 1-based index of the desired row.
	 */
	auto row(usize idx) -> Row&;
	/**
	 * Get the column at the 1-based index indicated by the argument.
	 * @param idx The 1-based index of the desired column.
	 */
	auto col(usize idx) const -> Col;

	// Matrix transposition
	auto transpose() const -> Matrix<Cols, Rows>;

	// Scalar multiplication
	constexpr auto operator*(flt value) const -> Matrix;
	auto operator*=(flt value) -> Matrix&;

	/** Calculate the determinant of the matrix. */
	auto determinant() const -> flt;

	/**
	 * Calculate the cofactor for an element of the matrix.
	 * @param r The 1-based row index
	 * @param c The 1-based column index
	 */
	auto cofactor(usize r, usize c) const -> flt;

	/**
	 * Calculate the minor determinant for an element of the matrix.
	 * @param r The 1-based row index
	 * @param c The 1-based column index
	 */
	auto minor(usize r, usize c) const -> flt;

	/** Compute the matrix's inversion, if possible. */
	auto inverse() const -> std::optional<Matrix<Cols, Rows>>;
	/**
	 * Compute the matrix's inversion with a pre-computed determinant. Will abort
	 * in debug builds if `determinant` is zero.
	 *
	 * @param determinant The pre-computed determinant.
	 */
	auto inverse(flt determinant) const -> Matrix<Cols, Rows>;

	/** Compute the classical adjoint of the matrix. */
	auto adjoint() const -> Matrix<Cols, Rows>;

	/**
	 * Correct the orthogonality of a matrix, e.g. due to floating-point error
	 * accumulation.
	 */
	void orthogonalize();

	/**
	 * Computes the determinant and writes it to the output parameter, returning
	 * a bool indicating whether or not the matrix is invertible.
	 */
	auto is_invertible(flt& out_determinant) const -> bool;

	/**
	 * Computes the transpose of the matrix and writes it to the output
	 * parameter, returning a bool indicating whether or not the matrix is
	 * orthogonal.
	 */
	auto is_orthogonal(
		Matrix<Cols, Rows>& out_transposed,
		flt tolerance = std::numeric_limits<flt>::epsilon()
	) const -> bool;
	auto is_orthogonal(flt tolerance = std::numeric_limits<flt>::epsilon()) const -> bool;

	constexpr auto is_identity(flt tolerance = std::numeric_limits<flt>::epsilon()) const -> bool;

	// Misc / Utility
	auto to_string(usize precision = 3) const -> std::string;
};

}
