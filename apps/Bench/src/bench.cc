#include <benchmark/benchmark.h>

#include <math/vector.h>
#include <sized.h>

using namespace sized; // NOLINT
using benchmark::State;

using Vec3f = math::Vector<3, f32>;
using Vec3 = math::Vector<3, f64>;

// Constructor (sanity check)
static void BM_Ctor_f32(State& state)
{
	for (auto _ : state)
		auto _vec = Vec3f{ 1, 2, 3 };
}
static void BM_Ctor_f64(State& state)
{
	for (auto _ : state)
		auto _vec = Vec3{ 1, 2, 3 };
}
BENCHMARK(BM_Ctor_f32); // NOLINT
BENCHMARK(BM_Ctor_f64); // NOLINT


// Cross-Product
static void BM_CrossProduct_f32(State& state)
{
	for (auto _ : state) {
		auto a = Vec3f{ 1.f, 3.f, 4.f };
		auto b = Vec3f{ 2.f, -5.f, 8.f };
		auto result = a.cross(b);
	}
}
static void BM_CrossProduct_f64(State& state)
{
	for (auto _ : state) {
		auto a = Vec3{ 1.0, 3.0, 4.0 };
		auto b = Vec3{ 2.0, -5.0, 8.0 };
		auto result = a.cross(b);
	}
}
BENCHMARK(BM_CrossProduct_f32); // NOLINT
BENCHMARK(BM_CrossProduct_f64); // NOLINT


// Magnitude
static void BM_Magnitude_f32(State& state)
{
	for (auto _ : state) {
		auto vec = Vec3f{ 1.f, 2.f, 3.f };
		auto result = vec.magnitude();
	}
}
static void BM_Magnitude_f64(State& state)
{
	for (auto _ : state) {
		auto vec = Vec3{ 1.0, 2.0, 3.0 };
		auto result = vec.magnitude();
	}
}
BENCHMARK(BM_Magnitude_f32); // NOLINT
BENCHMARK(BM_Magnitude_f64); // NOLINT


// Unit
static void BM_Unit_f32(State& state)
{
	for (auto _ : state) {
		auto vec = Vec3f{ 1.f, 2.f, 3.f };
		auto result = vec.unit();
	}
}
static void BM_Unit_f64(State& state)
{
	for (auto _ : state) {
		auto vec = Vec3{ 1.0, 2.0, 3.0 };
		auto result = vec.unit();
	}
}
BENCHMARK(BM_Unit_f32); // NOLINT
BENCHMARK(BM_Unit_f64); // NOLINT


// Distance
static void BM_Distance_f32(State& state)
{
	for (auto _ : state) {
		auto a = Vec3f{ 1.f, 3.f, 4.f };
		auto b = Vec3f{ 2.f, -5.f, 8.f };
		auto result = Vec3f::dist(a, b);
	}
}
static void BM_Distance_f64(State& state)
{
	for (auto _ : state) {
		auto a = Vec3{ 1.0, 3.0, 4.0 };
		auto b = Vec3{ 2.0, -5.0, 8.0 };
		auto result = Vec3::dist(a, b);
	}
}
BENCHMARK(BM_Distance_f32); // NOLINT
BENCHMARK(BM_Distance_f64); // NOLINT


// Dot-Product
static void BM_DotProduct_f32(State& state)
{
	for (auto _ : state) {
		auto a = Vec3f{ 1.f, 3.f, 4.f };
		auto b = Vec3f{ 2.f, -5.f, 8.f };
		auto result = a.dot(b);
	}
}
static void BM_DotProduct_f64(State& state)
{
	for (auto _ : state) {
		auto a = Vec3{ 1.0, 3.0, 4.0 };
		auto b = Vec3{ 2.0, -5.0, 8.0 };
		auto result = a.dot(b);
	}
}
BENCHMARK(BM_DotProduct_f32); // NOLINT
BENCHMARK(BM_DotProduct_f64); // NOLINT


BENCHMARK_MAIN(); // NOLINT
