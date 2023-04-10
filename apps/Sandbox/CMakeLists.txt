add_executable(
	Sandbox
		"src/main.cc"
		"src/app/bbox.cc"
		"src/app/coord_spaces.cc"
		"src/app/euler.cc"
		"src/app/invalid_matrix_ctor.cc"
		"src/app/matrix_determinants.cc"
		"src/app/matrix_inversion.cc"
		"src/app/matrix_orthogonality.cc"
		"src/app/matrix_transformation.cc"
		"src/app/matrix.cc"
		"src/app/orientation_conversions.cc"
		"src/app/plane.cc"
		"src/app/polar_coords.cc"
		"src/app/quaternion.cc"
		"src/app/rotation_matrix.cc"
		"src/app/scale_matrix.cc"
		"src/app/slerp.cc"
		"src/app/vector_exercises.cc"
)

target_link_libraries(
	Sandbox
		PUBLIC
			fmt::fmt
			Sized
			Math
)

set_target_properties(
	Sandbox PROPERTIES
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
