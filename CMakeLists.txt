cmake_minimum_required(VERSION 3.22)

project(
	CppProjectTemplate
		VERSION 0.1.0
		LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(FetchContent)

add_subdirectory("libs/Sized")
add_subdirectory("apps/Main")
