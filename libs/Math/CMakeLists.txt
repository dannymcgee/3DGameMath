add_library(
	Math STATIC
		"include/math/assert.h"

		"include/math/euler.h"
		"include/math/_euler_decl.h"
		"include/math/_euler_impl.hpp"

		"include/math/fmt.h"
		"include/math/literals.h"

		"include/math/matrix.h"
		"include/math/_matrix_decl.h"
		"include/math/_matrix_impl.hpp"

			"include/math/matrix/rotation.h"
			"include/math/matrix/_rotation_decl.h"
			"include/math/matrix/_rotation_impl.hpp"

			"include/math/matrix/scale.h"
			"src/math/matrix/scale.cc"

			"include/math/matrix/translation.h"
			"src/math/matrix/translation.cc"

		"include/math/polar.h"
		"include/math/_polar_decl.h"
		"include/math/_polar_impl.hpp"

		"include/math/quat.h"
		"include/math/_quat_decl.h"
		"include/math/_quat_impl.hpp"

		"include/math/spaces.h"

		"include/math/utility.h"

		"include/math/vector.h"
		"include/math/_vector_decl.h"
		"include/math/_vector_impl.hpp"
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
