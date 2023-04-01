#include "math/fmt.h"
#include <array>
#include <iostream>

#include <fmt/format.h>

#include <math/matrix.h>
#include <math/matrix/rotation.h>
#include <math/matrix/scale.h>
#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT;
using math::Vector;
using math::Matrix;

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

	auto angle = math::deg2rad(45.0);
	auto axis = Vec3{ -0.25, 0.5, 0.33 }.unit();

	auto rotation_mat = RotationMatrix(angle, axis);

	auto x = Vec3{ 1, 0, 0 } * rotation_mat;
	auto y = Vec3{ 0, 1, 0 } * rotation_mat;
	auto z = Vec3{ 0, 0, 1 } * rotation_mat;

	auto formatter = AlignedValues(x.begin(), z.end(), 5);

	fmt::print("============= Rotation Matrix =============\n");
	fmt::print("Right:   {}\n", x.to_string(formatter));
	fmt::print("Up:      {}\n", y.to_string(formatter));
	fmt::print("Forward: {}\n", z.to_string(formatter));
	fmt::print("-------------------------------------------\n");
	fmt::print("Axis:    {}\n", axis.to_string(formatter));
	fmt::print("Radians: {}\n", angle);
	fmt::print("\n");
}

void scale_matrix()
{
	using math::ScaleMatrix;
	using Mat3x3 = math::Matrix<3,3>;
	using Vec3 = math::Vector<3>;

	auto mat = ScaleMatrix(10.0);
	fmt::print("ScaleMatrix(10)\n{}\n", mat.to_string());

	auto non_uniform = ScaleMatrix({ -1.0, 1.5, 1.0 });
	fmt::print("non-uniform:\n{}\n", non_uniform.to_string(5));

	auto scaled = Mat3x3::identity() * non_uniform;
	fmt::print("scaled identity:\n{}\n", scaled.to_string(5));
}

auto main() -> int
{
	rotation_matrix();
	scale_matrix();
}