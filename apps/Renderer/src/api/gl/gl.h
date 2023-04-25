#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <sized.h>

#include "api/gl/attributes.h"
#include "api/gl/shader.h"
#include "api/gl/types.h"
#include "api/gl/uniforms.h"


/**
 * A fairly pointless wrapper around the OpenGL API with inline docs, distinct
 * enums, and some paper-thin syntactic conveniences.
 */
namespace gl {
using namespace sized;


/** @see `gen_buffers` */
inline auto gen_buffer() -> u32
{
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
inline void gen_buffers(i32 n, u32* out_buffers)
{
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
inline void bind_buffer(Target target, u32 buffer)
{
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
inline void buffer_data(Target target, i32 size, void* data, Usage usage)
{
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
inline void clear(u32 mask)
{
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
inline void draw_arrays(DrawMode mode, i32 first, i32 count)
{
	glDrawArrays(static_cast<GLenum>(mode), first, count);
}

/**
 * @brief render primitives from array data
 *
 * @tparam Unsigned Specifies the type of the values in `indices`.
 *
 * @param mode Specifies what kind of primitives to render.
 * @param count Specifies the number of elements to be rendered.
 *
 * @param indices
 * Specifies an offset of the first index in the array in the data store of the
 * buffer currently bound to the `gl::BufferTarget::ElementArray` target.
 *
 * @see https://docs.gl/gl4/glDrawElements
 */
template <typename Unsigned>
inline void draw_elements(DrawMode mode, i32 count, const Unsigned* indices);
/**
 * @brief render primitives from array data
 *
 * @param mode Specifies what kind of primitives to render.
 * @param count Specifies the number of elements to be rendered.
 *
 * @param indices
 * Specifies an offset of the first index in the array in the data store of the
 * buffer currently bound to the `gl::BufferTarget::ElementArray` target.
 *
 * @see https://docs.gl/gl4/glDrawElements
 */
template <>
inline void draw_elements(DrawMode mode, i32 count, const u8* indices)
{
	glDrawElements(static_cast<GLenum>(mode), count, GL_UNSIGNED_BYTE, indices);
}
/**
 * @brief render primitives from array data
 *
 * @param mode Specifies what kind of primitives to render.
 * @param count Specifies the number of elements to be rendered.
 *
 * @param indices
 * Specifies an offset of the first index in the array in the data store of the
 * buffer currently bound to the `gl::BufferTarget::ElementArray` target.
 *
 * @see https://docs.gl/gl4/glDrawElements
 */
template <>
inline void draw_elements(DrawMode mode, i32 count, const u16* indices)
{
	glDrawElements(static_cast<GLenum>(mode), count, GL_UNSIGNED_SHORT, indices);
}
/**
 * @brief render primitives from array data
 *
 * @param mode Specifies what kind of primitives to render.
 * @param count Specifies the number of elements to be rendered.
 *
 * @param indices
 * Specifies an offset of the first index in the array in the data store of the
 * buffer currently bound to the `gl::BufferTarget::ElementArray` target.
 *
 * @see https://docs.gl/gl4/glDrawElements
 */
template <>
inline void draw_elements(DrawMode mode, i32 count, const u32* indices)
{
	glDrawElements(static_cast<GLenum>(mode), count, GL_UNSIGNED_INT, indices);
}

/**
 * @brief return a string describing the current GL connection
 * @param name
 */
inline auto get_string(Info name) -> char const*
{
	return reinterpret_cast<const char*>(glGetString(static_cast<GLenum>(name)));
}

/**
 * @brief return a string describing the current GL connection
 * @param name
 * @param index Specifies the index of the string to return.
 */
inline auto get_string(Info name, u32 index) -> char const*
{
	return reinterpret_cast<const char*>(glGetStringi(static_cast<GLenum>(name), index));
}

inline void delete_buffers(i32 n, const u32* buffers)
{
	glDeleteBuffers(n, buffers);
}

inline void delete_buffer(u32 buffer)
{
	glDeleteBuffers(1, &buffer);
}

/**
 * First you need to create a valid OpenGL rendering context and call
 * `glew::init()` to initialize the extension entry points. If `glew::init()`
 * returns `glew::Result::OK`, the initialization succeeded and you can use the
 * available extensions as well as core OpenGL functionality.
 *
 * @see https://glew.sourceforge.net/basic.html
 */
inline auto init() -> Result
{
	return static_cast<Result>(glewInit());
}

inline auto get_error_string(Result result) -> char const*
{
	return reinterpret_cast<const char*>(glewGetErrorString(static_cast<GLenum>(result)));
}

} // namespace gl
