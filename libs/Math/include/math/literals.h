#pragma once

#include <sized.h>

namespace math::literals {

auto operator""_deg(long double value) -> sized::f64;
auto operator""_deg(sized::usize value) -> sized::f64;
auto operator""_rad(long double value) -> sized::f64;
auto operator""_rad(sized::usize value) -> sized::f64;

} // namespace math::literals
