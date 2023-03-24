add_executable(
	Main
		"src/main.cc"
)

target_link_libraries(
	Main
		PUBLIC
			Sized
)
