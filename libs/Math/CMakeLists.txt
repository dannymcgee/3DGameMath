add_library(
	Math STATIC
		"include/math/fmt.h"
		"include/math/_matrix_decl.h"
		"include/math/_matrix_impl.hpp"
		"include/math/matrix.h"
		"include/math/matrix/rotation.h"
		"include/math/matrix/scale.h"
		"include/math/matrix/translation.h"
		"include/math/utility.h"
		"include/math/_vector_decl.h"
		"include/math/_vector_impl.hpp"
		"include/math/vector.h"
		"src/math/matrix/rotation.cc"
		"src/math/matrix/scale.cc"
		"src/math/matrix/translation.cc"
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
