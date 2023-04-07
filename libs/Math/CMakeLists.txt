add_library(
	Math STATIC
		"include/math/matrix.h"
		"include/math/_matrix_decl.h"
		"include/math/_matrix_impl.hpp"

			"include/math/matrix/rotation.h"
			"src/math/matrix/rotation.cc"

			"include/math/matrix/scale.h"
			"src/math/matrix/scale.cc"

			"include/math/matrix/translation.h"
			"src/math/matrix/translation.cc"

		"include/math/polar.h"
		"include/math/_polar_decl.h"
		"include/math/_polar_impl.hpp"

		"include/math/vector.h"
		"include/math/_vector_decl.h"
		"include/math/_vector_impl.hpp"

		"include/math/literals.h"
		"src/math/literals.cc"

		"include/math/assert.h"
		"include/math/fmt.h"
		"include/math/utility.h"
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
