#pragma once

#include <sized.h>

namespace math {
using namespace sized; // NOLINT

enum class Space : u8 {
	Local2Parent,
	Parent2Local,
};

} // namespace math
