#include "math/_matrix_decl.h"
#include <benchmark/benchmark.h>

#include <array>

#include <math/matrix.h>
#include <math/matrix/rotation.h>
#include <math/vector.h>
#include <sized.h>

// NOLINTBEGIN

using namespace sized;
using benchmark::State;
using benchmark::DoNotOptimize;

using Mat2x2 = math::Matrix<2,2,f64>;
using Mat3x3 = math::Matrix<3,3,f64>;
using Mat4x4 = math::Matrix<4,4,f64>;
using math::RotationMatrix;

using Vec2 = math::Vector<2,f64>;
using Vec3 = math::Vector<3,f64>;
using Vec4 = math::Vector<4,f64>;
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


// FIXME: Huge performance regression -- ~0.8ns -> ~25ns
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


BENCHMARK_MAIN();

// NOLINTEND
