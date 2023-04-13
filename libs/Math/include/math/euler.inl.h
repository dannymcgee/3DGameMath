#pragma once

#include <string>

#include <sized.h>

#include "math/spaces.h"


namespace math {
using namespace sized; // NOLINT(*-using-namespace)

class RotationMatrix;
struct Quat;


struct Euler {
	flt yaw = 0;
	flt pitch = 0;
	flt roll = 0;

	auto matrix(Space space = Space::Local2Parent) const -> RotationMatrix;
	auto quat(Space space = Space::Local2Parent) const -> Quat;

	constexpr auto canonical() const -> Euler;
	void canonicalize();

	auto to_string(usize precision = 3) const -> std::string;
};

} // namespace math
