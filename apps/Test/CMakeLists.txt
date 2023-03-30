add_executable(
	Test
		"src/test.cc"
)

target_link_libraries(
	Test
		PRIVATE
			Sized
			Math
			fmt::fmt
			Catch2::Catch2WithMain
)

set_target_properties(
	Test PROPERTIES
		FOLDER "Apps"
)

# FIXME: Warnings for MSVC
# target_compile_options(
# 	Test
# 		PRIVATE
# 			-Wunused
# 			-Wcast-align
# 			-Wconversion
# 			-Wnull-dereference
# 			-Wnon-virtual-dtor
# 			-Woverloaded-virtual
# 			-Weffc++
# 			-Wno-missing-braces
# )
