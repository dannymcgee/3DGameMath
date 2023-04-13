#include <fmt/format.h>

#include <math/literals.h>
#include <math/quat.h>
#include <math/utility.h>
#include <sized.h>

using namespace sized; // NOLINT(*-using-namespace)
using namespace math::literals; // NOLINT(*-using-namespace)
using math::Vec3;
using math::Quat;

void slerp()
{
	f64 t = 0;
	auto start = Quat::angle_axis(90_deg, Vec3::up());
	auto end = Quat::angle_axis(270_deg, Vec3::up());

	while (t <= 1) {
		auto state = Quat::slerp(start, end, t);
		auto [angle,axis] = state.angle_axis();
		fmt::print("{:>7.3f}°, {}   |   {}\n",
			math::rad2deg(angle),
			axis.to_string(),
			state.to_string());

		t += 0.01;
	}
}
