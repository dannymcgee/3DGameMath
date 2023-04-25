#pragma once

#include <math/matrix.h>
#include <math/sfinae.h>
#include <math/vector.h>
#include <sized.h>

#include "api/gl/types.h"


namespace gl {
using namespace sized;
using math::Mat2x2;
using math::Mat3x3;
using math::Mat4x4;
using math::Mat3x4;
using math::Mat4x3;
using math::Vec2;
using math::Vec3;
using math::Vec4;

inline auto get_uniform_location(u32 program, const char* name) -> i32
{
	return glGetUniformLocation(program, name);
}

template <typename T>
void uniform(i32 location, const T& data);

template <>
inline void uniform<f32>(i32 location, const f32& data)
{
	glUniform1f(location, data);
}
template <>
inline void uniform<Vec2>(i32 location, const Vec2& data)
{
	glUniform2f(location, data.x, data.y);
}
template <>
inline void uniform<Vec3>(i32 location, const Vec3& data)
{
	glUniform3f(location, data.x, data.y, data.z);
}
template <>
inline void uniform<Vec4>(i32 location, const Vec4& data)
{
	glUniform4f(location, data.x, data.y, data.z, data.w);
}


struct UniformMatrixParams {
	bool transpose = false;
};

template <usize R, usize C>
void uniform(
	i32 location,
	i32 count, const math::Matrix<R,C> data[],
	const UniformMatrixParams& params = {});

template <usize R, usize C>
inline void uniform(
	i32 location,
	const math::Matrix<R,C>& data,
	const UniformMatrixParams& params = {})
{
	uniform<R,C>(location, 1, &data, params);
}

template <>
inline void uniform<2,2>(
	i32 location,
	i32 count, const Mat2x2 data[],
	const UniformMatrixParams& params)
{
	glUniformMatrix2fv(location, count, params.transpose, &data->m11);
}
template <>
inline void uniform<3,3>(
	i32 location,
	i32 count, const Mat3x3 data[],
	const UniformMatrixParams& params)
{
	glUniformMatrix3fv(location, count, params.transpose, &data->m11);
}
template <>
inline void uniform<4,4>(
	i32 location,
	i32 count, const Mat4x4 data[],
	const UniformMatrixParams& params)
{
	glUniformMatrix4fv(location, count, params.transpose, &data->m11);
}
template <>
inline void uniform<4,3>(
	i32 location,
	i32 count, const Mat4x3 data[],
	const UniformMatrixParams& params)
{
	glUniformMatrix4x3fv(location, count, params.transpose, &data->m11);
}
template <>
inline void uniform<3,4>(
	i32 location,
	i32 count, const Mat3x4 data[],
	const UniformMatrixParams& params)
{
	glUniformMatrix3x4fv(location, count, params.transpose, &data->m11);
}

} // namespace gl
