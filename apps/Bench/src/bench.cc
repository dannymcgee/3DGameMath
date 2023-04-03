#include <benchmark/benchmark.h>

#include <math/matrix.h>
#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT
using benchmark::State;
using benchmark::DoNotOptimize;

using math::Matrix;
using Vec3f = math::Vector<3,f32>;
using Vec3 = math::Vector<3,f64>;

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
BENCHMARK(BM_Ctor_f32); // NOLINT
BENCHMARK(BM_Ctor_f64); // NOLINT


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
BENCHMARK(BM_CrossProduct_f32); // NOLINT
BENCHMARK(BM_CrossProduct_f64); // NOLINT


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
BENCHMARK(BM_Length_f32); // NOLINT
BENCHMARK(BM_Length_f64); // NOLINT


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
BENCHMARK(BM_Normal_f32); // NOLINT
BENCHMARK(BM_Normal_f64); // NOLINT


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
BENCHMARK(BM_LengthAndDirection_f32); // NOLINT
BENCHMARK(BM_LengthAndDirection_f64); // NOLINT


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
BENCHMARK(BM_Distance_f32); // NOLINT
BENCHMARK(BM_Distance_f64); // NOLINT


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
BENCHMARK(BM_DotProduct_f32); // NOLINT
BENCHMARK(BM_DotProduct_f64); // NOLINT


static void BM_Mat2x2_Determinant(State& state)
{
	auto mat = Matrix<2,2,f64>{
		{ -3.0, 4.0 },
		{  2.0, 5.0 },
	};

	for (auto _ : state)
		DoNotOptimize(mat.determinant());
}
static void BM_Mat3x3_Determinant(State& state)
{
	auto mat = Matrix<3,3,f64>{
		{ -4.0, -3.0,  3.0 },
		{  0.0,  2.0, -2.0 },
		{  1.0,  4.0, -1.0 },
	};

	for (auto _ : state)
		DoNotOptimize(mat.determinant());
}
static void BM_Mat4x4_Determinant(State& state)
{
	auto mat = Matrix<4,4,f64>{
		{ -4.0, -3.0,  3.0,  1.0 },
		{  0.0,  2.0, -2.0,  0.0 },
		{  1.0,  4.0, -1.0,  1.0 },
		{  0.0,  2.0, -2.0,  1.0 },
	};

	for (auto _ : state)
		DoNotOptimize(mat.determinant());
}
BENCHMARK(BM_Mat2x2_Determinant); // NOLINT
BENCHMARK(BM_Mat3x3_Determinant); // NOLINT
BENCHMARK(BM_Mat4x4_Determinant); // NOLINT


BENCHMARK_MAIN(); // NOLINT
