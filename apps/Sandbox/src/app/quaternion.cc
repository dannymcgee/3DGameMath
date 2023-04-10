#include <array>

#include <fmt/format.h>

#include <math/literals.h>
#include <math/matrix/rotation.h>
#include <math/quat.h>
#include <math/utility.h>
#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT(*-using-namespace)
using namespace math::literals; // NOLINT(*-using-namespace)
using math::Vec3;
using math::Quat;
using math::RotationMatrix;

void quaternion()
{
	auto axis = Vec3{ -0.25, 0.5, 0.33 }.normal();
	auto q = Quat<f64>::angle_axis(45_deg, axis);
	fmt::print("Original: 45°, {}\n", axis.to_string());
	{
		auto [angle,axis] = q.angle_axis();
		fmt::print("Extracted: {:.3}°, {}\n\n", math::rad2deg(angle), axis.to_string());
	}

	auto mat = RotationMatrix(45_deg, axis);
	auto q2 = mat.quat();
	fmt::print("q1: {}\n", q.to_string());
	fmt::print("q2: {}\n\n", q2.to_string());

	fmt::print(" q: {}\n", q.to_string());
	fmt::print("-q: {}\n\n", (-q).to_string());

	auto three60 = Quat<f64>::angle_axis(360_deg, Vec3::up());
	auto five40 = Quat<f64>::angle_axis(540_deg, Vec3::up());
	auto seven20 = Quat<f64>::angle_axis(720_deg, Vec3::up());
	auto nine00 = Quat<f64>::angle_axis(900_deg, Vec3::up());
	auto ten80 = Quat<f64>::angle_axis(1080_deg, Vec3::up());

	std::array<f64, 5_usize * 4_usize> values; // NOLINT
	for (usize i = 0; i < 5; ++i) {
		Quat<f64>* ptr = nullptr;
		switch (i) {
			case 0: ptr = &three60; break;
			case 1: ptr = &five40; break;
			case 2: ptr = &seven20; break;
			case 3: ptr = &nine00; break;
			case 4: ptr = &ten80; break;
		}
		auto q = *ptr;

		std::copy(q.begin(), q.end(), &values[i * 4]); // NOLINT
	}

	auto formatter = math::fmt::AlignedValues(values.begin(), values.end(), 3);

	// auto begin = math::detail::RawConstIterator(&three60.x);
	// auto end = math::detail::RawConstIterator(&ten80.w); // NOLINT
	// auto formatter = math::fmt::AlignedValues(begin, end, 3);

	fmt::print(" 360°: {}\n", three60.to_string(formatter));
	fmt::print(" 540°: {}\n", five40.to_string(formatter));
	fmt::print(" 720°: {}\n", seven20.to_string(formatter));
	fmt::print(" 900°: {}\n", nine00.to_string(formatter));
	fmt::print("1080°: {}\n", ten80.to_string(formatter));

	fmt::print("magnitudes: {}, {}, {}, {}, {}, {}, {}\n\n",
		q.magnitude(),
		(-q).magnitude(),
		three60.magnitude(),
		five40.magnitude(),
		seven20.magnitude(),
		nine00.magnitude(),
		ten80.magnitude());

	// Quat uninit;
	// fmt::print("uninit: {}\n", uninit.to_string(20));

	auto id = Quat<f64>::identity();
	fmt::print("identity: {}\n\n", id.to_string());

	auto slerp = Quat<f64>::slerp(three60, five40, 0.5);
	fmt::print("slerp1(360°, 540°, 0.5): {}\n", slerp.to_string());
	auto [angle1,axis1] = slerp.angle_axis();
	fmt::print("slerp1 angle/axis: {}°, {}\n\n", math::rad2deg(angle1), axis1.to_string());

	auto slerp2 = (three60 - five40).pow(0.5) * three60;
	fmt::print("slerp2(360°, 540°, 0.5): {}\n", slerp2.to_string());
	auto [angle2,axis2] = slerp2.angle_axis();
	fmt::print("slerp1 angle/axis: {}°, {}\n\n", math::rad2deg(angle2), axis2.to_string());
}
