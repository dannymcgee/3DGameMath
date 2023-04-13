#include <fmt/format.h>

#include <math/fmt.h>
#include <math/geo/circle.h>
#include <math/geo/tri.h>
#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT
using math::geo::Tri;
using math::Vec3;

void cart2bary_alt(const Tri& t, const Vec3& p)
{
	Vec3 d1 = t.edge<3>();
	Vec3 d2 = t.edge<1>();
	Vec3 n = d1 ^ d2;

	f64 u1, u2, u3, u4; // NOLINT
	f64 v1, v2, v3, v4; // NOLINT

	if (std::abs(n.x) >= std::abs(n.y)
		&& std::abs(n.x) >= std::abs(n.z))
	{
		// Discard x, project onto yz plane
		u1 = t.v1.y - t.v3.y;
		u2 = t.v2.y - t.v3.y;
		u3 = p.y - t.v1.y;
		u4 = p.y - t.v3.y;

		v1 = t.v1.z - t.v3.z;
		v2 = t.v2.z - t.v3.z;
		v3 = p.z - t.v1.z;
		v4 = p.z - t.v3.z;
	}
	else if (std::abs(n.y) >= std::abs(n.z)) {
		// Discard y, project onto xz plane
		u1 = t.v1.z - t.v3.z;
		u2 = t.v2.z - t.v3.z;
		u3 = p.z - t.v1.z;
		u4 = p.z - t.v3.z;

		v1 = t.v1.x - t.v3.x;
		v2 = t.v2.x - t.v3.x;
		v3 = p.x - t.v1.x;
		v4 = p.x - t.v3.x;
	}
	else {
		// Discard z, project onto xy plane
		u1 = t.v1.x - t.v3.x;
		u2 = t.v2.x - t.v3.x;
		u3 = p.x - t.v1.x;
		u4 = p.x - t.v3.x;

		v1 = t.v1.y - t.v3.y;
		v2 = t.v2.y - t.v3.y;
		v3 = p.y - t.v1.y;
		v4 = p.y - t.v3.y;
	}

	f64 denom = v1 * u2 - v2 * u1;

	if (math::nearly_equal(denom, 0))
		std::exit(1);

	f64 scale = 1.0 / denom;

	f64 x = (v4 * u2 - v2 * u4) * scale;
	f64 y = (v1 * u3 - v3 * u1) * scale;
	f64 z = 1.0 - x - y;

	auto result = Vec3{ x, y, z };

	fmt::print("result {}\n", result.to_string());
}

void tri()
{
	auto t = Tri{
		Vec3{ -1,  0, -0.5 },
		Vec3{  0,  0,  1   },
		Vec3{  1,  0, -0.5 },
	};
	auto bary2world = t.bary2cart();

	auto e1_center = Vec3{ 0, 0.5, 0.5 } * bary2world;
	auto e2_center = Vec3{ 0.5, 0, 0.5 } * bary2world;
	auto e3_center = Vec3{ 0.5, 0.5, 0 } * bary2world;

	auto begin = t.v1.begin();
	auto end = t.v3.end();
	auto formatter = math::fmt::AlignedValues(begin, end, 3);

	fmt::print("edge centers:\n{}\n{}\n{}\n\n",
		e1_center.to_string(formatter),
		e2_center.to_string(formatter),
		e3_center.to_string(formatter));

	auto e1_center2 = t.bary2cart(0, 0.5, 0.5);
	auto e2_center2 = t.bary2cart(0.5, 0, 0.5);
	auto e3_center2 = t.bary2cart(0.5, 0.5, 0);

	fmt::print("edge centers:\n{}\n{}\n{}\n\n",
		e1_center2.to_string(formatter),
		e2_center2.to_string(formatter),
		e3_center2.to_string(formatter));

	auto v1_bary = t.cart2bary(t.v1);
	fmt::print("v1 bary: {}\n", v1_bary.to_string(20));

	auto v2_bary = t.cart2bary(t.v2);
	fmt::print("v2 bary: {}\n", v2_bary.to_string(20));

	auto v3_bary = t.cart2bary(t.v3);
	fmt::print("v3 bary: {}\n", v3_bary.to_string(20));

	fmt::print("\n");

	constexpr f64 one_third = 1.0 / 3.0;
	auto tcenter = t.bary2cart(one_third, one_third, one_third);

	fmt::print("barycentric center: {}\n", tcenter.to_string());

	auto tcenter_bary = t.cart2bary(tcenter);
	fmt::print("tcenter bary: {}\n", tcenter_bary.to_string());

	auto above = Vec3{ 0, 2, 0 };
	auto above_bary = t.cart2bary(above);
	fmt::print("above bary:   {}\n", above_bary.to_string());

	auto below = Vec3{ 0, -2, 0 };
	auto below_bary = t.cart2bary(below);
	fmt::print("below bary:   {}\n\n", below_bary.to_string());

	cart2bary_alt(t, tcenter);
	cart2bary_alt(t, above);
	cart2bary_alt(t, below);

	f64 d1a = -(t.edge<2>() | t.edge<3>());
	f64 d1b = -t.edge<2>() | t.edge<3>();
	fmt::print("-(e2 | e3) = {}\n", d1a);
	fmt::print(" -e2 | e3  = {}\n", d1b);
}
