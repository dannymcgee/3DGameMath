#pragma once

#include <sized.h>

#include "api/gl/types.h"


namespace gl {
using namespace sized;

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
inline void vertex_attrib_pointer(u32 index, const VertexAttribParams& params = {})
{
	glVertexAttribPointer(
		index,
		params.size,
		static_cast<GLenum>(params.type),
		params.normalized,
		params.stride,
		params.offset);
}

struct VertexAttribTmplParams {
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
 * @param size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
 *
 * @see https://docs.gl/gl4/glVertexAttribPointer
 */
template <GLenum Type>
inline void vertex_attrib_pointer(u32 index, i32 size, const VertexAttribTmplParams& params = {})
{
	glVertexAttribPointer(
		index,
		size,
		Type,
		params.normalized,
		params.stride,
		params.offset);
}
/**
 * @brief define an array of generic vertex attribute data
 *
 * @param index Specifies the index of the generic vertex attribute to be modified.
 * @param size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
 *
 * @see https://docs.gl/gl4/glVertexAttribPointer
 */
template <Scalar Type>
inline void vertex_attrib_pointer(u32 index, i32 size, const VertexAttribTmplParams& params = {})
{
	glVertexAttribPointer(
		index,
		size,
		static_cast<GLenum>(Type),
		params.normalized,
		params.stride,
		params.offset);
}
/**
 * @brief define an array of generic vertex attribute data
 *
 * @param index Specifies the index of the generic vertex attribute to be modified.
 * @param size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
 *
 * @see https://docs.gl/gl4/glVertexAttribPointer
 */
template <typename T>
inline void vertex_attrib_pointer(u32 index, i32 size, const VertexAttribTmplParams& params = {});

template <>
inline void vertex_attrib_pointer<i8>(u32 index, i32 size, const VertexAttribTmplParams& params)
{
	glVertexAttribPointer(
		index,
		size,
		GL_BYTE,
		params.normalized,
		params.stride,
		params.offset);
}
/**
 * @brief define an array of generic vertex attribute data
 *
 * @param index Specifies the index of the generic vertex attribute to be modified.
 * @param size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
 *
 * @see https://docs.gl/gl4/glVertexAttribPointer
 */
template <>
inline void vertex_attrib_pointer<u8>(u32 index, i32 size, const VertexAttribTmplParams& params)
{
	glVertexAttribPointer(
		index,
		size,
		GL_UNSIGNED_BYTE,
		params.normalized,
		params.stride,
		params.offset);
}
/**
 * @brief define an array of generic vertex attribute data
 *
 * @param index Specifies the index of the generic vertex attribute to be modified.
 * @param size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
 *
 * @see https://docs.gl/gl4/glVertexAttribPointer
 */
template <>
inline void vertex_attrib_pointer<i16>(u32 index, i32 size, const VertexAttribTmplParams& params)
{
	glVertexAttribPointer(
		index,
		size,
		GL_SHORT,
		params.normalized,
		params.stride,
		params.offset);
}
/**
 * @brief define an array of generic vertex attribute data
 *
 * @param index Specifies the index of the generic vertex attribute to be modified.
 * @param size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
 *
 * @see https://docs.gl/gl4/glVertexAttribPointer
 */
template <>
inline void vertex_attrib_pointer<u16>(u32 index, i32 size, const VertexAttribTmplParams& params)
{
	glVertexAttribPointer(
		index,
		size,
		GL_UNSIGNED_SHORT,
		params.normalized,
		params.stride,
		params.offset);
}
/**
 * @brief define an array of generic vertex attribute data
 *
 * @param index Specifies the index of the generic vertex attribute to be modified.
 * @param size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
 *
 * @see https://docs.gl/gl4/glVertexAttribPointer
 */
template <>
inline void vertex_attrib_pointer<i32>(u32 index, i32 size, const VertexAttribTmplParams& params)
{
	glVertexAttribPointer(
		index,
		size,
		GL_INT,
		params.normalized,
		params.stride,
		params.offset);
}
/**
 * @brief define an array of generic vertex attribute data
 *
 * @param index Specifies the index of the generic vertex attribute to be modified.
 * @param size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
 *
 * @see https://docs.gl/gl4/glVertexAttribPointer
 */
template <>
inline void vertex_attrib_pointer<u32>(u32 index, i32 size, const VertexAttribTmplParams& params)
{
	glVertexAttribPointer(
		index,
		size,
		GL_UNSIGNED_INT,
		params.normalized,
		params.stride,
		params.offset);
}
/**
 * @brief define an array of generic vertex attribute data
 *
 * @param index Specifies the index of the generic vertex attribute to be modified.
 * @param size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
 *
 * @see https://docs.gl/gl4/glVertexAttribPointer
 */
template <>
inline void vertex_attrib_pointer<f32>(u32 index, i32 size, const VertexAttribTmplParams& params)
{
	glVertexAttribPointer(
		index,
		size,
		GL_FLOAT,
		params.normalized,
		params.stride,
		params.offset);
}
/**
 * @brief define an array of generic vertex attribute data
 *
 * @param index Specifies the index of the generic vertex attribute to be modified.
 * @param size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
 *
 * @see https://docs.gl/gl4/glVertexAttribPointer
 */
template <>
inline void vertex_attrib_pointer<f64>(u32 index, i32 size, const VertexAttribTmplParams& params)
{
	glVertexAttribPointer(
		index,
		size,
		GL_DOUBLE,
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
inline auto get_attrib_location(u32 program, const char* name) -> i32
{
	return glGetAttribLocation(program, name);
}

/**
 * @brief Enable a generic vertex attribute array
 * @param index Specifies the index of the generic vertex attribute.
 */
inline void enable_vertex_attrib_array(u32 index)
{
	glEnableVertexAttribArray(index);
}

/**
 * @brief Disable a generic vertex attribute array
 * @param index Specifies the index of the generic vertex attribute.
 */
inline void disable_vertex_attrib_array(u32 index)
{
	glDisableVertexAttribArray(index);
}

} // namespace gl
