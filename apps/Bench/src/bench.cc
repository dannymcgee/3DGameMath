#include <benchmark/benchmark.h>

#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT
using benchmark::State;
using math::Vec;

// Constructor (sanity check)
static void BM_Ctor(State& state)
{
	for (auto _ : state)
		auto _vec = Vec<3, f32>{ 1, 2, 3 };
}
BENCHMARK(BM_Ctor); // NOLINT


// Cross-Product
static void BM_CrossProduct(State& state)
{
	for (auto _ : state) {
		auto a = Vec<3, f32>{ 1.f, 3.f, 4.f };
		auto b = Vec<3, f32>{ 2.f, -5.f, 8.f };
		auto result = a.cross(b);
	}
}
BENCHMARK(BM_CrossProduct); // NOLINT


// Dot-Product
static void BM_DotProduct(State& state)
{
	for (auto _ : state) {
		auto a = Vec<3, f32>{ 1.f, 3.f, 4.f };
		auto b = Vec<3, f32>{ 2.f, -5.f, 8.f };
		auto result = a.dot(b);
	}
}
BENCHMARK(BM_DotProduct); // NOLINT


// Distance
static void BM_Distance(State& state)
{
	for (auto _ : state) {
		auto a = Vec<3, f32>{ 1.f, 3.f, 4.f };
		auto b = Vec<3, f32>{ 2.f, -5.f, 8.f };
		auto result = Vec<3, f32>::dist(a, b);
	}
}
BENCHMARK(BM_Distance); // NOLINT


BENCHMARK_MAIN(); // NOLINT
