#include <algorithm>
#include <array>
#include <cstdlib>

#include <fmt/format.h>

#include <math/euler.h>
#include <math/fmt.h>
#include <math/geo/plane.h>
#include <math/literals.h>
#include <math/matrix/transform.h>
#include <math/polar.h>
#include <math/random.h>
#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT
using namespace math::literals; // NOLINT
using math::Euler;
using math::geo::Plane;
using math::PolarCoords;
using math::TransformMatrix;
using math::Vec3;
using math::Vec4;

void triangle()
{
	auto p1 = Vec3{ -2,  0, -1 };
	auto p2 = Vec3{ -2,  0,  1 };
	auto p3 = Vec3{  2,  0,  0 };

	auto transform = TransformMatrix{
		Euler{ 45_deg, -15_deg, 3.3_deg }.quat(),
		Vec3{ 1, 5.25, 0.25 },
	};

	p1 = transform.transform_point(p1);
	p2 = transform.transform_point(p2);
	p3 = transform.transform_point(p3);

	auto plane = Plane::from_points(p1, p2, p3);

	fmt::print("Plane{{\n");
	fmt::print("   normal: {},\n", plane.normal.to_string());
	fmt::print("   distance: {},\n", plane.distance);
	fmt::print("}}\n\n");
}

void best_fit()
{
	// Randomly generate 32 points on a fuzzy plane
	auto xz_rng = math::Random<flt>(-20.0, 20.0);
	auto y_rng = math::Random<flt>(0.5, 1.5);

	std::array<Vec3, 32> points;
	for (auto& p : points) {
		p.x = xz_rng.get();
		p.y = y_rng.get();
		p.z = xz_rng.get();
	}

	// Sort the points in a clockwise direction
	std::sort(points.begin(), points.end(),
		[](const Vec3& a, const Vec3& b) -> bool {
			auto theta_a = PolarCoords::from_cartesian(a.x, a.z).angle;
			auto theta_b = PolarCoords::from_cartesian(b.x, b.z).angle;
			return theta_a > theta_b;
		});

	// Transform all of the points
	auto transform = TransformMatrix{
		Euler{ 45_deg, -15_deg, 3.3_deg }.quat(),
		Vec3{ 1, 5.25, 0.25 },
	};

	for (auto& p : points)
		p = transform.transform_point(p);

	// Print all of the points
	auto begin = points.front().begin();
	auto end = points.back().end();
	auto formatter = math::fmt::AlignedValues(begin, end, 3);

	fmt::print("Points:\n");
	for (const auto& p : points)
		fmt::print("   {}\n", p.to_string(formatter));

	// Create the best-fit plane
	auto plane = Plane::best_fit(points);
	fmt::print("\nPlane{{\n");
	fmt::print("   normal: {},\n", plane.normal.to_string());
	fmt::print("   distance: {},\n", plane.distance);
	fmt::print("}}\n\n");
}

void plane()
{
	triangle();
	best_fit();
}
