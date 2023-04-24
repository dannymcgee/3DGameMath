#include <GLFW/glfw3.h>


auto main() -> int
{
	if (!glfwInit())
		return -1;

	// Create a windowed mode window and its OpenGL context
	auto* window = glfwCreateWindow(1280, 960, "Hello Triangle", nullptr, nullptr);
	if (!window) {
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2f( -0.5, -0.5 );
		glVertex2f(  0.0,  0.5 );
		glVertex2f(  0.5, -0.5 );
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
