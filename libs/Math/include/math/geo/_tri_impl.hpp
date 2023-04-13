#pragma once

#include "math/geo/_tri_decl.h"

#include <array>
#include <cstdlib>

#include "math/assert.h"
#include "math/geo/circle.h"

namespace math::geo {

// Derived properties ----------------------------------------------------------

template <usize Index>
constexpr auto Tri::edge() const -> Vec3
{
	static_assert(Index > 0 && Index <= 3, "Expected an Index in the range [1,3]");

	if constexpr (Index == 1) return v3 - v2;
	if constexpr (Index == 2) return v1 - v3;
	if constexpr (Index == 3) return v2 - v1;
}

constexpr auto Tri::edge(usize index) const -> Vec3
{
	ASSERT(index > 0 && index <= 3, // NOLINT
		"Expected an index in the range [1,3], received {}", index);

	switch (index) {
		case 1: return edge<1>();
		case 2: return edge<2>();
		case 3: return edge<3>();
	}
	return Vec3::Zero;
}

constexpr auto Tri::perimeter() const -> flt
{
	return edge<1>().length()
	     + edge<2>().length()
	     + edge<3>().length();
}

constexpr auto Tri::area() const -> flt
{
	return 0.5 * (edge<1>() ^ edge<2>()).length();
}

constexpr auto Tri::centroid() const -> Vec3
{
	constexpr flt one_third = 1_flt / 3_flt;
	return bary2cart(one_third, one_third, one_third);
}

constexpr auto Tri::incenter() const -> Vec3
{
	flt l1 = edge<1>().length();
	flt l2 = edge<2>().length();
	flt l3 = edge<3>().length();

	flt perim = l1 + l2 + l3;
	flt scale = 1 / perim;

	return bary2cart(l1 * scale, l2 * scale, l3 * scale);
}

constexpr auto Tri::inscribed_circle() const -> Circle
{
	Vec3 e1 = edge<1>();
	Vec3 e2 = edge<2>();
	Vec3 e3 = edge<3>();

	flt l1 = e1.length();
	flt l2 = e2.length();
	flt l3 = e3.length();

	flt perim = l1 + l2 + l3;
	flt perim_scale = 1 / perim;

	auto center = bary2cart(l1 * perim_scale, l2 * perim_scale, l3 * perim_scale);

	Vec3 e1_x_e2 = e1 ^ e2;
	auto [e1_x_e2_length, normal] = e1_x_e2.length_and_direction();

	flt area = 0.5 * e1_x_e2_length;
	flt radius = 2 * area / perim;

	return Circle{
		center,
		normal,
		radius,
	};
}

constexpr auto Tri::circumcenter() const -> Vec3
{
	Vec3 e1 = edge<1>();
	Vec3 e2 = edge<2>();
	Vec3 e3 = edge<3>();

	flt d1 = -e2 | e3;
	flt d2 = -e3 | e1;
	flt d3 = -e1 | e2;

	flt c1 = d2 * d3;
	flt c2 = d3 * d1;
	flt c3 = d1 * d2;

	flt c = c1 + c2 + c3;
	flt two_c_inv = 1 / (2 * c);

	return bary2cart(
		(c2 + c3) * two_c_inv,
		(c3 + c1) * two_c_inv,
		(c1 + c2) * two_c_inv);
}

constexpr auto Tri::circumscribed_circle() const -> Circle
{
	Vec3 e1 = edge<1>();
	Vec3 e2 = edge<2>();
	Vec3 e3 = edge<3>();

	flt d1 = -e2 | e3;
	flt d2 = -e3 | e1;
	flt d3 = -e1 | e2;

	flt c1 = d2 * d3;
	flt c2 = d3 * d1;
	flt c3 = d1 * d2;

	flt c = c1 + c2 + c3;
	flt two_c_inv = 1 / (2 * c);

	Vec3 center = bary2cart(
		(c2 + c3) * two_c_inv,
		(c3 + c1) * two_c_inv,
		(c1 + c2) * two_c_inv);

	Vec3 normal = (e1 ^ e2).normal();

	flt radius = std::sqrt((d1 + d2) * (d2 + d3) * (d3 + d1) / c) * 0.5;

	return Circle{
		center,
		normal,
		radius,
	};
}


// Barycentric <-> Cartesian coordinates ---------------------------------------

constexpr auto Tri::bary2cart() const -> Matrix<3,3>
{
	return Matrix<3,3>{ v1, v2, v3 };
}

constexpr auto Tri::bary2cart(flt x, flt y, flt z) const -> Vec3
{
	return x * v1 + y * v2 + z * v3;
}

constexpr auto Tri::bary2cart(const Vec3& coords) const -> Vec3
{
	auto [x,y,z] = coords;
	return bary2cart(x, y, z);
}

inline auto Tri::cart2bary(flt x, flt y, flt z) const -> Vec3
{
	return cart2bary(Vec3{ x, y, z });
}

inline auto Tri::cart2bary(const Vec3& p) const -> Vec3
{
	Vec3 e1 = edge<1>();
	Vec3 e2 = edge<2>();
	Vec3 e3 = edge<3>();

	Vec3 d1 = p - v1;
	Vec3 d2 = p - v2;
	Vec3 d3 = p - v3;

	Vec3 e1_x_e2 = e1 ^ e2;
	Vec3 n = e1_x_e2.normal();

	flt at  = (e1_x_e2 | n);
	flt at1 = ((e1 ^ d3) | n);
	flt at2 = ((e2 ^ d1) | n);
	flt at3 = ((e3 ^ d2) | n);

	// TODO: Handle potential div by 0?
	flt scale = 1 / at;

	return Vec3{
		scale * at1,
		scale * at2,
		scale * at3,
	};
}

} // namespace math::geo
