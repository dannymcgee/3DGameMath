add_library(
	Sized INTERFACE
		"include/sized.h"
)

target_include_directories(
	Sized INTERFACE
		"include"
)

set_target_properties(
	Sized PROPERTIES
		LINKER_LANGUAGE CXX
		FOLDER "Libs"
)
