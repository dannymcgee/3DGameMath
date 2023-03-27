cmake_minimum_required(VERSION 3.22)

project(
	3DMathPrimer
		VERSION 0.1.0
		LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(FetchContent)

FetchContent_Declare(
	fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG 9.1.0
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG v3.3.2
)
FetchContent_MakeAvailable(Catch2)

add_subdirectory("libs/Sized")
add_subdirectory("libs/Math")
add_subdirectory("apps/Main")
add_subdirectory("apps/Test")
