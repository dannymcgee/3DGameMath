#include "math/_polar_decl.h"
#include <fmt/format.h>

#include <math/literals.h>
#include <math/polar.h>
#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT(*-using-namespace)
using namespace math::literals; // NOLINT(*-using-namespace)
using math::Vec2;
using math::Vec3;
using math::PolarCoords;
using math::SphericalCoords;

void polar_coords()
{
	auto polar = PolarCoords<f64>::from_cartesian(Vec2{ 1, 2 });
	fmt::print("Polar: {}\n", polar.to_string());

	auto cart = Vec2::from_polar(polar);
	fmt::print("Cartesian: {}\n", cart.to_string());

	auto aliased = PolarCoords<f64>{ -10.5, 270.0_deg };
	auto canon = aliased.canonical();
	fmt::print("Canonical: {} -> {}\n", aliased.to_string(), canon.to_string());

	auto aliased_cart = Vec2::from_polar(aliased);
	auto canon_cart = Vec2::from_polar(canon);
	fmt::print("Aliased (cartesian):   {}\n", aliased_cart.to_string());
	fmt::print("Canonical (cartesian): {}\n\n", canon_cart.to_string());

	auto spherical = SphericalCoords<f64>::from_cartesian(Vec3{ 1, 2, 3 });
	fmt::print("Spherical: {}\n", spherical.to_string());

	auto scart = Vec3::from_polar(spherical);
	fmt::print("Cartesian: {}\n", scart.to_string());

	auto saliased = SphericalCoords<f64>{ -10.5, 270_deg, -187.5_deg };
	auto scanon = saliased.canonical();
	fmt::print("Canonical: {} -> {}\n", saliased.to_string(), scanon.to_string());

	auto saliased_cart = Vec3::from_polar(saliased);
	auto scanon_cart = Vec3::from_polar(scanon);
	fmt::print("Aliased (cartesian):   {}\n", saliased_cart.to_string());
	fmt::print("Canonical (cartesian): {}\n\n", scanon_cart.to_string());
}
