add_executable(
	Bench
		"src/bench.cc"
)

target_link_libraries(
	Bench
		PUBLIC
			Sized
			Math
			benchmark::benchmark
)

set_target_properties(
	Bench PROPERTIES
		FOLDER "Apps"
)

# FIXME: Warnings for MSVC
# target_compile_options(
# 	Bench
# 		PRIVATE
# 			-Wunused
# 			-Wcast-align
# 			-Wconversion
# 			-Wnull-dereference
# 			-Wnon-virtual-dtor
# 			-Woverloaded-virtual
# 			-Weffc++
# 			-Wno-missing-braces
# 			-Wno-unused-variable
# )
