find_package(OpenGL REQUIRED)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/external/glew-2.1.0")
add_definitions(-DGLEW_STATIC)
set(GLEW_USE_STATIC_LIBS ON)
set(GLEW_VERBOSE ON)
find_package(GLEW REQUIRED)

file(TO_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" RENDERER_SOURCE_DIR)
add_definitions(-DPROJECT_SOURCE_DIR="${RENDERER_SOURCE_DIR}")

add_executable(Renderer "src/main.cc")

target_include_directories(Renderer PRIVATE "src")

target_link_libraries(
	Renderer
		PRIVATE
			fmt::fmt
			GLEW::glew_s
			glfw
			OpenGL::GL
			Sized
			Math
)

target_compile_features(Renderer PRIVATE cxx_std_17)
set_target_properties(Renderer PROPERTIES FOLDER "Apps")
