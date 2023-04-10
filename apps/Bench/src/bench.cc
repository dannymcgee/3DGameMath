#include "math/matrix/_rotation_decl.h"
#include <benchmark/benchmark.h>

#include <array>

#include <math/euler.h>
#include <math/literals.h>
#include <math/matrix.h>
#include <math/matrix/rotation.h>
#include <math/quat.h>
#include <math/spaces.h>
#include <math/vector.h>
#include <sized.h>

// NOLINTBEGIN

using namespace sized;
using benchmark::State;
using benchmark::DoNotOptimize;

using math::Mat2x2;
using math::Mat3x3;
using math::Mat4x4;

using RotationMatrix = math::RotationMatrix<f64>;
using Euler = math::Euler<f64>;
using Quat = math::Quat<f64>;
using math::Axis;
using math::Space;

using math::Vec2;
using math::Vec3;
using math::Vec4;
using Vec3f = math::Vector<3,f32>;


// Constructor (sanity check)
static void BM_Ctor_f32(State& state)
{
	for (auto _ : state)
		DoNotOptimize(Vec3f{ 1, 2, 3 });
}
static void BM_Ctor_f64(State& state)
{
	for (auto _ : state)
		DoNotOptimize(Vec3{ 1, 2, 3 });
}
BENCHMARK(BM_Ctor_f32);
BENCHMARK(BM_Ctor_f64);


// Cross-Product
static void BM_CrossProduct_f32(State& state)
{
	auto a = Vec3f{ 1.f, 3.f, 4.f };
	auto b = Vec3f{ 2.f, -5.f, 8.f };

	for (auto _ : state)
		DoNotOptimize(a.cross(b));
}
static void BM_CrossProduct_f64(State& state)
{
	auto a = Vec3{ 1.0, 3.0, 4.0 };
	auto b = Vec3{ 2.0, -5.0, 8.0 };

	for (auto _ : state)
		DoNotOptimize(a.cross(b));
}
BENCHMARK(BM_CrossProduct_f32);
BENCHMARK(BM_CrossProduct_f64);


// Length
static void BM_Length_f32(State& state)
{
	auto vec = Vec3f{ 1.f, 2.f, 3.f };
	for (auto _ : state)
		DoNotOptimize(vec.length());
}
static void BM_Length_f64(State& state)
{
	auto vec = Vec3{ 1.0, 2.0, 3.0 };
	for (auto _ : state)
		DoNotOptimize(vec.length());
}
BENCHMARK(BM_Length_f32);
BENCHMARK(BM_Length_f64);


// Normal
static void BM_Normal_f32(State& state)
{
	auto vec = Vec3f{ 1.f, 2.f, 3.f };
	for (auto _ : state)
		DoNotOptimize(vec.normal());
}
static void BM_Normal_f64(State& state)
{
	auto vec = Vec3{ 1.0, 2.0, 3.0 };
	for (auto _ : state)
		DoNotOptimize(vec.normal());
}
BENCHMARK(BM_Normal_f32);
BENCHMARK(BM_Normal_f64);


// Length and Direction
static void BM_LengthAndDirection_f32(State& state)
{
	auto vec = Vec3f{ 1.f, 2.f, 3.f };
	for (auto _ : state)
		DoNotOptimize(vec.length_and_direction());
}
static void BM_LengthAndDirection_f64(State& state)
{
	auto vec = Vec3f{ 1.0, 2.0, 3.0 };
	for (auto _ : state)
		DoNotOptimize(vec.length_and_direction());
}
BENCHMARK(BM_LengthAndDirection_f32);
BENCHMARK(BM_LengthAndDirection_f64);


// Distance
static void BM_Distance_f32(State& state)
{
	auto a = Vec3f{ 1.f, 3.f, 4.f };
	auto b = Vec3f{ 2.f, -5.f, 8.f };

	for (auto _ : state)
		DoNotOptimize(Vec3f::dist(a, b));
}
static void BM_Distance_f64(State& state)
{
	auto a = Vec3{ 1.0, 3.0, 4.0 };
	auto b = Vec3{ 2.0, -5.0, 8.0 };

	for (auto _ : state)
		DoNotOptimize(Vec3::dist(a, b));
}
BENCHMARK(BM_Distance_f32);
BENCHMARK(BM_Distance_f64);


// Dot-Product
static void BM_DotProduct_f32(State& state)
{
	auto a = Vec3f{ 1.f, 3.f, 4.f };
	auto b = Vec3f{ 2.f, -5.f, 8.f };

	for (auto _ : state)
		DoNotOptimize(a.dot(b));
}
static void BM_DotProduct_f64(State& state)
{
	auto a = Vec3{ 1.0, 3.0, 4.0 };
	auto b = Vec3{ 2.0, -5.0, 8.0 };

	for (auto _ : state)
		DoNotOptimize(a.dot(b));
}
BENCHMARK(BM_DotProduct_f32);
BENCHMARK(BM_DotProduct_f64);


// Matrix Constructors
static void BM_Mat2x2_Ctor(State& state)
{
	for (auto _ : state) {
		auto mat = Mat2x2{
			{ -3.0, 4.0 },
			{  2.0, 5.0 },
		};
		DoNotOptimize(mat);
	}
}
static void BM_Mat3x3_Ctor(State& state)
{
	for (auto _ : state) {
		auto mat = Mat3x3{
			{ -4.0, -3.0,  3.0 },
			{  0.0,  2.0, -2.0 },
			{  1.0,  4.0, -1.0 },
		};
		DoNotOptimize(mat);
	}
}
static void BM_Mat4x4_Ctor(State& state)
{
	for (auto _ : state) {
		auto mat = Mat4x4{
			{ -4.0, -3.0,  3.0,  1.0 },
			{  0.0,  2.0, -2.0,  0.0 },
			{  1.0,  4.0, -1.0,  1.0 },
			{  0.0,  2.0, -2.0,  1.0 },
		};
		DoNotOptimize(mat);
	}
}
BENCHMARK(BM_Mat2x2_Ctor);
BENCHMARK(BM_Mat3x3_Ctor);
BENCHMARK(BM_Mat4x4_Ctor);


// Matrix Transposition
static void BM_Mat2x2_Transpose(State& state)
{
	auto mat = Mat2x2{
		{ -3.0, 4.0 },
		{  2.0, 5.0 },
	};
	for (auto _ : state)
		DoNotOptimize(mat.transpose());
}
static void BM_Mat3x3_Transpose(State& state)
{
	auto mat = Mat3x3{
		{ -4.0, -3.0,  3.0 },
		{  0.0,  2.0, -2.0 },
		{  1.0,  4.0, -1.0 },
	};
	for (auto _ : state)
		DoNotOptimize(mat.transpose());
}
static void BM_Mat4x4_Transpose(State& state)
{
	auto mat = Mat4x4{
		{ -4.0, -3.0,  3.0,  1.0 },
		{  0.0,  2.0, -2.0,  0.0 },
		{  1.0,  4.0, -1.0,  1.0 },
		{  0.0,  2.0, -2.0,  1.0 },
	};
	for (auto _ : state)
		DoNotOptimize(mat.transpose());
}
BENCHMARK(BM_Mat2x2_Transpose);
BENCHMARK(BM_Mat3x3_Transpose);
BENCHMARK(BM_Mat4x4_Transpose);


// Matrix Determinants
static void BM_Mat2x2_Determinant(State& state)
{
	auto mat = Mat2x2{
		{ -3.0, 4.0 },
		{  2.0, 5.0 },
	};
	for (auto _ : state)
		DoNotOptimize(mat.determinant());
}
static void BM_Mat3x3_Determinant(State& state)
{
	auto mat = Mat3x3{
		{ -4.0, -3.0,  3.0 },
		{  0.0,  2.0, -2.0 },
		{  1.0,  4.0, -1.0 },
	};
	for (auto _ : state)
		DoNotOptimize(mat.determinant());
}
static void BM_Mat4x4_Determinant(State& state)
{
	auto mat = Mat4x4{
		{ -4.0, -3.0,  3.0,  1.0 },
		{  0.0,  2.0, -2.0,  0.0 },
		{  1.0,  4.0, -1.0,  1.0 },
		{  0.0,  2.0, -2.0,  1.0 },
	};
	for (auto _ : state)
		DoNotOptimize(mat.determinant());
}
BENCHMARK(BM_Mat2x2_Determinant);
BENCHMARK(BM_Mat3x3_Determinant);
BENCHMARK(BM_Mat4x4_Determinant);


static void BM_Mat2x2_Inverse(State& state)
{
	auto mat = Mat2x2{
		{ -3.0, 4.0 },
		{  2.0, 5.0 },
	};
	for (auto _ : state)
		DoNotOptimize(mat.inverse());
}
static void BM_Mat3x3_Inverse(State& state)
{
	auto mat = Mat3x3{
		{ -4.0, -3.0,  3.0 },
		{  0.0,  2.0, -2.0 },
		{  1.0,  4.0, -1.0 },
	};
	for (auto _ : state)
		DoNotOptimize(mat.inverse());
}
static void BM_Mat4x4_Inverse(State& state)
{
	auto mat = Mat4x4{
		{ -4.0, -3.0,  3.0,  1.0 },
		{  0.0,  2.0, -2.0,  0.0 },
		{  1.0,  4.0, -1.0,  1.0 },
		{  0.0,  2.0, -2.0,  1.0 },
	};
	for (auto _ : state)
		DoNotOptimize(mat.inverse());
}
static void BM_RotationMatrix_Inverse(State& state)
{
	auto angle = math::deg2rad(45.0);
	auto axis = Vec3{ -0.25, 0.5, 0.33 }.unit();
	auto rotation_mat = RotationMatrix(angle, axis);

	for (auto _ : state)
		DoNotOptimize(rotation_mat.inverse());
}
static void BM_Identity4x4_Inverse(State& state)
{
	auto mat = Mat4x4::identity();

	for (auto _ : state)
		DoNotOptimize(mat.inverse());
}
BENCHMARK(BM_Mat2x2_Inverse);
BENCHMARK(BM_Mat3x3_Inverse);
BENCHMARK(BM_RotationMatrix_Inverse);
BENCHMARK(BM_Mat4x4_Inverse);
BENCHMARK(BM_Identity4x4_Inverse);


static void BM_Mat3x3_Orthogonalize(State& state)
{
	auto angle = math::deg2rad(45.0);
	auto axis = Vec3{ -0.25, 0.5, 0.33 }.unit();
	const auto initial = RotationMatrix(angle, axis);

	for (auto _ : state) {
		auto mat = initial;
		mat.orthogonalize();
		DoNotOptimize(mat);
	}
}
BENCHMARK(BM_Mat3x3_Orthogonalize);


static void BM_EulerToMatrix_Expanded(State& state)
{
	using namespace math::literals;
	auto euler = Euler{ 31.9_deg, -22.8_deg, 17.2_deg };

	for (auto _ : state)
		DoNotOptimize(euler.matrix(Space::Local2Parent));
}
static void BM_EulerToMatrix_Composed(State& state)
{
	using namespace math::literals;
	auto euler = Euler{ 31.9_deg, -22.8_deg, 17.2_deg };

	for (auto _ : state) {
		auto mat
			= RotationMatrix(euler.roll, Axis::Forward)
			* RotationMatrix(euler.pitch, Axis::Right)
			* RotationMatrix(euler.yaw, Axis::Up);

		DoNotOptimize(mat);
	}
}
BENCHMARK(BM_EulerToMatrix_Expanded);
BENCHMARK(BM_EulerToMatrix_Composed);


static void BM_Max_Recursive(State& state)
{
	using namespace math::literals;
	auto m = RotationMatrix(45_deg, Vec3{ -0.25, 0.5, 0.33 }.normal());
	f64 w = m.m11 + m.m22 + m.m33;
	f64 x = m.m11 - m.m22 - m.m33;
	f64 y = m.m22 - m.m11 - m.m33;
	f64 z = m.m33 - m.m11 - m.m22;

	for (auto _ : state) {
		f64 largest = std::max(w, std::max(x, std::max(y, z)));
		DoNotOptimize(largest);
	}
}
static void BM_Max_InitList(State& state)
{
	using namespace math::literals;
	auto m = RotationMatrix(45_deg, Vec3{ -0.25, 0.5, 0.33 }.normal());
	f64 w = m.m11 + m.m22 + m.m33;
	f64 x = m.m11 - m.m22 - m.m33;
	f64 y = m.m22 - m.m11 - m.m33;
	f64 z = m.m33 - m.m11 - m.m22;

	for (auto _ : state) {
		f64 largest = std::max({ w, x, y, z });
		DoNotOptimize(largest);
	}
}
BENCHMARK(BM_Max_Recursive);
BENCHMARK(BM_Max_InitList);


static void BM_Slerp(State& state)
{
	using namespace math::literals;
	auto three60 = Quat::angle_axis(360_deg, Vec3::up());
	auto seven20 = Quat::angle_axis(720_deg, Vec3::up());

	for (auto _ : state)
		DoNotOptimize(Quat::slerp(three60, seven20, 0.5));
}
BENCHMARK(BM_Slerp);


static void BM_Euler2Quat_Composed1(State& state)
{
	using namespace math::literals;
	auto euler = Euler{ 45_deg, -15_deg, 3.3_deg };

	for (auto _ : state) {
		auto yq = Quat{
			std::cos(euler.yaw * 0.5),
			{ 0, std::sin(euler.yaw * 0.5), 0 },
		};
		auto pq = Quat{
			std::cos(euler.pitch * 0.5),
			{ std::sin(euler.pitch * 0.5), 0, 0 },
		};
		auto rq = Quat{
			std::cos(euler.roll * 0.5),
			{ 0, 0, std::sin(euler.roll * 0.5) },
		};
		auto result = yq * pq * rq;
		DoNotOptimize(result);
	}
}
static void BM_Euler2Quat_Composed2(State& state)
{
	using namespace math::literals;
	auto euler = Euler{ 45_deg, -15_deg, 3.3_deg };

	for (auto _ : state) {
		auto yq = Quat::angle_axis(euler.yaw, Vec3::up());
		auto pq = Quat::angle_axis(euler.pitch, Vec3::right());
		auto rq = Quat::angle_axis(euler.roll, Vec3::forward());
		auto result = yq * pq * rq;
		DoNotOptimize(result);
	}
}
static void BM_Euler2Quat_Expanded(State& state)
{
	using namespace math::literals;
	auto euler = Euler{ 45_deg, -15_deg, 3.3_deg };

	for (auto _ : state) {
		f64 cos_y2 = std::cos(euler.yaw * 0.5);
		f64 cos_p2 = std::cos(euler.pitch * 0.5);
		f64 cos_r2 = std::cos(euler.roll * 0.5);

		f64 sin_y2 = std::sin(euler.yaw * 0.5);
		f64 sin_p2 = std::sin(euler.pitch * 0.5);
		f64 sin_r2 = std::sin(euler.roll * 0.5);

		auto result = Quat{
			 cos_y2*cos_p2*cos_r2 + sin_y2*sin_p2*sin_r2,
			-cos_y2*sin_p2*cos_r2 - sin_y2*cos_p2*sin_r2,
			 cos_y2*sin_p2*sin_r2 - sin_y2*cos_p2*cos_r2,
			 sin_y2*sin_p2*cos_r2 - cos_y2*cos_p2*sin_r2,
		};
		DoNotOptimize(result);
	}
}
BENCHMARK(BM_Euler2Quat_Composed1);
BENCHMARK(BM_Euler2Quat_Composed2);
BENCHMARK(BM_Euler2Quat_Expanded);


BENCHMARK_MAIN();

// NOLINTEND
