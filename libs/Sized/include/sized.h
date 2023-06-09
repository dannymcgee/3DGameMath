#pragma once

#include <cstdint>

#ifndef FLOAT_PRECISION
	#if INTPTR_MAX == INT32_MAX
		#define FLOAT_PRECISION 32
	#elif INTPTR_MAX == INT64_MAX
		#define FLOAT_PRECISION 64
	#endif
#endif

namespace sized {

using i8 = int8_t;
using u8 = uint8_t;

using i16 = int16_t;
using u16 = uint16_t;

using i32 = int32_t;
using u32 = uint32_t;

using i64 = int64_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

#if FLOAT_PRECISION == 32
using flt = f32;
#elif FLOAT_PRECISION == 64
using flt = f64;
#endif

using usize = size_t;

constexpr auto operator""_i8(usize value) -> i8 { return static_cast<i8>(value); }
constexpr auto operator""_u8(usize value) -> u8 { return static_cast<u8>(value); }

constexpr auto operator""_i16(usize value) -> i16 { return static_cast<i16>(value); }
constexpr auto operator""_u16(usize value) -> u16 { return static_cast<u16>(value); }

constexpr auto operator""_i32(usize value) -> i32 { return static_cast<i32>(value); }
constexpr auto operator""_u32(usize value) -> u32 { return static_cast<u32>(value); }

constexpr auto operator""_i64(usize value) -> i64 { return static_cast<i64>(value); }
constexpr auto operator""_u64(usize value) -> u64 { return static_cast<u64>(value); }

constexpr auto operator""_flt(usize value) -> flt { return static_cast<flt>(value); }
constexpr auto operator""_flt(long double value) -> flt { return static_cast<flt>(value); }

constexpr auto operator""_usize(usize value) -> usize { return value; }

}
