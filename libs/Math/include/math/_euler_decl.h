#pragma once

#include <string>

#include <sized.h>

#include "math/spaces.h"


namespace math {
using namespace sized; // NOLINT(*-using-namespace)

template <typename T> class RotationMatrix;
template <typename T> struct Quat;


template <typename T = f64>
struct Euler {
	T yaw = 0;
	T pitch = 0;
	T roll = 0;

	auto matrix(Space space = Space::Local2Parent) const -> RotationMatrix<T>;
	auto quat(Space space = Space::Local2Parent) const -> Quat<T>;

	constexpr auto canonical() const -> Euler;
	void canonicalize();

	auto to_string(usize precision = 3) const -> std::string;
};

} // namespace math
