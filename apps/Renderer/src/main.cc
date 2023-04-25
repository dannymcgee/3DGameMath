#include <iostream>

#include <fmt/format.h>
#include <math/vector.h>
#include <sized.h>

#include "api/gl/attributes.h"
#include "api/gl/gl.h"


auto main() -> int
{
	using namespace sized;
	using gl::DrawMode;
	using gl::Info;
	using gl::Mask;
	using gl::Scalar;
	using gl::Shader;
	using gl::Target;
	using gl::Usage;
	using math::Vec4;


	// Initialize the window and OpenGL context

	if (!glfwInit())
		return 1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto* window = glfwCreateWindow(1280, 960, "Hello Triangle", nullptr, nullptr);
	if (!window)
		return 1;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	auto result = gl::init();
	if (result != gl::Result::OK) {
		std::cerr << fmt::format("Error: {}\n", gl::get_error_string(result));
		return 1;
	}

	fmt::print("OpenGL {}\n", gl::get_string(Info::Version));

	// Create vertex array
	u32 vert_array = gl::gen_vertex_array();
	gl::bind_vertex_array(vert_array);

	// Set vertex positions
	f32 positions[] {
		-1.f,  1.f,
		 1.f,  1.f,
		 1.f, -1.f,
		-1.f, -1.f,
	};
	u32 vertex_buffer = gl::gen_buffer();
	gl::bind_buffer(Target::Array, vertex_buffer);
	gl::buffer_data(Target::Array, sizeof(positions), &positions, Usage::StaticDraw);
	gl::enable_vertex_attrib_array(0);
	gl::vertex_attrib_pointer<f32>(0, 2);

	// Setup index buffer
	u32 indices[] { 0, 1, 2, 0, 2, 3 };
	u32 index_buffer = gl::gen_buffer();
	gl::bind_buffer(Target::ElementArray, index_buffer);
	gl::buffer_data(Target::ElementArray, sizeof(indices), &indices, Usage::StaticDraw);

	// Compile and link the program
	u32 program = gl::make_program(PROJECT_SOURCE_DIR"/res/shaders/hello.shader");
	gl::use_program(program);

	// Unbind buffers
	gl::use_program(0);
	gl::bind_vertex_array(0);
	gl::bind_buffer(Target::Array, 0);
	gl::bind_buffer(Target::ElementArray, 0);

	// Setup our color-shifting uniform
	Vec4 u_color = { 0.2, 0.3, 0.8, 1.0 };
	i32 location = gl::get_uniform_location(program, "u_color");
	f32 increment = 0.01;

	// Run render loop
	while (!glfwWindowShouldClose(window)) {
		gl::clear(Mask::ColorBuffer);

		// Bind the shader program
		gl::use_program(program);
		gl::uniform(location, u_color);

		// Bind vertex buffer
		// gl::bind_buffer(Target::Array, vertex_buffer);
		// gl::enable_vertex_attrib_array(0);
		// gl::vertex_attrib_pointer<f32>(0, 2);

		// Bind vertex array
		gl::bind_vertex_array(vert_array);

		// Bind and draw index buffer
		gl::bind_buffer(Target::ElementArray, index_buffer);
		gl::draw_elements<u32>(DrawMode::Triangles, 6, nullptr);

		// Cycle the uniform color
		if (u_color.x > 1)
			increment = -0.01;
		else if (u_color.x < 0)
			increment = 0.01;

		u_color.x += increment;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	gl::delete_program(program);
	gl::delete_buffer(vertex_buffer);
	gl::delete_buffer(index_buffer);

	glfwTerminate();

	return 0;
}
