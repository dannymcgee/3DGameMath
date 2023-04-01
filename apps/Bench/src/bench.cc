#include <benchmark/benchmark.h>

#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT
using benchmark::State;
using benchmark::DoNotOptimize;

using Vec3f = math::Vector<3, f32>;
using Vec3 = math::Vector<3, f64>;

// Constructor (sanity check)
static void BM_Ctor_f32(State& state)
{
	for (auto _ : state)
		for (usize i = 0; i < 1000; ++i)
			DoNotOptimize(Vec3f{ 1, 2, 3 });
}
static void BM_Ctor_f64(State& state)
{
	for (auto _ : state)
		for (usize i = 0; i < 1000; ++i)
			DoNotOptimize(Vec3{ 1, 2, 3 });
}
BENCHMARK(BM_Ctor_f32); // NOLINT
BENCHMARK(BM_Ctor_f64); // NOLINT


// Cross-Product
static void BM_CrossProduct_f32(State& state)
{
	for (auto _ : state) {
		for (usize i = 0; i < 1000; ++i) {
			auto a = Vec3f{ 1.f, 3.f, 4.f };
			auto b = Vec3f{ 2.f, -5.f, 8.f };
			DoNotOptimize(a.cross(b));
		}
	}
}
static void BM_CrossProduct_f64(State& state)
{
	for (auto _ : state) {
		for (usize i = 0; i < 1000; ++i) {
			auto a = Vec3{ 1.0, 3.0, 4.0 };
			auto b = Vec3{ 2.0, -5.0, 8.0 };
			DoNotOptimize(a.cross(b));
		}
	}
}
BENCHMARK(BM_CrossProduct_f32); // NOLINT
BENCHMARK(BM_CrossProduct_f64); // NOLINT


// Length
static void BM_Length_f32(State& state)
{
	for (auto _ : state) {
		for (usize i = 0; i < 1000; ++i) {
			auto vec = Vec3f{ 1.f, 2.f, 3.f };
			DoNotOptimize(vec.length());
		}
	}
}
static void BM_Length_f64(State& state)
{
	for (auto _ : state) {
		for (usize i = 0; i < 1000; ++i) {
			auto vec = Vec3{ 1.0, 2.0, 3.0 };
			DoNotOptimize(vec.length());
		}
	}
}
BENCHMARK(BM_Length_f32); // NOLINT
BENCHMARK(BM_Length_f64); // NOLINT


// Normal
static void BM_Normal_f32(State& state)
{
	for (auto _ : state) {
		for (usize i = 0; i < 1000; ++i) {
			auto vec = Vec3f{ 1.f, 2.f, 3.f };
			DoNotOptimize(vec.normal());
		}
	}
}
static void BM_Normal_f64(State& state)
{
	for (auto _ : state) {
		for (usize i = 0; i < 1000; ++i) {
			auto vec = Vec3{ 1.0, 2.0, 3.0 };
			DoNotOptimize(vec.normal());
		}
	}
}
BENCHMARK(BM_Normal_f32); // NOLINT
BENCHMARK(BM_Normal_f64); // NOLINT


// Length and Direction
static void BM_LengthAndDirection_f32(State& state)
{
	for (auto _ : state) {
		for (usize i = 0; i < 1000; ++i) {
			auto vec = Vec3f{ 1.f, 2.f, 3.f };
			DoNotOptimize(vec.length_and_direction());
		}
	}
}
static void BM_LengthAndDirection_f64(State& state)
{
	for (auto _ : state) {
		for (usize i = 0; i < 1000; ++i) {
			auto vec = Vec3f{ 1.0, 2.0, 3.0 };
			DoNotOptimize(vec.length_and_direction());
		}
	}
}
BENCHMARK(BM_LengthAndDirection_f32); // NOLINT
BENCHMARK(BM_LengthAndDirection_f64); // NOLINT


// Distance
static void BM_Distance_f32(State& state)
{
	for (auto _ : state) {
		for (usize i = 0; i < 1000; ++i) {
			auto a = Vec3f{ 1.f, 3.f, 4.f };
			auto b = Vec3f{ 2.f, -5.f, 8.f };
			DoNotOptimize(Vec3f::dist(a, b));
		}
	}
}
static void BM_Distance_f64(State& state)
{
	for (auto _ : state) {
		for (usize i = 0; i < 1000; ++i) {
			auto a = Vec3{ 1.0, 3.0, 4.0 };
			auto b = Vec3{ 2.0, -5.0, 8.0 };
			DoNotOptimize(Vec3::dist(a, b));
		}
	}
}
BENCHMARK(BM_Distance_f32); // NOLINT
BENCHMARK(BM_Distance_f64); // NOLINT


// Dot-Product
static void BM_DotProduct_f32(State& state)
{
	for (auto _ : state) {
		for (usize i = 0; i < 1000; ++i) {
			auto a = Vec3f{ 1.f, 3.f, 4.f };
			auto b = Vec3f{ 2.f, -5.f, 8.f };
			DoNotOptimize(a.dot(b));
		}
	}
}
static void BM_DotProduct_f64(State& state)
{
	for (auto _ : state) {
		for (usize i = 0; i < 1000; ++i) {
			auto a = Vec3{ 1.0, 3.0, 4.0 };
			auto b = Vec3{ 2.0, -5.0, 8.0 };
			DoNotOptimize(a.dot(b));
		}
	}
}
BENCHMARK(BM_DotProduct_f32); // NOLINT
BENCHMARK(BM_DotProduct_f64); // NOLINT


BENCHMARK_MAIN(); // NOLINT
