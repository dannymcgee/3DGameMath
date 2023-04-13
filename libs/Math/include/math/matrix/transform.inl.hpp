#pragma once

#include "math/matrix/transform.inl.h"

#include "math/matrix/rotation.h"
#include "math/matrix/translation.h"

namespace math {

// Constructors ----------------------------------------------------------------

constexpr TransformMatrix::TransformMatrix()
	: Super(Super::identity())
{}

constexpr TransformMatrix::TransformMatrix(
	const RotationMatrix& rotation,
	const TranslationMatrix& translation)
	: Super(construct(rotation, translation))
{}

constexpr TransformMatrix::TransformMatrix(const RotationMatrix& rotation)
	: Super(construct(rotation))
{}

constexpr TransformMatrix::TransformMatrix(const Quat& rotation, const Vec3& origin)
	: Super(construct(rotation, origin))
{}

constexpr auto TransformMatrix::construct(
	const RotationMatrix& rotation,
	const TranslationMatrix& translation)
-> Super
{
	return rotation * translation;
}

constexpr auto TransformMatrix::construct(const RotationMatrix& rotation) -> Super
{
	auto [m11, m12, m13] = rotation.template row<1>();
	auto [m21, m22, m23] = rotation.template row<2>();
	auto [m31, m32, m33] = rotation.template row<3>();

	return {
		{ m11, m12, m13,   0 },
		{ m21, m22, m23,   0 },
		{ m31, m32, m33,   0 },
		{   0,   0,   0,   1 },
	};
}

constexpr auto TransformMatrix::construct(const Quat& rotation, const Vec3& origin) -> Super
{
	auto rot_matrix = rotation.matrix();
	auto [m11, m12, m13] = rot_matrix.template row<1>();
	auto [m21, m22, m23] = rot_matrix.template row<2>();
	auto [m31, m32, m33] = rot_matrix.template row<3>();

	auto [m41, m42, m43] = origin;

	return {
		{ m11, m12, m13,   0 },
		{ m21, m22, m23,   0 },
		{ m31, m32, m33,   0 },
		{ m41, m42, m43,   1 },
	};
}


// Transformation --------------------------------------------------------------

constexpr auto TransformMatrix::transform_point(const Vec3& point) const -> Vec3
{
	auto [xin, yin, zin] = point;
	auto [xout, yout, zout, _] = Vec4{ xin, yin, zin, 1 } * (*this);

	return Vec3{ xout, yout, zout };
}

constexpr auto TransformMatrix::transform_vector(const Vec3& vector) const -> Vec3
{
	auto [xin, yin, zin] = vector;
	auto [xout, yout, zout, _] = Vec4{ xin, yin, zin, 0 } * (*this);

	return Vec3{ xout, yout, zout };
}

} // namespace math
