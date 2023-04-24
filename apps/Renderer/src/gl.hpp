#pragma once

#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <sized.h>


/**
 * A fairly pointless wrapper around the OpenGL API with inline docs, distinct
 * enums, and some paper-thin syntactic conveniences.
 */
namespace gl {
using namespace sized;
namespace fs = std::filesystem;

enum class Target : GLenum {
	/** Vertex attributes */
	Array = GL_ARRAY_BUFFER,
	/** Atomic counter storage */
	AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,
	/** Buffer copy source */
	CopyRead = GL_COPY_READ_BUFFER,
	/** Buffer copy destination */
	CopyWrite = GL_COPY_WRITE_BUFFER,
	/** Indirect compute dispatch commands */
	DispatchIndirect = GL_DISPATCH_INDIRECT_BUFFER,
	/** Indirect command arguments */
	DrawIndirect = GL_DRAW_INDIRECT_BUFFER,
	/** Vertex array indices */
	ElementArray = GL_ELEMENT_ARRAY_BUFFER,
	/** Pixel read target */
	PixelPack = GL_PIXEL_PACK_BUFFER,
	/** Texture data source */
	PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
	/** Query result buffer */
	Query = GL_QUERY_BUFFER,
	/** Read-write storage for shaders */
	ShaderStorage = GL_SHADER_STORAGE_BUFFER,
	/** Texture data buffer */
	Texture = GL_TEXTURE_BUFFER,
	/** Transform feedback buffer */
	TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
	/** Uniform block storage */
	Uniform = GL_UNIFORM_BUFFER,
};

enum class Usage : GLenum {
	/**
	 * The data store contents will be modified once and used at most a few
	 * times. The data store contents are modified by the application, and used
	 * as the source for GL drawing and image specification commands.
	 */
	StreamDraw = GL_STREAM_DRAW,
	/**
	 * The data store contents will be modified once and used at most a few
	 * times. The data store contents are modified by reading data from the GL,
	 * and used to return that data when queried by the application.
	 */
	StreamRead = GL_STREAM_READ,
	/**
	 * The data store contents will be modified once and used at most a few
	 * times. The data store contents are modified by reading data from the GL,
	 * and used as the source for GL drawing and image specification commands.
	 */
	StreamCopy = GL_STREAM_COPY,

	/**
	 * The data store contents will be modified once and used many times. The
	 * data store contents are modified by the application, and used as the
	 * source for GL drawing and image specification commands.
	 */
	StaticDraw = GL_STATIC_DRAW,
	/**
	 * The data store contents will be modified once and used many times. The
	 * data store contents are modified by reading data from the GL, and used to
	 * return that data when queried by the application.
	 */
	StaticRead = GL_STATIC_READ,
	/**
	 * The data store contents will be modified once and used many times. The
	 * data store contents are modified by reading data from the GL, and used as
	 * the source for GL drawing and image specification commands.
	 */
	StaticCopy = GL_STATIC_COPY,

	/**
	 * The data store contents will be modified repeatedly and used many times.
	 * The data store contents are modified by the application, and used as the
	 * source for GL drawing and image specification commands.
	 */
	DynamicDraw = GL_DYNAMIC_DRAW,
	/**
	 * The data store contents will be modified repeatedly and used many times.
	 * The data store contents are modified by reading data from the GL, and
	 * used to return that data when queried by the application.
	 */
	DynamicRead = GL_DYNAMIC_READ,
	/**
	 * The data store contents will be modified repeatedly and used many times.
	 * The data store contents are modified by reading data from the GL, and
	 * used as the source for GL drawing and image specification commands.
	 */
	DynamicCopy = GL_DYNAMIC_COPY,
};

class Mask {
public:
	enum Bits : u32 {
		ColorBuffer = GL_COLOR_BUFFER_BIT,
		DepthBuffer = GL_DEPTH_BUFFER_BIT,
		StencilBuffer = GL_STENCIL_BUFFER_BIT,
	};
};

enum class DrawMode : GLenum {
	Points = GL_POINTS,
	LineStrip = GL_LINE_STRIP,
	LineLoop = GL_LINE_LOOP,
	Lines = GL_LINES,
	LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
	LinesAdjacency = GL_LINES_ADJACENCY,
	TriangleStrip = GL_TRIANGLE_STRIP,
	TriangleFan = GL_TRIANGLE_FAN,
	Triangles = GL_TRIANGLES,
	TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
	TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
	Patches = GL_PATCHES,
};

enum class Info : GLenum {
	Vendor = GL_VENDOR,
	Renderer = GL_RENDERER,
	Version = GL_VERSION,
	ShadingLanguageVersion = GL_SHADING_LANGUAGE_VERSION,
	Extensions = GL_EXTENSIONS,
};

enum class Scalar : GLenum {
	Byte = GL_BYTE,   UnsignedByte = GL_UNSIGNED_BYTE,
	Short = GL_SHORT, UnsignedShort = GL_UNSIGNED_SHORT,
	Int = GL_INT,     UnsignedInt = GL_UNSIGNED_INT,
	HalfFloat = GL_HALF_FLOAT,
	Float = GL_FLOAT,
	Double = GL_DOUBLE,
	Fixed = GL_FIXED,
	Int_2_10_10_10_Rev = GL_INT_2_10_10_10_REV,
	UnsignedInt_2_10_10_10_Rev = GL_UNSIGNED_INT_2_10_10_10_REV,
	UnsignedInt_10f_11f_11f_Rev = GL_UNSIGNED_INT_10F_11F_11F_REV,

	i8 = Byte,   u8 = UnsignedByte,
	i16 = Short, u16 = UnsignedShort,
	i32 = Int,   u32 = UnsignedInt,
	f16 = HalfFloat,
	f32 = Float,
	f64 = Double,
	fixed = Fixed,
};

enum class Shader : GLenum {
	_none = 0,
	Compute = GL_COMPUTE_SHADER,
	Vertex = GL_VERTEX_SHADER,
	TessControl = GL_TESS_CONTROL_SHADER,
	TessEval = GL_TESS_EVALUATION_SHADER,
	Geometry = GL_GEOMETRY_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
};

enum class ShaderParam : GLenum {
	Type = GL_SHADER_TYPE,
	DeleteStatus = GL_DELETE_STATUS,
	CompileStatus = GL_COMPILE_STATUS,
	InfoLogLength = GL_INFO_LOG_LENGTH,
	SourceLength = GL_SHADER_SOURCE_LENGTH,
};

inline auto shader_type_name(Shader type) -> std::string {
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

/** @see `gen_buffers` */
inline auto gen_buffer() -> u32 {
	u32 result;
	glGenBuffers(1, &result);

	return result;
}

/**
 * @brief generate buffer object names
 *
 * @param n Specifies the number of buffer object names to be generated.
 *
 * @param out_buffers
 * Specifies an array in which the generated buffer objectnames are stored.
 *
 * @see https://docs.gl/gl4/glGenBuffers
 */
inline void gen_buffers(i32 n, u32* out_buffers) {
	glGenBuffers(n, out_buffers);
}

/**
 * @brief bind a named buffer object
 *
 * @param target Specifies the target to which the buffer object is bound.
 * @param buffer Specifies the name of a buffer object.
 *
 * @see https://docs.gl/gl4/glBindBuffer
 */
inline void bind_buffer(Target target, u32 buffer) {
	glBindBuffer(static_cast<GLenum>(target), buffer);
}

/**
 * @brief creates and initializes a buffer object's data store
 *
 * @param target Specifies the target to which the buffer object is bound.
 *
 * @param size
 * Specifies the size in bytes of the buffer object's new data store.
 *
 * @param data
 * Specifies a pointer to data that will be copied into the data store for
 * initialization, or `nullptr` if no data is to be copied.
 *
 * @param usage Specifies the expected usage pattern of the data store.
 *
 * @see https://docs.gl/gl4/glBufferData
 */
inline void buffer_data(Target target, i32 size, void* data, Usage usage) {
	glBufferData(static_cast<GLenum>(target), size, data, static_cast<GLenum>(usage));
}

/**
 * @brief clear buffers to preset values
 *
 * @param mask
 * Bitwise OR of masks that indicate the buffers to be cleared. Allowable values
 * are any combination of the enumerators of `gl::Mask::Bits`.
 *
 * @see https://docs.gl/gl4/glClear
 */
inline void clear(u32 mask) {
	glClear(mask);
}

/**
 * @brief render primitives from array data
 *
 * @param mode Specifies what kind of primitives to render.
 * @param first Specifies the starting index in the enabled arrays.
 * @param count Specifies the number of indices to be rendered.
 *
 * @see https://docs.gl/gl4/glDrawArrays
 */
inline void draw_arrays(DrawMode mode, i32 first, i32 count) {
	glDrawArrays(static_cast<GLenum>(mode), first, count);
}

/**
 * @brief return a string describing the current GL connection
 * @param name
 */
inline auto get_string(Info name) -> char const* {
	return reinterpret_cast<const char*>(glGetString(static_cast<GLenum>(name)));
}

/**
 * @brief return a string describing the current GL connection
 * @param name
 * @param index Specifies the index of the string to return.
 */
inline auto get_string(Info name, u32 index) -> char const* {
	return reinterpret_cast<const char*>(glGetStringi(static_cast<GLenum>(name), index));
}

struct VertexAttribParams {
	static constexpr i32 k_BGRA = GL_BGRA;
	/**
	 * Specifies the data type of each component in the array. The initial value
	 * is `Scalar::Float`.
	 */
	Scalar type = Scalar::Float;
	/**
	 * Specifies the number of components per generic vertex attribute. Must be
	 * 1, 2, 3, 4, or `VertexAttribParams::k_BGRA`. The initial value is 4.
	 */
	i32 size = 4;
	/**
	 * specifies whether fixed-point data values should be normalized (`true`)
	 * or converted directly as fixed-point values (`false`) when they are
	 * accessed.
	 */
	bool normalized = false;
	/**
	 * Specifies the byte offset between consecutive generic vertex attributes.
	 * If stride is 0, the generic vertex attributes are understood to be tightly
	 * packed in the array. The initial value is 0.
	 */
	i32 stride = 0;
	/**
	 * Specifies the offset of the first component of the first generic vertex
	 * attribute in the array in the data store of the buffer currently bound to
	 * the `gl::Target::Array` target. The initial value is 0.
	 */
	const void* offset = 0; // NOLINT
};

/**
 * @brief define an array of generic vertex attribute data
 *
 * @param index Specifies the index of the generic vertex attribute to be modified.
 * @param params Additional options.
 */
inline void vertex_attrib_pointer(u32 index, const VertexAttribParams& params = {}) {
	glVertexAttribPointer(
		index,
		params.size,
		static_cast<GLenum>(params.type),
		params.normalized,
		params.stride,
		params.offset);
}

/**
 * @brief Returns the location of an attribute variable
 *
 * @param program Specifies the program object to be queried.
 * @param name
 * Points to a null terminated string containing the name of the attribute
 * variable whose location is to be queried.
 */
inline auto get_attrib_location(u32 program, const char* name) -> i32 {
	return glGetAttribLocation(program, name);
}

/**
 * @brief Enable a generic vertex attribute array
 * @param index Specifies the index of the generic vertex attribute.
 */
inline void enable_vertex_attrib_array(u32 index) {
	glEnableVertexAttribArray(index);
}

/**
 * @brief Disable a generic vertex attribute array
 * @param index Specifies the index of the generic vertex attribute.
 */
inline void disable_vertex_attrib_array(u32 index) {
	glDisableVertexAttribArray(index);
}

enum class Result : GLenum {
	OK = GLEW_OK,
	NoGLVersion = GLEW_ERROR_NO_GL_VERSION,
	GLVersion10Only = GLEW_ERROR_GL_VERSION_10_ONLY,
	GLXVersion11Only = GLEW_ERROR_GLX_VERSION_11_ONLY,
	NoGLXDisplay = GLEW_ERROR_NO_GLX_DISPLAY,
};

inline auto create_program() -> u32 {
	return glCreateProgram();
}

inline auto create_shader(Shader type) -> u32 {
	return glCreateShader(static_cast<GLenum>(type));
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
inline void shader_source(u32 shader, i32 count, const char** sources, const i32* length) {
	glShaderSource(shader, count, sources, length);
}


inline void shader_source(u32 shader, const char* source) {
	gl::shader_source(shader, 1, &source, nullptr);
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
inline void get_shader_info_log(u32 shader, i32 max_length, i32* out_length, char* out_result) {
	glGetShaderInfoLog(shader, max_length, out_length, out_result);
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
inline void get_shader(u32 shader, ShaderParam param, i32* out_params) {
	glGetShaderiv(shader, static_cast<GLenum>(param), out_params);
}

inline auto get_shader_compile_status(u32 shader) -> bool {
	i32 result;
	gl::get_shader(shader, ShaderParam::CompileStatus, &result);

	return result == GL_TRUE;
}

inline auto get_shader_info_log_length(u32 shader) -> i32 {
	i32 result;
	gl::get_shader(shader, ShaderParam::InfoLogLength, &result);

	return result;
}

/**
 * @brief Determines if a name corresponds to a shader object
 */
inline auto is_shader(u32 name) -> bool {
	return glIsShader(name);
}

/**
 * @brief Compiles a shader object
 * @param shader Specifies the shader object to be compiled.
 * @see https://docs.gl/gl4/glCompileShader
 */
inline void compile_shader(u32 shader) {
	glCompileShader(shader);
}

inline void attach_shader(u32 program, u32 shader) {
	glAttachShader(program, shader);
}

inline void link_program(u32 program) {
	glLinkProgram(program);
}

/**
 * @brief Validates a program object
 * @param program Specifies the handle of the program object to be validated.
 * @see https://docs.gl/gl4/glValidateProgram
 */
inline void validate_program(u32 program) {
	glValidateProgram(program);
}

inline void delete_shader(u32 shader) {
	glDeleteShader(shader);
}

inline void use_program(u32 program) {
	glUseProgram(program);
}

inline void delete_program(u32 program) {
	glDeleteProgram(program);
}

inline void delete_buffers(i32 n, const u32* buffers) {
	glDeleteBuffers(n, buffers);
}

inline void delete_buffer(u32 buffer) {
	glDeleteBuffers(1, &buffer);
}

inline auto compile_shader(Shader type, const char* source) -> u32 {
	u32 shader = gl::create_shader(type);

	gl::shader_source(shader, source);
	gl::compile_shader(shader);

	if (!gl::get_shader_compile_status(shader)) {
		const i32 log_length = gl::get_shader_info_log_length(shader);
		char message_buffer[log_length];
		char* message = &message_buffer[0];
		gl::get_shader_info_log(shader, log_length, nullptr, message);

		fmt::print("Error compiling {}:\n   {}\n", gl::shader_type_name(type), message);
	}

	return shader;
}

inline auto compile_shader(Shader type, const std::string& source) -> u32 {
	return compile_shader(type, source.c_str());
}

template <typename... Args>
inline auto link_program(Args... shaders) -> u32 {
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

static auto shader_type(const std::string& keyword) -> Shader {
	if (keyword == "vertex") return Shader::Vertex;
	if (keyword == "fragment") return Shader::Fragment;
	if (keyword == "compute") return Shader::Compute;
	if (keyword == "tessc") return Shader::TessControl;
	if (keyword == "tesse") return Shader::TessEval;
	if (keyword == "geometry") return Shader::Geometry;
	return Shader::_none;
}

inline auto parse_shaders(const fs::path& file_path) -> std::unordered_map<Shader, std::string> {
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

inline auto make_program(const fs::path& shader_path) -> u32 {
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

/**
 * First you need to create a valid OpenGL rendering context and call
 * `glew::init()` to initialize the extension entry points. If `glew::init()`
 * returns `glew::Result::OK`, the initialization succeeded and you can use the
 * available extensions as well as core OpenGL functionality.
 *
 * @see https://glew.sourceforge.net/basic.html
 */
inline auto init() -> Result {
	return static_cast<Result>(glewInit());
}

inline auto get_error_string(Result result) -> char const* {
	return reinterpret_cast<const char*>(glewGetErrorString(static_cast<GLenum>(result)));
}

} // namespace gl
