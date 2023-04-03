#include "math/_matrix_decl.h"
#include "math/fmt.h"
#include <array>
#include <cstdlib>
#include <iostream>

#include <fmt/format.h>

#include <math/matrix.h>
#include <math/matrix/rotation.h>
#include <math/matrix/scale.h>
#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT;
using math::Matrix;
using math::Vector;
using math::fmt::AlignedValues;

using Vec3 = math::Vector<3>;
using Mat3x3 = math::Matrix<3,3>;

void vector_exercises()
{
	{
		auto a = Vector<2, f32>{ -3, 8 };
		auto b = Vector<3, f32>{ 4, 0, 5 };
		auto c = Vector<4, f32>{ 16, -1, 4, 6 };
		fmt::print("by + cw + ax + bz == {}\n", b.y + c.w + a.x + b.z);
	}
	fmt::print("\n");
	fmt::print("Evaluate the vector expressions:\n");
	{
		auto [x,y] = -Vector<2, i32>{ 3, 7 };
		fmt::print("1. -[3, 7] == [{}, {}]\n", x, y);
	}
	fmt::print("2. ||[-12, 5]|| == {}\n", Vector<2, f32>{ -12, 5 }.length());
	fmt::print("3. ||[8, -3, 1/2]|| == {}\n", Vector<3, f32>{ 8, -3, 0.5 }.length());
	{
		auto [x,y,z] = 3.f * Vector<3, f32>{ 4, -7, 0 };
		fmt::print("4. 3[4, -7, 0] == [{}, {}, {}]\n", x, y, z);
	}
	{
		auto [x,y] = Vector<2, f32>{ 4, 5 } / 2.f;
		fmt::print("5. [4, 5] / 2 == [{}, {}]\n", x, y);
	}

	fmt::print("\n");
	fmt::print("Normalize the following vectors:\n");
	{
		auto [x,y] = Vector<2, f32>{ 12, 5 }.normal();
		fmt::print("1. [12, 5] -> [{}, {}]\n", x, y);
	}
	{
		auto [x,y] = Vector<2, f32>{ 0, 743.632f }.normal();
		fmt::print("2. [0, 743.632] -> [{}, {}]\n", x, y);
	}
	{
		auto [x,y,z] = Vector<3, f32>{ 8, -3, 0.5f }.normal();
		fmt::print("3. [8, -3, 1/2] -> [{}, {}, {}]\n", x, y, z);
	}
	{
		auto [x,y,z] = Vector<3, f32>{ -12, 3, -4 }.normal();
		fmt::print("4. [-12, 3, -4] -> [{}, {}, {}]\n", x, y, z);
	}
	{
		auto [x,y,z,w] = Vector<4, f32>::all(1).normal();
		fmt::print("5. [1, 1, 1, 1] -> [{}, {}, {}, {}]\n", x, y, z, w);
	}
}

void coord_spaces()
{
	using Vec3 = math::Vector<3, f64>;

	// body-space vector
	auto b = Vec3{ 1, 2, 3 };

	// origin
	auto o = Vec3{ 3, 2, 1 };

	// upright-space vector
	auto u = b + o; // [4 4 4]
	fmt::print("u: [{} {} {}]\n", u.x, u.y, u.z);

	// world basis vectors
	auto i = Vec3{ 1, 0, 0 };
	auto j = Vec3{ 0, 1, 0 };
	auto k = Vec3{ 0, 0, 1 };

	// body basis vectors
	auto p = i + o; // [4 2 1]
	auto q = j + o; // [3 3 1]
	auto r = k + o; // [3 2 2]

	// u == bx*p + by*q + bz*r
	//   == 1[4 2 1] + 2[3 3 1] + 3[3 2 2]
	//   == [4 2 1] + [6 6 2] + [9 6 6]
	//   == [10 8 3] + [9 6 6]
	//   == [19 14 9]

	// ux == bx*px + by*qx + bz*rx
	//    == 1*4 + 2*3 + 3*3
	//    == 4 + 6 + 9
	//    == 19

	// uy == bx*py + by*qy + bz*ry
	//    == 1*2 + 2*3 + 3*2
	//    == 2 + 6 + 6
	//    == 14

	// uz == 1*1 + 2*1 + 3*2
	//    == 1 + 2 + 6
	//    == 9

	// What if b is unknown?
	// bx == u|p == u|[1 0 0] == ux
	// by == u|q == u|[0 1 0] == uy
	// bz == u|r == u|[0 0 1] == uz
}

void matrix()
{
	using Mat4x3 = Matrix<4,3,f64>;

	auto mat = Mat4x3{
		{ 11, 12, 13 },
		{ 21, 22, 23 },
		{ 31, 32, 33 },
		{ 41, 42, 43 },
	};
	fmt::print("{}\n", mat.to_string());

	auto m21 = mat.m<21>();
	fmt::print("m21: {}\n", m21);

	mat.m<21>() = 5;
	fmt::print("{}\n", mat.to_string());

	auto transposed = mat.transpose();
	fmt::print("{}\n", transposed.to_string());
}

void rotation_matrix()
{
	using math::RotationMatrix;
	using math::fmt::AlignedValues;
	using Vec3 = math::Vector<3>;
	using Mat3x3 = math::Matrix<3,3>;

	auto angle = math::deg2rad(45.0);
	auto axis = Vec3{ -0.25, 0.5, 0.33 }.unit();

	auto rotation_mat = RotationMatrix(angle, axis);

	auto rotated = Mat3x3::identity() * rotation_mat;
	auto formatter = AlignedValues(rotated.begin(), rotated.end(), 5);

	fmt::print("============= Rotation Matrix =============\n");
	fmt::print("Right:   {}\n", rotated.row(1).to_string(formatter));
	fmt::print("Up:      {}\n", rotated.row(2).to_string(formatter));
	fmt::print("Forward: {}\n", rotated.row(3).to_string(formatter));
	fmt::print("-------------------------------------------\n");
	fmt::print("Axis:    {}\n", axis.to_string(formatter));
	fmt::print("Radians: {}\n", angle);
	fmt::print("\n");

	fmt::print("Rotation matrix is orthogonal? {}\n", rotation_mat.is_orthogonal());

	auto transposed = rotation_mat.transpose();
	auto id = rotation_mat * transposed;
	fmt::print("RM*RM^T:\n{}\n", id.to_string());
}

void scale_matrix()
{
	using math::ScaleMatrix;
	using Mat3x3 = math::Matrix<3,3>;
	using Vec3 = math::Vector<3>;

	auto mat = ScaleMatrix(-12.125);
	fmt::print("ScaleMatrix(-12.125)\n{}\n", mat.to_string());

	auto non_uniform = ScaleMatrix({ -1.0, 1.5, 1.0 });
	fmt::print("non-uniform:\n{}\n", non_uniform.to_string(5));

	auto scaled = Mat3x3::identity() * non_uniform;
	fmt::print("scaled identity:\n{}\n", scaled.to_string(5));
}

void matrix_determinants()
{
	auto mat2x2 = Matrix<2,2,f64>{
		{ -3.0, 4.0 },
		{  2.0, 5.0 },
	};
	fmt::print("Mat2x2:\n{}", mat2x2.to_string());
	fmt::print("determinant: {}\n\n", mat2x2.determinant());

	auto mat3x3 = Matrix<3,3,f64>{
		{ -4.0, -3.0,  3.0 },
		{  0.0,  2.0, -2.0 },
		{  1.0,  4.0, -1.0 },
	};
	fmt::print("Mat3x3:\n{}", mat3x3.to_string());
	fmt::print("determinant: {}\n\n", mat3x3.determinant());

	auto mat4x4 = Matrix<4,4,f64>{
		{ -4.0, -3.0,  3.0,  1.0 },
		{  0.0,  2.0, -2.0,  0.0 },
		{  1.0,  4.0, -1.0,  1.0 },
		{  0.0,  2.0, -2.0,  1.0 },
	};
	fmt::print("Mat4x4:\n{}", mat4x4.to_string());
	fmt::print("determinant: {}\n\n", mat4x4.determinant());
}

void invalid_matrix_ctor()
{
	auto whoops = Matrix<2,3,f64>{
		{ 11, 12, 13 },
		{ 21, 22, 23 },
		{ 31, 32, 33 },
	};
	fmt::print("Truncated Mat2x3:\n{}", whoops.to_string());
}

void matrix_inversion()
{
	auto mat = Matrix<4,4>{
		{ -4.0, -3.0,  3.0,  1.0 },
		{  0.0,  2.0, -2.0,  0.0 },
		{  1.0,  4.0, -1.0,  1.0 },
		{  0.0,  2.0, -2.0,  1.0 },
	};
	fmt::print("Mat4x4:\n{}\n", mat.to_string());

	auto inverted = mat.inverse();
	if (inverted)
		fmt::print("Inverse Mat4x4:\n{}\n", inverted->to_string());
}

void print_error(const Mat3x3& lhs, const Mat3x3& rhs, const AlignedValues& formatter)
{
	auto print_row = [&](const Vec3& row) -> f64 {
		f64 avg = (std::abs(row.x) + std::abs(row.y) + std::abs(row.z)) / 3.0;
		fmt::print("{}   ~{:.5}\n", row.to_string(formatter), avg);

		return avg;
	};

	fmt::print("\n");

	f64 avg1 = print_row(rhs.row<1>() - lhs.row<1>());
	f64 avg2 = print_row(rhs.row<2>() - lhs.row<2>());
	f64 avg3 = print_row(rhs.row<3>() - lhs.row<3>());

	fmt::print("Avg overall error: ~{}\n\n", (avg1 + avg2 + avg3) / 3.0);
}

void matrix_orthogonality()
{
	using math::RotationMatrix;
	// using math::OrthoStrategy;
	// using Ortho = math::OrthoStrategy;
	using Vec3 = math::Vector<3>;

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

auto main() -> int
{
	// matrix_inversion();
	// rotation_matrix();
	// scale_matrix();
	// matrix_determinants();
	// invalid_matrix_ctor();
	matrix_orthogonality();
}
