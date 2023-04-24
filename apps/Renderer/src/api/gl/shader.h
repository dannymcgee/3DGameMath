#pragma once

#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <fmt/format.h>
#include <sized.h>

#include "api/gl/types.h"

namespace gl {
using namespace sized;
namespace fs = std::filesystem;


inline void attach_shader(u32 program, u32 shader)
{
	glAttachShader(program, shader);
}

inline auto create_program() -> u32
{
	return glCreateProgram();
}

inline auto create_shader(Shader type) -> u32
{
	return glCreateShader(static_cast<GLenum>(type));
}

/**
 * @brief Compiles a shader object
 * @param shader Specifies the shader object to be compiled.
 * @see https://docs.gl/gl4/glCompileShader
 */
inline void compile_shader(u32 shader)
{
	glCompileShader(shader);
}

inline void delete_shader(u32 shader)
{
	glDeleteShader(shader);
}

inline void link_program(u32 program)
{
	glLinkProgram(program);
}

/**
 * @brief Replaces the source code in a shader object
 *
 * @param shader
 * Specifies the handle of the shader object whose source code is to be
 * replaced.
 *
 * @param count
 * Specifies the number of elements in the string and length arrays.
 *
 * @param sources
 * Specifies an array of pointers to strings containing the source code to be
 * loaded into the shader.
 *
 * @param length
 * Specifies an array of string lengths.
 *
 * @see https://docs.gl/gl4/glShaderSource
 */
inline void shader_source(u32 shader, i32 count, const char** sources, const i32* length)
{
	glShaderSource(shader, count, sources, length);
}

inline void shader_source(u32 shader, const char* source)
{
	gl::shader_source(shader, 1, &source, nullptr);
}

/**
 * @brief Validates a program object
 * @param program Specifies the handle of the program object to be validated.
 * @see https://docs.gl/gl4/glValidateProgram
 */
inline void validate_program(u32 program)
{
	glValidateProgram(program);
}

/**
 * @brief Returns a parameter from a shader object
 *
 * @param shader Specifies the shader object to be queried.
 * @param param Specifies the object parameter.
 * @param out_params Returns the requested object parameter.
 *
 * @see https://docs.gl/gl4/glGetShader
 */
inline void get_shader(u32 shader, ShaderParam param, i32* out_params)
{
	glGetShaderiv(shader, static_cast<GLenum>(param), out_params);
}

inline auto get_shader_compile_status(u32 shader) -> bool
{
	i32 result;
	gl::get_shader(shader, ShaderParam::CompileStatus, &result);

	return result == GL_TRUE;
}

inline auto get_shader_info_log_length(u32 shader) -> i32
{
	i32 result;
	gl::get_shader(shader, ShaderParam::InfoLogLength, &result);

	return result;
}

/**
 * @brief Returns the information log for a shader object
 *
 * @param shader
 * Specifies the shader object whose information log is to be queried.
 *
 * @param max_length
 * Specifies the size of the character buffer for storing the returned
 * information log.
 *
 * @param out_length
 * Returns the length of the string returned in `out_result` (excluding the null
 * terminator).
 *
 * @param out_result
 *
 * @see https://docs.gl/gl4/glGetShaderInfoLog
 */
inline void get_shader_info_log(u32 shader, i32 max_length, i32* out_length, char* out_result)
{
	glGetShaderInfoLog(shader, max_length, out_length, out_result);
}

/**
 * @brief Determines if a name corresponds to a shader object
 */
inline auto is_shader(u32 name) -> bool
{
	return glIsShader(name);
}

inline void use_program(u32 program)
{
	glUseProgram(program);
}

inline void delete_program(u32 program)
{
	glDeleteProgram(program);
}

inline auto shader_type_name(Shader type) -> std::string
{
	switch (type) {
		case Shader::Compute: return "compute shader";
		case Shader::Vertex: return "vertex shader";
		case Shader::TessControl: return "tesselation control shader";
		case Shader::TessEval: return "tesselation evaluation shader";
		case Shader::Geometry: return "geometry shader";
		case Shader::Fragment: return "fragment shader";
		default: return "unknown shader";
	}
}

static auto shader_type(const std::string& keyword) -> Shader
{
	if (keyword == "vertex") return Shader::Vertex;
	if (keyword == "fragment") return Shader::Fragment;
	if (keyword == "compute") return Shader::Compute;
	if (keyword == "tessc") return Shader::TessControl;
	if (keyword == "tesse") return Shader::TessEval;
	if (keyword == "geometry") return Shader::Geometry;
	return Shader::_none;
}

inline auto parse_shaders(const fs::path& file_path) -> std::unordered_map<Shader, std::string>
{
	std::unordered_map<Shader, std::string> result;
	const auto shader_block_pattern = std::regex(R"(#shader (\S+))");
	auto file = std::ifstream(file_path);

	std::string line;
	std::smatch matches;

	auto current_type = Shader::_none;
	std::string current_src;

	while (std::getline(file, line)) {
		if (std::regex_match(line, matches, shader_block_pattern)) {
			if (!current_src.empty() && current_type != Shader::_none)
				result.emplace(current_type, current_src);

			current_type = gl::shader_type(matches.str(1));
			current_src.clear();
		}
		else if (current_type != Shader::_none) {
			current_src.append(line);
			current_src.push_back('\n');
		}
	}

	if (!current_src.empty() && current_type != Shader::_none)
		result.emplace(current_type, current_src);

	return result;
}

inline auto compile_shader(Shader type, const char* source) -> u32
{
	u32 shader = gl::create_shader(type);

	gl::shader_source(shader, source);
	gl::compile_shader(shader);

	if (!gl::get_shader_compile_status(shader)) {
		const i32 log_length = gl::get_shader_info_log_length(shader);
		char message_buffer[log_length];
		char* message = &message_buffer[0];
		gl::get_shader_info_log(shader, log_length, nullptr, message);

		fmt::print("\nError compiling {}:\n{}\n{}\n",
			gl::shader_type_name(type),
			std::string(40, '-'),
			message);
	}

	return shader;
}

inline auto compile_shader(Shader type, const std::string& source) -> u32
{
	return compile_shader(type, source.c_str());
}

inline auto make_program(const fs::path& shader_path) -> u32
{
	u32 program = gl::create_program();
	auto sources = gl::parse_shaders(shader_path);

	std::vector<u32> shaders;
	shaders.reserve(sources.size());

	for (auto [type, source] : sources) {
		u32 shader = gl::compile_shader(type, source);
		gl::attach_shader(program, shader);
		shaders.push_back(shader);
	}

	gl::link_program(program);
	// TODO: Handle link errors
	gl::validate_program(program);
	// TODO: Handle validation errors?

	for (u32 shader : shaders)
		gl::delete_shader(shader);

	return program;
}

template <typename... Args>
inline auto link_program(Args... shaders) -> u32
{
	u32 program = gl::create_program();

	for (auto shader : { shaders... })
		gl::attach_shader(program, shader);

	gl::link_program(program);
	// TODO: Handle link errors
	gl::validate_program(program);

	for (auto shader : { shaders... })
		gl::delete_shader(shader);

	return program;
}

} // namespace gl
