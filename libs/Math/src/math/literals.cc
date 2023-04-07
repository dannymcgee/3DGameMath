#include "math/literals.h"
#include "math/utility.h"

namespace math::literals {

auto operator""_deg(long double value) -> sized::f64
{
	return math::deg2rad(static_cast<sized::f64>(value));
}

auto operator""_deg(sized::usize value) -> sized::f64
{
	return math::deg2rad(static_cast<sized::f64>(value));
}

auto operator""_rad(long double value) -> sized::f64
{
	return static_cast<sized::f64>(value);
}

auto operator""_rad(sized::usize value) -> sized::f64
{
	return static_cast<sized::f64>(value);
}

} // namespace math::literals
