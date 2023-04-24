cmake_minimum_required(VERSION 3.25)

project(
	3DMathPrimer
		VERSION 0.1.0
		LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(FetchContent)


# fmt --------------------------------------------------------------------------

FetchContent_Declare(
	fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG 9.1.0
)
FetchContent_MakeAvailable(fmt)
set_target_properties(
	fmt PROPERTIES
		FOLDER "External"
)


# Catch2 -----------------------------------------------------------------------

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG v3.3.2
)
FetchContent_MakeAvailable(Catch2)
set_target_properties(
	Catch2 PROPERTIES
		FOLDER "External"
)
set_target_properties(
	Catch2WithMain PROPERTIES
		FOLDER "External"
)


# benchmark --------------------------------------------------------------------

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark tests" FORCE)
FetchContent_Declare(
	benchmark
	GIT_REPOSITORY https://github.com/google/benchmark.git
	GIT_TAG v1.7.1
)
FetchContent_MakeAvailable(benchmark)
set_target_properties(
	benchmark PROPERTIES
		FOLDER "External"
)
set_target_properties(
	benchmark_main PROPERTIES
		FOLDER "External"
)


# GLFW -------------------------------------------------------------------------

FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)
set_target_properties(
	glfw PROPERTIES
		FOLDER "External"
)


add_subdirectory("libs/Sized")
add_subdirectory("libs/Math")
add_subdirectory("apps/Sandbox")
add_subdirectory("apps/Test")
add_subdirectory("apps/Bench")
add_subdirectory("apps/Renderer")
