add_executable(Renderer "src/main.cc")

target_link_libraries(
	Renderer
		PRIVATE
			fmt::fmt
			Sized
			Math
)

target_compile_features(Renderer PRIVATE cxx_std_17)
set_target_properties(Renderer PROPERTIES FOLDER "Apps")
