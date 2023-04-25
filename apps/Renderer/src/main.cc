#include <iostream>

#include <fmt/format.h>
#include <math/vector.h>
#include <sized.h>

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

	if (!glfwInit())
		return 1;

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

	f32 positions[] {
		-1.f,  1.f,
		 1.f,  1.f,
		 1.f, -1.f,
		-1.f, -1.f,
	};
	u32 position_buffer = gl::gen_buffer();
	gl::bind_buffer(Target::Array, position_buffer);
	gl::buffer_data(Target::Array, sizeof(positions), &positions, Usage::StaticDraw);

	gl::enable_vertex_attrib_array(0);
	gl::vertex_attrib_pointer<f32>(0, 2);

	f32 colors[] {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,
		1, 0, 1,
	};
	u32 color_buffer = gl::gen_buffer();
	gl::bind_buffer(Target::Array, color_buffer);
	gl::buffer_data(Target::Array, sizeof(colors), &colors, Usage::StaticDraw);

	gl::enable_vertex_attrib_array(1);
	gl::vertex_attrib_pointer<f32>(1, 3);

	u32 indices[] { 0, 1, 2, 0, 2, 3 };
	u32 index_buffer = gl::gen_buffer();
	gl::bind_buffer(Target::ElementArray, index_buffer);
	gl::buffer_data(Target::ElementArray, sizeof(indices), &indices, Usage::StaticDraw);

	u32 program = gl::make_program(PROJECT_SOURCE_DIR"/res/shaders/hello.shader");
	gl::use_program(program);

	Vec4 u_color = { 0.2, 0.3, 0.8, 1.0 };
	i32 location = gl::get_uniform_location(program, "u_color");

	f32 increment = 0.01;
	while (!glfwWindowShouldClose(window)) {
		gl::clear(Mask::ColorBuffer);

		gl::uniform(location, u_color);
		gl::draw_elements<u32>(DrawMode::Triangles, 6, nullptr);

		if (u_color.x > 1)
			increment = -0.01;
		else if (u_color.x < 0)
			increment = 0.01;

		u_color.x += increment;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	gl::delete_program(program);
	gl::delete_buffer(position_buffer);
	gl::delete_buffer(color_buffer);
	gl::delete_buffer(index_buffer);

	glfwTerminate();

	return 0;
}
