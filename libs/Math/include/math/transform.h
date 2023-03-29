#pragma once

#include <sized.h>

#include "math/vector.h"

namespace math {

using namespace sized; // NOLINT(*-using-namespace)

using Vec3 = Vec<3, f64>;

enum class CoordSpace : u8 {
	World = 0,
	Body,
};

class Transform {
public:
	auto local_to_world_pos(const Vec3& pos) const -> Vec3
	{
		return m_origin
			+ pos.x * m_up
			+ pos.y * m_right
			+ pos.z * m_forward;
	}

private:
	Vec3 m_origin  { 0, 0, 0 };
	Vec3 m_up      { 1, 0, 0 };
	Vec3 m_right   { 0, 1, 0 };
	Vec3 m_forward { 0, 0, 1 };
};

}
