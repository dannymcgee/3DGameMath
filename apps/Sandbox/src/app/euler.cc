#include <fmt/format.h>

#include <math/euler.h>
#include <math/literals.h>
#include <math/matrix.h>
#include <math/matrix/translation.h>
#include <math/spaces.h>
#include <sized.h>

using namespace sized; // NOLINT(*-using-namespace)
using namespace math::literals; // NOLINT(*-using-namespace)
using math::Euler;
using math::Mat4x4;
using math::Space;
using math::TranslationMatrix;

void euler()
{
	auto rot = Euler<f64>{ 31.9_deg, -22.8_deg, 17.2_deg };
	fmt::print("rotation: {}\n", rot.to_string());

	auto l2w = rot.matrix();
	fmt::print("local->world:\n{}\n", l2w.to_string());

	auto w2l = rot.matrix(Space::Parent2Local);
	fmt::print("world->local:\n{}\n", w2l.to_string());

	auto roundtrip = l2w.euler();
	fmt::print("round-trip: {}\n\n", roundtrip.to_string());

	auto xlation = TranslationMatrix(5.0, 1.0, -12.125);
	auto local = Mat4x4{
		{ 1, 0, 0, 1 },
		{ 0, 1, 0, 1 },
		{ 0, 0, 1, 1 },
		{ 0, 0, 0, 1 },
	};
	auto transform = local * (l2w * xlation);
	fmt::print("transform matrix:\n{}\n", transform.to_string());

	auto q_l2w = rot.quat();
	fmt::print("Quat local->world: {}\n", q_l2w.to_string());
}
