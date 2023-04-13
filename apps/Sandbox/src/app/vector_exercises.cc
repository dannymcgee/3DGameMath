#include <fmt/format.h>

#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT(*-using-namespace)
using math::Vector;

void vector_exercises()
{
	{
		auto a = Vector<2>{ -3, 8 };
		auto b = Vector<3>{ 4, 0, 5 };
		auto c = Vector<4>{ 16, -1, 4, 6 };
		fmt::print("by + cw + ax + bz == {}\n", b.y + c.w + a.x + b.z);
	}
	fmt::print("\n");
	fmt::print("Evaluate the vector expressions:\n");

	fmt::print("2. ||[-12, 5]|| == {}\n", Vector<2>{ -12, 5 }.length());
	fmt::print("3. ||[8, -3, 1/2]|| == {}\n", Vector<3>{ 8, -3, 0.5 }.length());
	{
		auto [x,y,z] = 3.f * Vector<3>{ 4, -7, 0 };
		fmt::print("4. 3[4, -7, 0] == [{}, {}, {}]\n", x, y, z);
	}
	{
		auto [x,y] = Vector<2>{ 4, 5 } / 2.f;
		fmt::print("5. [4, 5] / 2 == [{}, {}]\n", x, y);
	}

	fmt::print("\n");
	fmt::print("Normalize the following vectors:\n");
	{
		auto [x,y] = Vector<2>{ 12, 5 }.normal();
		fmt::print("1. [12, 5] -> [{}, {}]\n", x, y);
	}
	{
		auto [x,y] = Vector<2>{ 0, 743.632f }.normal();
		fmt::print("2. [0, 743.632] -> [{}, {}]\n", x, y);
	}
	{
		auto [x,y,z] = Vector<3>{ 8, -3, 0.5f }.normal();
		fmt::print("3. [8, -3, 1/2] -> [{}, {}, {}]\n", x, y, z);
	}
	{
		auto [x,y,z] = Vector<3>{ -12, 3, -4 }.normal();
		fmt::print("4. [-12, 3, -4] -> [{}, {}, {}]\n", x, y, z);
	}
	{
		auto [x,y,z,w] = Vector<4>::all(1).normal();
		fmt::print("5. [1, 1, 1, 1] -> [{}, {}, {}, {}]\n", x, y, z, w);
	}
}
