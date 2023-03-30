#include <cstdlib>
#include <limits>

#include <catch2/catch_all.hpp>
#include <fmt/format.h>

#include <math/vector.h>
#include <sized.h>

TEST_CASE("math::Vec", "[vector]") {
	using namespace sized; // NOLINT

	SECTION("Vec<3, f32>") {
		using Vec3 = math::Vec<3, f32>;

		static constexpr auto epsilon_practical = 0.00001f;

		SECTION("supports construction with initializer list") {
			auto vec = Vec3{ 1.f, 2.f, 3.f };

			REQUIRE(vec.x == 1.f);
			REQUIRE(vec.y == 2.f);
			REQUIRE(vec.z == 3.f);
		}
		SECTION("supports structured binding") {
			auto [x,y,z] = Vec3{ 1.f, 2.f, 3.f };

			REQUIRE(x == 1.f);
			REQUIRE(y == 2.f);
			REQUIRE(z == 3.f);
		}
		SECTION("has a ::Zero const static member") {
			auto vec = Vec3::Zero;

			REQUIRE(vec.x == 0.f);
			REQUIRE(vec.y == 0.f);
			REQUIRE(vec.z == 0.f);
		}
		SECTION("support unary negation operator") {
			auto pos = Vec3{ 1.f, 2.f, 3.f };
			auto neg = -pos;

			REQUIRE(math::nearly_equal<f32>(neg.x, -1.f));
			REQUIRE(math::nearly_equal<f32>(neg.y, -2.f));
			REQUIRE(math::nearly_equal<f32>(neg.z, -3.f));
		}
		SECTION("supports Vec + Vec operator...") {
			auto lhs = Vec3{ 1.f, 2.f, 3.f };

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 1.f, 2.f, 3.f };
				auto result = lhs + rhs;

				REQUIRE(result.x == 2.f);
				REQUIRE(result.y == 4.f);
				REQUIRE(result.z == 6.f);
			}
			SECTION("...with rvalue rhs") {
				auto result = lhs + Vec3{ 1.f, 2.f, 3.f };

				REQUIRE(result.x == 2.f);
				REQUIRE(result.y == 4.f);
				REQUIRE(result.z == 6.f);
			}
		}
		SECTION("supports Vec += Vec operator...") {
			auto result = Vec3{ 1.f, 2.f, 3.f };

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 1.f, 2.f, 3.f };
				result += rhs;

				REQUIRE(result.x == 2.f);
				REQUIRE(result.y == 4.f);
				REQUIRE(result.z == 6.f);
			}
			SECTION("...with rvalue rhs") {
				result += Vec3{ 1.f, 2.f, 3.f };

				REQUIRE(result.x == 2.f);
				REQUIRE(result.y == 4.f);
				REQUIRE(result.z == 6.f);
			}
		}
		SECTION("supports Vec - Vec operator...") {
			auto lhs = Vec3{ 1.f, 2.f, 3.f };

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 3.f, 2.f, 1.f };
				auto result = lhs - rhs;

				REQUIRE(math::nearly_equal<f32>(result.x, -2.f));
				REQUIRE(result.y == 0.f);
				REQUIRE(result.z == 2.f);
			}

			SECTION("...with rvalue rhs") {
				auto result = lhs - Vec3{ 3.f, 2.f, 1.f };

				REQUIRE(math::nearly_equal<f32>(result.x, -2.f));
				REQUIRE(result.y == 0.f);
				REQUIRE(result.z == 2.f);
			}
		}
		SECTION("supports Vec -= Vec operator...") {
			auto result = Vec3{ 1.f, 2.f, 3.f };

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 3.f, 2.f, 1.f };
				result -= rhs;

				REQUIRE(math::nearly_equal<f32>(result.x, -2.f));
				REQUIRE(result.y == 0.f);
				REQUIRE(result.z == 2.f);
			}
			SECTION("...with rvalue rhs") {
				result -= Vec3{ 3.f, 2.f, 1.f };

				REQUIRE(math::nearly_equal<f32>(result.x, -2.f));
				REQUIRE(result.y == 0.f);
				REQUIRE(result.z == 2.f);
			}
		}
		SECTION("supports Vec * f32 operator") {
			auto lhs = Vec3{ 1.f, 2.f, 3.f };
			auto result = lhs * 2.f;

			REQUIRE(result.x == 2.f);
			REQUIRE(result.y == 4.f);
			REQUIRE(result.z == 6.f);
		}
		SECTION("supports Vec *= f32 operator") {
			auto result = Vec3{ 1.f, 2.f, 3.f };
			result *= 2.f;

			REQUIRE(result.x == 2.f);
			REQUIRE(result.y == 4.f);
			REQUIRE(result.z == 6.f);
		}
		SECTION("supports f32 * Vec operator...") {
			auto lhs = 2.f;

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 1.f, 2.f, 3.f };
				auto result = lhs * rhs;

				REQUIRE(result.x == 2.f);
				REQUIRE(result.y == 4.f);
				REQUIRE(result.z == 6.f);
			}
			SECTION("...with rvalue rhs") {
				auto result = lhs * Vec3{ 1.f, 2.f, 3.f };

				REQUIRE(result.x == 2.f);
				REQUIRE(result.y == 4.f);
				REQUIRE(result.z == 6.f);
			}
		}
		SECTION("supports Vec / f32 operator") {
			auto lhs = Vec3{ 1.f, 2.f, 3.f };
			auto result = lhs / 2.f;

			REQUIRE(result.x == 0.5f);
			REQUIRE(result.y == 1.f);
			REQUIRE(result.z == 1.5f);

			SECTION("avoids NaN") {
				auto imposibble = lhs / 0.f;

				REQUIRE(!std::isnan(imposibble.x));
				REQUIRE(!std::isnan(imposibble.y));
				REQUIRE(!std::isnan(imposibble.z));
			}
		}
		SECTION("supports Vec /= f32 operator") {
			auto result = Vec3{ 1.f, 2.f, 3.f };
			result /= 2.f;

			REQUIRE(result.x == 0.5f);
			REQUIRE(result.y == 1.f);
			REQUIRE(result.z == 1.5f);
		}
		SECTION("supports Vec == Vec operator...") {
			auto lhs = Vec3{ 1.f, 2.f, 3.f };

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 1.f, 2.f, 3.f };
				auto result = lhs == rhs;

				REQUIRE(result == true);
			}
			SECTION("... with rvalue rhs") {
				auto result = lhs == Vec3{ 1.f, 2.f, 3.f };

				REQUIRE(result == true);
			}
		}
		SECTION("supports Vec != Vec operator...") {
			auto lhs = Vec3{ 1.f, 2.f, 3.f };

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 1.1f, 2.1f, 3.1f };
				auto result = lhs != rhs;

				REQUIRE(result == true);
			}
			SECTION("...with rvalue rhs") {
				auto result = lhs != Vec3{ 1.1f, 2.1f, 3.1f };

				REQUIRE(result == true);
			}
		}
		SECTION("can calculate the vector length") {
			auto vec = Vec3{ 1.5f, 2.25f, 3.34f };
			auto expected = 4.29745f;

			auto length = vec.length();
			auto magnitude = vec.magnitude();

			REQUIRE(math::nearly_equal<f32>(length, expected, epsilon_practical));
			REQUIRE(math::nearly_equal<f32>(magnitude, expected, epsilon_practical));

			// Unit vector length == 1
			auto up = Vec3{ 0.f, 1.f, 0.f };
			REQUIRE(up.length() == 1.f);

			// Zero Vector length == 0
			REQUIRE(Vec3::Zero.length() == 0.f);

			// Vector length is always non-negative
			REQUIRE(Vec3{ -1.f, -2.f, -3.f }.length() > 0.f);
		}
		SECTION("can calculate its unit-length direction") {
			auto vec = Vec3{ 12.f, -5.f, 0.f };
			auto expected = Vec3{ 0.92308f, -0.38462f, 0.f };

			// `Unit()`
			auto unit = vec.unit();
			REQUIRE(math::nearly_equal<f32>(unit.x, expected.x, epsilon_practical));
			REQUIRE(math::nearly_equal<f32>(unit.y, expected.y, epsilon_practical));
			REQUIRE(math::nearly_equal<f32>(unit.z, expected.z, epsilon_practical));

			// `Normal()` alias
			auto norm = vec.normal();
			REQUIRE(math::nearly_equal<f32>(norm.x, unit.x));
			REQUIRE(math::nearly_equal<f32>(norm.y, unit.y));
			REQUIRE(math::nearly_equal<f32>(norm.z, unit.z));

			// `Direction()` alias
			auto dir = vec.direction();
			REQUIRE(math::nearly_equal<f32>(dir.x, unit.x));
			REQUIRE(math::nearly_equal<f32>(dir.y, unit.y));
			REQUIRE(math::nearly_equal<f32>(dir.z, unit.z));

			SECTION("in-place") {
				vec.normalize();
				REQUIRE(math::nearly_equal<f32>(vec.x, unit.x));
				REQUIRE(math::nearly_equal<f32>(vec.y, unit.y));
				REQUIRE(math::nearly_equal<f32>(vec.z, unit.z));
			}

			SECTION("avoids NaN") {
				auto impossible = Vec3::Zero.unit();
				REQUIRE(!std::isnan(impossible.x));
				REQUIRE(!std::isnan(impossible.y));
				REQUIRE(!std::isnan(impossible.z));
			}
		}
		SECTION("can calculate the distance to another point") {
			auto expected = 2.828427f;

			SECTION("static method") {
				SECTION("rvalue, rvalue") {
					auto result = Vec3::dist({ 1, 2, 3 }, { 3, 2, 1 });

					REQUIRE(math::nearly_equal<f32>(result, expected, epsilon_practical));
				}
				SECTION("lvalue, lvalue") {
					auto vec1 = Vec3{ 1, 2, 3 };
					auto vec2 = Vec3{ 3, 2, 1 };
					auto result = Vec3::dist(vec1, vec2);

					REQUIRE(math::nearly_equal<f32>(result, expected, epsilon_practical));
				}
				SECTION("lvalue, rvalue") {
					auto vec1 = Vec3{ 1, 2, 3 };
					auto result = Vec3::dist(vec1, { 3, 2, 1 });

					REQUIRE(math::nearly_equal<f32>(result, expected, epsilon_practical));
				}
				SECTION("rvalue, lvalue") {
					auto vec2 = Vec3{ 3, 2, 1 };
					auto result = Vec3::dist({ 1, 2, 3 }, vec2);

					REQUIRE(math::nearly_equal<f32>(result, expected, epsilon_practical));
				}
			}
			SECTION("instance method") {
				auto vec1 = Vec3{ 1, 2, 3 };

				SECTION("lvalue arg") {
					auto vec2 = Vec3{ 3, 2, 1 };
					auto result = vec1.dist(vec2);

					REQUIRE(math::nearly_equal<f32>(result, expected, epsilon_practical));
				}
				SECTION("rvalue arg") {
					auto result = vec1.dist({ 3, 2, 1 });

					REQUIRE(math::nearly_equal<f32>(result, expected, epsilon_practical));
				}
			}
		}
		SECTION("can calculate the dot-product with another vector") {
			auto vec1 = Vec3{ 3.f, -2.f, 7.f };
			auto vec2 = Vec3{ 0.f, 4.f, -1.f };
			auto expected = -15.f;

			SECTION("rvalue rhs") {
				REQUIRE(math::nearly_equal<f32>(vec1.dot({ 0.f, 4.f, -1.f }), expected));
				REQUIRE(math::nearly_equal<f32>((vec1 | Vec3{ 0.f, 4.f, -1.f }), expected));
			}
			SECTION("lvalue rhs") {
				REQUIRE(math::nearly_equal<f32>(vec1.dot(vec2), expected));
				REQUIRE(math::nearly_equal<f32>((vec1 | vec2), expected));
			}
		}
		SECTION("can calculate the cross-product with another vector") {
			auto vec1 = Vec3{ 1.f, 3.f, 4.f };
			auto vec2 = Vec3{ 2.f, -5.f, 8.f };
			auto expected = Vec3{ 44.f, 0.f, -11.f };

			SECTION("named method with rvalue arg") {
				auto result = vec1.cross({ 2.f, -5.f, 8.f });

				REQUIRE(math::nearly_equal<f32>(result.x, expected.x));
				REQUIRE(math::nearly_equal<f32>(result.y, expected.y));
				REQUIRE(math::nearly_equal<f32>(result.z, expected.z));
			}
			SECTION("named method with lvalue arg") {
				auto result = vec1.cross(vec2);

				REQUIRE(math::nearly_equal<f32>(result.x, expected.x));
				REQUIRE(math::nearly_equal<f32>(result.y, expected.y));
				REQUIRE(math::nearly_equal<f32>(result.z, expected.z));
			}
			SECTION("^ operator with rvalue rhs") {
				auto result = vec1 ^ Vec3{ 2.f, -5.f, 8.f };

				REQUIRE(math::nearly_equal<f32>(result.x, expected.x));
				REQUIRE(math::nearly_equal<f32>(result.y, expected.y));
				REQUIRE(math::nearly_equal<f32>(result.z, expected.z));
			}
			SECTION("^ operator with lvalue rhs") {
				auto result = vec1 ^ vec2;

				REQUIRE(math::nearly_equal<f32>(result.x, expected.x));
				REQUIRE(math::nearly_equal<f32>(result.y, expected.y));
				REQUIRE(math::nearly_equal<f32>(result.z, expected.z));
			}
			SECTION("^= operator with rvalue rhs") {
				auto result = vec1;
				result ^= Vec3{ 2.f, -5.f, 8.f };

				REQUIRE(math::nearly_equal<f32>(result.x, expected.x));
				REQUIRE(math::nearly_equal<f32>(result.y, expected.y));
				REQUIRE(math::nearly_equal<f32>(result.z, expected.z));
			}
			SECTION("^= operator with lvalue rhs") {
				auto result = vec1;
				result ^= vec2;

				REQUIRE(math::nearly_equal<f32>(result.x, expected.x));
				REQUIRE(math::nearly_equal<f32>(result.y, expected.y));
				REQUIRE(math::nearly_equal<f32>(result.z, expected.z));
			}
		}
	}
}
