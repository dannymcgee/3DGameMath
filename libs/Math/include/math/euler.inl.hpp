#pragma once

#include "math/euler.inl.h"

#include "math/matrix/rotation.h"
#include "math/quat.h"
#include "math/utility.h"

namespace math {

inline auto Euler::matrix(Space space) const -> RotationMatrix
{
	flt cy = std::cos(yaw);
	flt sy = std::sin(yaw);

	flt cp = std::cos(pitch);
	flt sp = std::sin(pitch);

	flt cr = std::cos(roll);
	flt sr = std::sin(roll);

	switch (space) {
		case Space::Local2Parent:
			// NOTE: This matrix is the expanded form of:
			// 	RotationMatrix(roll, Axis::Forward)
			// 		* RotationMatrix(pitch, Axis::Right)
			// 		* RotationMatrix(yaw, Axis::Up);
			return RotationMatrix{{
				{  cy*cr + sy*sp*sr,   sr*cp,  -sy*cr + cy*sp*sr },
				{ -cy*sr + sy*sp*cr,   cr*cp,   sr*sy + cy*sp*cr },
				{  sy*cp,             -sp,      cy*cp            },
			}};

		case Space::Parent2Local:
			// NOTE: This matrix is the expanded form of:
			// 	*(RotationMatrix(roll, Axis::Forward)
			// 		* RotationMatrix(pitch, Axis::Right)
			// 		* RotationMatrix(yaw, Axis::Up))
			//			.inverse();
			// (This is the transpose/inverse of the `Local2Parent` return value)
			return RotationMatrix{{
				{  cy*cr + sy*sp*sr,  -cy*sr + sy*sp*cr,   sy*cp },
				{  sr*cp,              cr*cp,             -sp    },
				{ -sy*cr + cy*sp*sr,   sr*sy + cy*sp*cr,   cy*cp },
			}};
	}

}

inline auto Euler::quat(Space space) const -> Quat
{
	auto yq = Quat::angle_axis(yaw, Vec3::up());
	auto pq = Quat::angle_axis(pitch, Vec3::right());
	auto rq = Quat::angle_axis(roll, Vec3::forward());

	Quat result = yq * pq * rq;

	switch (space) {
		case Space::Local2Parent:
			return result;

		case Space::Parent2Local:
			return result.inverse();
	}
}

constexpr auto Euler::canonical() const -> Euler
{
	// TODO
	return Euler{};
}

inline void Euler::canonicalize()
{
	// TODO
}

inline auto Euler::to_string(usize precision) const -> std::string
{
	return ::fmt::format(
		"( yaw {0:+.{3}}°, pitch {1:+.{3}}°, roll {2:+.{3}}° )",
		math::rad2deg(yaw),
		math::rad2deg(pitch),
		math::rad2deg(roll),
		precision);
}

} // namespace math
