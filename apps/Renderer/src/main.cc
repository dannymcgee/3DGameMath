#include <iostream>

#include <fmt/format.h>
#include <sized.h>

#include "gl.hpp"

const auto vert_src = R"(
#version 330 core

layout(location = 0)
in vec4 position;

void main()
{
	gl_Position = position;
}
)";

const auto frag_src = R"(
#version 330 core

layout(location = 0)
out vec4 color;

void main()
{
	color = vec4(1, 0, 0, 1);
}
)";

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

	if (!glfwInit())
		return 1;

	auto* window = glfwCreateWindow(1280, 960, "Hello Triangle", nullptr, nullptr);
	if (!window)
		return 1;

	glfwMakeContextCurrent(window);

	auto result = gl::init();
	if (result != gl::Result::OK) {
		std::cerr << fmt::format("Error: {}\n", gl::get_error_string(result));
		return 1;
	}

	fmt::print("OpenGL {}\n", gl::get_string(Info::Version));

	f32 positions[] {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f,
	};

	u32 position_buffer = gl::gen_buffer();
	gl::bind_buffer(Target::Array, position_buffer);
	gl::buffer_data(Target::Array, sizeof(positions), &positions, Usage::StaticDraw);

	gl::enable_vertex_attrib_array(0);
	gl::vertex_attrib_pointer(0, {
		.type = Scalar::f32,
		.size = 2,
	});

	u32 vert_shader = gl::compile_shader(Shader::Vertex, vert_src);
	u32 frag_shader = gl::compile_shader(Shader::Fragment, frag_src);
	u32 program = gl::link_program(vert_shader, frag_shader);

	gl::use_program(program);

	while (!glfwWindowShouldClose(window)) {
		gl::clear(Mask::ColorBuffer);
		gl::draw_arrays(DrawMode::Triangles, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	gl::delete_program(program);
	gl::delete_buffer(position_buffer);

	glfwTerminate();

	return 0;
}
