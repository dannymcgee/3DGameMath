#include <fmt/format.h>

#include <math/euler.h>
#include <math/literals.h>
#include <math/matrix.h>
#include <math/matrix/rotation.h>
#include <math/matrix/translation.h>
#include <math/quat.h>
#include <math/spaces.h>
#include <math/vector.h>
#include <sized.h>

using namespace math::literals; // NOLINT(*-using-namespace)
using namespace sized; // NOLINT(*-using-namespace)
using math::Axis;
using math::Euler;
using math::Mat4x4;
using math::Quat;
using math::RotationMatrix;
using math::Space;
using math::Vec3;

void matrix(const Euler<f64>& euler)
{
	auto l2p_conv = euler.matrix(Space::Local2Parent);
	auto p2l_conv = euler.matrix(Space::Parent2Local);

	auto yaw_mat   = RotationMatrix(45_deg, Axis::Up);
	auto pitch_mat = RotationMatrix(-15_deg, Axis::Right);
	auto roll_mat  = RotationMatrix(3.3_deg, Axis::Forward);

	auto l2p_comp = roll_mat * pitch_mat * yaw_mat;
	auto p2l_comp = (roll_mat * pitch_mat * yaw_mat).inverse();

	fmt::print("Local->Parent Converted:\n{}\n", l2p_conv.to_string());
	fmt::print("Local->Parent Composed:\n{}\n\n", l2p_comp.to_string());

	fmt::print("Parent->Local Converted:\n{}\n", p2l_conv.to_string());
	fmt::print("Parent->Local Composed:\n{}\n\n", p2l_comp.to_string());
}

auto expand_quat_p2l(const Euler<f64>& euler) -> Quat<f64>
{
	f64 cos_h2 = std::cos(euler.yaw * 0.5);
	f64 cos_p2 = std::cos(euler.pitch * 0.5);
	f64 cos_b2 = std::cos(euler.roll * 0.5);

	f64 sin_h2 = std::sin(euler.yaw * 0.5);
	f64 sin_p2 = std::sin(euler.pitch * 0.5);
	f64 sin_b2 = std::sin(euler.roll * 0.5);

	f64 w =  cos_h2*cos_p2*cos_b2 + sin_h2*sin_p2*sin_b2;
	f64 x = -cos_h2*sin_p2*cos_b2 - sin_h2*cos_p2*sin_b2;
	f64 y =  cos_h2*sin_p2*sin_b2 - sin_h2*cos_p2*cos_b2;
	f64 z =  sin_h2*sin_p2*cos_b2 - cos_h2*cos_p2*sin_b2;

	return Quat<f64>{ w, x, y, z };
}

void quat(const Euler<f64>& euler)
{
	auto l2p_conv = euler.quat(Space::Local2Parent);
	auto p2l_conv = euler.quat(Space::Parent2Local);

	auto yaw_quat   = Quat<f64>::angle_axis(45_deg, Vec3::up());
	auto pitch_quat = Quat<f64>::angle_axis(-15_deg, Vec3::right());
	auto roll_quat  = Quat<f64>::angle_axis(3.3_deg, Vec3::forward());

	auto l2p_comp = yaw_quat * pitch_quat * roll_quat;
	auto p2l_comp = (yaw_quat * pitch_quat * roll_quat).conjugate();
	auto p2l_expn = expand_quat_p2l(euler);

	auto l2p_from_comp = l2p_comp.euler(Space::Local2Parent);
	auto l2p_from_conv = l2p_conv.euler(Space::Local2Parent);

	auto p2l_from_comp = p2l_comp.euler(Space::Parent2Local);
	auto p2l_from_conv = p2l_conv.euler(Space::Parent2Local);
	auto p2l_from_expn = p2l_expn.euler(Space::Parent2Local);

	fmt::print("Local->Parent Converted: {}\n", l2p_conv.to_string());
	fmt::print("Local->Parent Composed:  {}\n\n", l2p_comp.to_string());

	fmt::print("Parent->Local Converted: {}\n", p2l_conv.to_string());
	fmt::print("Parent->Local Composed:  {}\n", p2l_comp.to_string());
	fmt::print("Parent->Local Expanded:  {}\n\n", p2l_expn.to_string());

	fmt::print("Local->Parent Quat (converted) -> Euler: {}\n", l2p_from_conv.to_string());
	fmt::print("Local->Parent Quat (composed)  -> Euler: {}\n\n", l2p_from_comp.to_string());

	fmt::print("Parent->Local Quat (converted) -> Euler: {}\n", p2l_from_conv.to_string());
	fmt::print("Parent->Local Quat (composed)  -> Euler: {}\n", p2l_from_comp.to_string());
	fmt::print("Parent->Local Quat (expanded)  -> Euler: {}\n\n", p2l_from_expn.to_string());
}

void orientation_conversions()
{
	auto euler = Euler<f64>{
		45_deg,
		-15_deg,
		3.3_deg,
	};

	// matrix(euler);
	quat(euler);
}
