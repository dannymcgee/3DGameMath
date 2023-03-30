#pragma once

#include <limits>
#include <sized.h>

namespace math {

using namespace sized; // NOLINT(*-using-namespace)

template <typename T = f64>
inline auto nearly_equal(T lhs, T rhs, T tolerance = std::numeric_limits<T>::epsilon())
{
	return std::abs(lhs - rhs) < tolerance;
}

}
