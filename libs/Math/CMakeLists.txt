add_library(
	Math INTERFACE
		"include/math/fmt.h"
		"include/math/matrix.h"
		"include/math/matrix/rotation.h"
		"include/math/matrix/scale.h"
		"include/math/utility.h"
		"include/math/vector.h"
)

target_include_directories(
	Math INTERFACE
		"include"
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
	Math INTERFACE
		fmt::fmt
		Sized
)
