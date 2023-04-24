#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>


auto main() -> int
{
	if (!glfwInit())
		return 1;

	auto* window = glfwCreateWindow(1280, 960, "Hello Triangle", nullptr, nullptr);
	if (!window)
		return 1;

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		return 1;

	fmt::print("OpenGL {}\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

	while (!glfwWindowShouldClose(window)) {
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
