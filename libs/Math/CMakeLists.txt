add_library(
	Math STATIC
		"include/math/assert.h"

		"include/math/euler.h"
		"include/math/euler.inl.h"
		"include/math/euler.inl.hpp"

		"include/math/fmt.h"

		"include/math/geo/aabb.h"
		"include/math/geo/circle.h"
		"include/math/geo/plane.h"
		"include/math/geo/ray.h"
		"include/math/geo/sphere.h"
		"include/math/geo/tri.h"

		"include/math/literals.h"

		"include/math/matrix.h"
		"include/math/matrix.inl.h"
		"include/math/matrix.inl.hpp"

			"include/math/matrix/rotation.h"
			"include/math/matrix/rotation.inl.h"
			"include/math/matrix/rotation.inl.hpp"

			"include/math/matrix/scale.h"
			"src/math/matrix/scale.cc"

			"include/math/matrix/transform.h"
			"include/math/matrix/transform.inl.h"
			"include/math/matrix/transform.inl.hpp"

			"include/math/matrix/translation.h"
			"src/math/matrix/translation.cc"

		"include/math/polar.h"
		"include/math/polar.inl.h"
		"include/math/polar.inl.hpp"

		"include/math/quat.h"
		"include/math/quat.inl.h"
		"include/math/quat.inl.hpp"

		"include/math/sfinae.h"
		"include/math/spaces.h"

		"include/math/utility.h"

		"include/math/vector.h"
		"include/math/vector.inl.h"
		"include/math/vector.inl.hpp"
)

target_include_directories(
	Math
		PUBLIC "include"
		PRIVATE "src"
)

set_target_properties(
	Math PROPERTIES
		LINKER_LANGUAGE CXX
		FOLDER "Libs"
)

# FIXME: Support warnings for MSVC
# target_compile_options(
# 	Math
# 		INTERFACE
# 			-Wunused
# 			-Wcast-align
# 			-Wconversion
# 			-Wnull-dereference
# 			-Wnon-virtual-dtor
# 			-Woverloaded-virtual
# 			-Weffc++
# 			-Wno-missing-braces
# )

target_link_libraries(
	Math PUBLIC
		fmt::fmt
		Sized
)
