add_library(
	Math
		INTERFACE
			"include/math/vector.h"
)

include_directories("./include")
target_include_directories(Math INTERFACE "./include")

target_link_libraries(
	Math
		INTERFACE Sized fmt::fmt
)

set_target_properties(
	Math
		PROPERTIES
			LINKER_LANGUAGE CXX
)
