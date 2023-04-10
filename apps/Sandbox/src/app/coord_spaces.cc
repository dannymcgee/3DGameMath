#include <fmt/format.h>

#include <math/vector.h>

using math::Vec3;

void coord_spaces()
{
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
	fmt::print("p: {}\n", p.to_string());
	fmt::print("q: {}\n", q.to_string());
	fmt::print("r: {}\n", r.to_string());

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
