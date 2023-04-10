#include <algorithm>
#include <array>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <random>

#include <fmt/format.h>

#include <math/euler.h>
#include <math/fmt.h>
#include <math/geo/plane.h>
#include <math/literals.h>
#include <math/matrix/translation.h>
#include <math/polar.h>
#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT
using namespace math::literals; // NOLINT
using math::Euler;
using math::geo::Plane;
using math::PolarCoords;
using math::TranslationMatrix;
using math::Vec3;
using math::Vec4;

template <typename T>
class Rand {
public:
	Rand(T from, T to) : m_dist(from, to) {}

	auto get() -> T { return m_dist(m_device); }

private:
	std::random_device m_device;
	std::uniform_real_distribution<T> m_dist;
};

void triangle()
{
	auto p1 = Vec4{ -2,  0, -1,  1 };
	auto p2 = Vec4{ -2,  0,  1,  1 };
	auto p3 = Vec4{  2,  0,  0,  1 };

	auto rot = Euler<f64>{ 45_deg, -15_deg, 3.3_deg }.matrix();
	auto transform = rot * TranslationMatrix(1, 5.25, 0.25);

	p1 = p1 * transform;
	p2 = p2 * transform;
	p3 = p3 * transform;

	auto plane = Plane<f64>::from_points(
		{ p1.x, p1.y, p1.z },
		{ p2.x, p2.y, p2.z },
		{ p3.x, p3.y, p3.z });

	fmt::print("Plane{{\n");
	fmt::print("   normal: {},\n", plane.normal.to_string());
	fmt::print("   distance: {},\n", plane.distance);
	fmt::print("}}\n\n");
}

void best_fit()
{
	// Randomly generate 32 points on a fuzzy plane
	auto xz_rng = Rand<f64>(-20.0, 20.0);
	auto y_rng = Rand<f64>(0.5, 1.5);

	std::array<Vec3, 32> points;
	for (auto& p : points) {
		p.x = xz_rng.get();
		p.y = y_rng.get();
		p.z = xz_rng.get();
	}

	// Sort the points in a clockwise direction
	std::sort(points.begin(), points.end(),
		[](const Vec3& a, const Vec3& b) -> bool {
			auto theta_a = PolarCoords<f64>::from_cartesian(a.x, a.z).angle;
			auto theta_b = PolarCoords<f64>::from_cartesian(b.x, b.z).angle;
			return theta_a > theta_b;
		});

	// Transform all of the points
	auto rot = Euler<f64>{ 45_deg, -15_deg, 3.3_deg }.matrix();
	auto transform = rot * TranslationMatrix(1, 5.25, 0.25);

	for (auto& p : points) {
		auto xformed = Vec4{ p.x, p.y, p.z, 1 } * transform;
		auto [x,y,z,_] = xformed;
		p = Vec3{ x, y, z };
	}

	// Print all of the points
	auto begin = points.front().begin();
	auto end = points.back().end();
	auto formatter = math::fmt::AlignedValues(begin, end, 3);

	fmt::print("Points:\n");
	for (const auto& p : points)
		fmt::print("   {}\n", p.to_string(formatter));

	// Create the best-fit plane
	auto plane = Plane<f64>::best_fit(points);
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
