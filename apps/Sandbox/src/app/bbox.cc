#include <vector>

#include <fmt/format.h>

#include <math/geo/aabb.h>
#include <math/utility.h>
#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT
using math::geo::AABBox;
using math::Vec3;

void bbox()
{
	auto points = std::vector<Vec3>{
		Vec3{   1,   2,  3 },
		Vec3{   3,   2,  1 },
		Vec3{   5,   5,  5 },
		Vec3{ -10,   0,  4 },
		Vec3{   0, -20, -2.333333 },
	};

	auto begin = points.front().begin();
	auto end = points.back().end();
	auto formatter = math::fmt::AlignedValues(begin, end, 3);
	formatter.debug();

	auto bbox = AABBox::empty().add(points);

	fmt::print("min:    {}\n", bbox.min.to_string(formatter));
	fmt::print("max:    {}\n\n", bbox.max.to_string(formatter));

	fmt::print("center: {}\n", bbox.center().to_string(formatter));
	fmt::print("size:   {}\n", bbox.size().to_string(formatter));
	fmt::print("radius: {}\n", bbox.radius().to_string(formatter));
}
