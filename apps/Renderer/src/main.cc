#include <GLFW/glfw3.h>


auto main() -> int
{
	if (!glfwInit())
		return -1;

	// Create a windowed mode window and its OpenGL context
	auto* window = glfwCreateWindow(1920, 1080, "Hello OpenGL", nullptr, nullptr);
	if (!window) {
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
