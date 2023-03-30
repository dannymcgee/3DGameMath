add_executable(
	Main
		"src/main.cc"
)

target_link_libraries(
	Main
		PUBLIC
			fmt::fmt
			Sized
			Math
)

set_target_properties(
	Main PROPERTIES
		FOLDER "Apps"
)

# FIXME: Warnings for MSVC
# target_compile_options(
# 	Main
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
