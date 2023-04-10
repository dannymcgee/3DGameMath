#pragma once

#include "math/_euler_decl.h"

#include "math/matrix/rotation.h"
#include "math/quat.h"
#include "math/utility.h"

namespace math {

template <typename T>
inline auto Euler<T>::matrix(Space space) const -> RotationMatrix<T>
{
	T cy = std::cos(yaw);
	T sy = std::sin(yaw);

	T cp = std::cos(pitch);
	T sp = std::sin(pitch);

	T cr = std::cos(roll);
	T sr = std::sin(roll);

	switch (space) {
		case Space::Local2Parent:
			// NOTE: This matrix is the expanded form of:
			// 	RotationMatrix(roll, Axis::Forward)
			// 		* RotationMatrix(pitch, Axis::Right)
			// 		* RotationMatrix(yaw, Axis::Up);
			return RotationMatrix<T>{{
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
			return RotationMatrix<T>{{
				{  cy*cr + sy*sp*sr,  -cy*sr + sy*sp*cr,   sy*cp },
				{  sr*cp,              cr*cp,             -sp    },
				{ -sy*cr + cy*sp*sr,   sr*sy + cy*sp*cr,   cy*cp },
			}};
	}

}

template <typename T>
inline auto Euler<T>::quat(Space space) const -> Quat<T>
{
	auto yq = Quat<T>::angle_axis(yaw, Vec3::up());
	auto pq = Quat<T>::angle_axis(pitch, Vec3::right());
	auto rq = Quat<T>::angle_axis(roll, Vec3::forward());

	Quat<T> result = yq * pq * rq;

	switch (space) {
		case Space::Local2Parent:
			return result;

		case Space::Parent2Local:
			return result.inverse();
	}
}

template <typename T>
constexpr auto Euler<T>::canonical() const -> Euler
{
	// TODO
}

template <typename T>
inline void Euler<T>::canonicalize()
{
	// TODO
}

template <typename T>
inline auto Euler<T>::to_string(usize precision) const -> std::string
{
	return ::fmt::format(
		"( yaw {0:+.{3}}°, pitch {1:+.{3}}°, roll {2:+.{3}}° )",
		math::rad2deg(yaw),
		math::rad2deg(pitch),
		math::rad2deg(roll),
		precision);
}

} // namespace math
