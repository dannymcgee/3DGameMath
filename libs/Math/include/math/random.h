#pragma once

#include <random>

#include <sized.h>

namespace math {
using namespace sized; // NOLINT(*-using-namespace)

template <typename T = flt>
class Random {
public:
	Random(T range_start, T range_end)
		: m_dist(range_start, range_end)
	{}

	auto get() -> T
	{
		return m_dist(m_device);
	}

private:
	std::random_device m_device;
	std::uniform_real_distribution<T> m_dist;
};

} // namespace math
