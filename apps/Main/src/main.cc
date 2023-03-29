#include <array>
#include <iostream>

#include <fmt/format.h>

#include <math/matrix.h>
#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT;
using math::Vec;
using math::Mat;

void vector_exercises()
{
	{
		auto a = Vec<2, f32>{ -3, 8 };
		auto b = Vec<3, f32>{ 4, 0, 5 };
		auto c = Vec<4, f32>{ 16, -1, 4, 6 };
		fmt::print("by + cw + ax + bz == {}\n", b.y + c.w + a.x + b.z);
	}
	fmt::print("\n");
	fmt::print("Evaluate the vector expressions:\n");
	{
		auto [x,y] = -Vec<2, i32>{ 3, 7 };
		fmt::print("1. -[3, 7] == [{}, {}]\n", x, y);
	}
	fmt::print("2. ||[-12, 5]|| == {}\n", Vec<2, f32>{ -12, 5 }.length());
	fmt::print("3. ||[8, -3, 1/2]|| == {}\n", Vec<3, f32>{ 8, -3, 0.5 }.length());
	{
		auto [x,y,z] = 3.f * Vec<3, f32>{ 4, -7, 0 };
		fmt::print("4. 3[4, -7, 0] == [{}, {}, {}]\n", x, y, z);
	}
	{
		auto [x,y] = Vec<2, f32>{ 4, 5 } / 2.f;
		fmt::print("5. [4, 5] / 2 == [{}, {}]\n", x, y);
	}

	fmt::print("\n");
	fmt::print("Normalize the following vectors:\n");
	{
		auto [x,y] = Vec<2, f32>{ 12, 5 }.normal();
		fmt::print("1. [12, 5] -> [{}, {}]\n", x, y);
	}
	{
		auto [x,y] = Vec<2, f32>{ 0, 743.632f }.normal();
		fmt::print("2. [0, 743.632] -> [{}, {}]\n", x, y);
	}
	{
		auto [x,y,z] = Vec<3, f32>{ 8, -3, 0.5f }.normal();
		fmt::print("3. [8, -3, 1/2] -> [{}, {}, {}]\n", x, y, z);
	}
	{
		auto [x,y,z] = Vec<3, f32>{ -12, 3, -4 }.normal();
		fmt::print("4. [-12, 3, -4] -> [{}, {}, {}]\n", x, y, z);
	}
	{
		auto [x,y,z,w] = Vec<4, f32>::all(1).normal();
		fmt::print("5. [1, 1, 1, 1] -> [{}, {}, {}, {}]\n", x, y, z, w);
	}
}


void matrix()
{
	using Mat4x3 = Mat<4,3>;

	auto mat = Mat4x3{
		{ 0, 0, 0 },
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 },
	};
}

auto main() -> int
{
	matrix();
}
