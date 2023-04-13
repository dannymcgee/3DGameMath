#include <fmt/format.h>

#include <math/fmt.h>
#include <math/matrix.h>
#include <math/matrix/rotation.h>
#include <sized.h>

using namespace sized; // NOLINT(*-using-namespace)
using math::fmt::AlignedValues;
using math::Mat3x3;
using math::Vec3;
using math::RotationMatrix;

void print_error(const Mat3x3& lhs, const Mat3x3& rhs, const AlignedValues& formatter)
{
	auto print_row = [&](const Vec3& row) -> flt {
		flt avg = (std::abs(row.x) + std::abs(row.y) + std::abs(row.z)) / 3.0;
		fmt::print("{}   ~{:.5}\n", row.to_string(formatter), avg);

		return avg;
	};

	fmt::print("\n");

	flt avg1 = print_row(rhs.row<1>() - lhs.row<1>());
	flt avg2 = print_row(rhs.row<2>() - lhs.row<2>());
	flt avg3 = print_row(rhs.row<3>() - lhs.row<3>());

	fmt::print("Avg overall error: ~{}\n\n", (avg1 + avg2 + avg3) / 3.0);
}

void matrix_orthogonality()
{
	auto angle = math::deg2rad(45.0);
	auto axis = Vec3{ -0.25, 0.5, 0.33 }.unit();
	auto mat = static_cast<Mat3x3>(RotationMatrix(angle, axis));

	fmt::print("Start:\n{}\n", mat.to_string(5));

	// Fuck it up!
	usize i = 0;
	const auto initial = mat;
	while (mat.is_orthogonal(0.0001)) {
		auto tposed = mat.transpose();
		auto id = mat * tposed;
		mat = mat * id;
		++i;
	}

	auto formatter = AlignedValues(mat.begin(), mat.end(), 5);

	fmt::print("Fucked after {} iterations:\n{}--------------------------------",
		i, mat.to_string(5));
	print_error(initial, mat, formatter);

	// Vanilla Gram-Schmidt
	// auto gram_schmidt = mat;
	// gram_schmidt.orthogonalize(Ortho::GramSchmidt);
	// fmt::print("Gram-Schmidt:\n{}--------------------------------",
	// 	gram_schmidt.to_string(5));
	// print_error(initial, gram_schmidt, formatter);

	// Gram-Schmidt + Unbiased
	// auto unbiased = mat;
	// unbiased.orthogonalize(Ortho::GramSchmidt | Ortho::Unbiased);
	// fmt::print("Unbiased Gram-Schmidt:\n{}--------------------------------",
	// 	unbiased.to_string(5));
	// print_error(initial, unbiased, formatter);

	// Gram-Schmidt + Unbiased x2
	// auto unbiased_2pass = mat;
	// unbiased_2pass.orthogonalize(Ortho::GramSchmidt | Ortho::Unbiased);
	// unbiased_2pass.orthogonalize(Ortho::GramSchmidt | Ortho::Unbiased);
	// fmt::print("Unbiased Gram-Schmidt (x2):\n{}--------------------------------",
	// 	unbiased_2pass.to_string(5));
	// print_error(initial, unbiased_2pass, formatter);

	// Gram-Schmidt + Derived 3rd
	// auto gs_derive_3rd = mat;
	// gs_derive_3rd.orthogonalize(Ortho::GramSchmidt | Ortho::DeriveThird);
	// fmt::print("Gram-Schmidt + Derived 3rd:\n{}--------------------------------",
	// 	gs_derive_3rd.to_string(5));
	// print_error(initial, gs_derive_3rd, formatter);

	// Default
	auto ortho = mat;
	ortho.orthogonalize();
	fmt::print("Orthogonalized:\n{}--------------------------------",
		ortho.to_string(5));
	print_error(initial, ortho, formatter);

	// Gram-Schmidt + Unbiased + Derived 3rd
	// auto whole9 = mat;
	// whole9.orthogonalize(Ortho::GramSchmidt | Ortho::Unbiased | Ortho::DeriveThird);
	// fmt::print("Unbiased Gram-Schmidt with Derived 3rd:\n{}--------------------------------",
	// 	whole9.to_string(5));
	// print_error(initial, whole9, formatter);
}
