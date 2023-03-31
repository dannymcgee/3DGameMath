#include <cstdlib>
#include <limits>

#include <catch2/catch_all.hpp>
#include <fmt/format.h>

#include <math/vector.h>
#include <math/matrix.h>
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

TEST_CASE("math::Mat<R,C,T>", "[matrix]") {
	using namespace sized; // NOLINT

	SECTION("Mat<R=4, C=3, T=f64>") {
		using Mat4x3 = math::Mat<4,3, f64>;

		auto mat4x3_labeled = Mat4x3{
			{ 11, 12, 13 },
			{ 21, 22, 23 },
			{ 31, 32, 33 },
			{ 41, 42, 43 },
		};

		SECTION("supports member access with 2D index") {
			REQUIRE(math::nearly_equal(mat4x3_labeled.m<11>(), 11.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m<12>(), 12.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m<13>(), 13.0));

			REQUIRE(math::nearly_equal(mat4x3_labeled.m<21>(), 21.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m<22>(), 22.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m<23>(), 23.0));

			REQUIRE(math::nearly_equal(mat4x3_labeled.m<31>(), 31.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m<32>(), 32.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m<33>(), 33.0));

			REQUIRE(math::nearly_equal(mat4x3_labeled.m<41>(), 41.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m<42>(), 42.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m<43>(), 43.0));
		}
		SECTION("supports getting an individual row as a math::Vec<C>") {
			auto row2 = mat4x3_labeled.row<2>();

			REQUIRE(math::nearly_equal(row2.x, 21.0));
			REQUIRE(math::nearly_equal(row2.y, 22.0));
			REQUIRE(math::nearly_equal(row2.z, 23.0));
		}
		SECTION("supports getting an individual column as a math::Vec<R>") {
			auto col2 = mat4x3_labeled.col<2>();

			REQUIRE(math::nearly_equal(col2.x, 12.0));
			REQUIRE(math::nearly_equal(col2.y, 22.0));
			REQUIRE(math::nearly_equal(col2.z, 32.0));
			REQUIRE(math::nearly_equal(col2.w, 42.0));
		}
		SECTION("supports matrix transposition") {
			auto mat3x4 = mat4x3_labeled.transpose();

			REQUIRE(math::nearly_equal(mat3x4.m<11>(), 11.0));
			REQUIRE(math::nearly_equal(mat3x4.m<12>(), 21.0));
			REQUIRE(math::nearly_equal(mat3x4.m<13>(), 31.0));
			REQUIRE(math::nearly_equal(mat3x4.m<14>(), 41.0));

			REQUIRE(math::nearly_equal(mat3x4.m<21>(), 12.0));
			REQUIRE(math::nearly_equal(mat3x4.m<22>(), 22.0));
			REQUIRE(math::nearly_equal(mat3x4.m<23>(), 32.0));
			REQUIRE(math::nearly_equal(mat3x4.m<24>(), 42.0));

			REQUIRE(math::nearly_equal(mat3x4.m<31>(), 13.0));
			REQUIRE(math::nearly_equal(mat3x4.m<32>(), 23.0));
			REQUIRE(math::nearly_equal(mat3x4.m<33>(), 33.0));
			REQUIRE(math::nearly_equal(mat3x4.m<34>(), 43.0));
		}
		SECTION("supports scalar multiplication") {
			auto mat = Mat4x3{
				{  1,  2,  3 },
				{  4,  5,  6 },
				{  7,  8,  9 },
				{ 10, 11, 12 },
			};

			SECTION("...Mat * f64") {
				auto result = mat * 2.0;

				REQUIRE(math::nearly_equal(result.m<11>(), 2.0));
				REQUIRE(math::nearly_equal(result.m<12>(), 4.0));
				REQUIRE(math::nearly_equal(result.m<13>(), 6.0));

				REQUIRE(math::nearly_equal(result.m<21>(), 8.0));
				REQUIRE(math::nearly_equal(result.m<22>(), 10.0));
				REQUIRE(math::nearly_equal(result.m<23>(), 12.0));

				REQUIRE(math::nearly_equal(result.m<31>(), 14.0));
				REQUIRE(math::nearly_equal(result.m<32>(), 16.0));
				REQUIRE(math::nearly_equal(result.m<33>(), 18.0));

				REQUIRE(math::nearly_equal(result.m<41>(), 20.0));
				REQUIRE(math::nearly_equal(result.m<42>(), 22.0));
				REQUIRE(math::nearly_equal(result.m<43>(), 24.0));
			}
			SECTION("...f64 * Mat") {
				auto result = 2.0 * mat;

				REQUIRE(math::nearly_equal(result.m<11>(), 2.0));
				REQUIRE(math::nearly_equal(result.m<12>(), 4.0));
				REQUIRE(math::nearly_equal(result.m<13>(), 6.0));

				REQUIRE(math::nearly_equal(result.m<21>(), 8.0));
				REQUIRE(math::nearly_equal(result.m<22>(), 10.0));
				REQUIRE(math::nearly_equal(result.m<23>(), 12.0));

				REQUIRE(math::nearly_equal(result.m<31>(), 14.0));
				REQUIRE(math::nearly_equal(result.m<32>(), 16.0));
				REQUIRE(math::nearly_equal(result.m<33>(), 18.0));

				REQUIRE(math::nearly_equal(result.m<41>(), 20.0));
				REQUIRE(math::nearly_equal(result.m<42>(), 22.0));
				REQUIRE(math::nearly_equal(result.m<43>(), 24.0));
			}
			SECTION("...Mat *= f64") {
				auto result = mat;
				result *= 2.0;

				REQUIRE(math::nearly_equal(result.m<11>(), 2.0));
				REQUIRE(math::nearly_equal(result.m<12>(), 4.0));
				REQUIRE(math::nearly_equal(result.m<13>(), 6.0));

				REQUIRE(math::nearly_equal(result.m<21>(), 8.0));
				REQUIRE(math::nearly_equal(result.m<22>(), 10.0));
				REQUIRE(math::nearly_equal(result.m<23>(), 12.0));

				REQUIRE(math::nearly_equal(result.m<31>(), 14.0));
				REQUIRE(math::nearly_equal(result.m<32>(), 16.0));
				REQUIRE(math::nearly_equal(result.m<33>(), 18.0));

				REQUIRE(math::nearly_equal(result.m<41>(), 20.0));
				REQUIRE(math::nearly_equal(result.m<42>(), 22.0));
				REQUIRE(math::nearly_equal(result.m<43>(), 24.0));
			}
		}
		SECTION("supports vector multiplication") {
			SECTION("Vec<4>(as Mat1x4) * Mat4x3") {
				auto result = math::Vec<4>{ 1.0, 2.0, 3.0, 4.0 } * mat4x3_labeled;
				auto expected = math::Vec<3>{
					1*11 + 2*21 + 3*31 + 4*41,
					1*12 + 2*22 + 3*32 + 4*42,
					1*13 + 2*23 + 3*33 + 4*43,
				};

				REQUIRE(math::nearly_equal(result.x, expected.x));
				REQUIRE(math::nearly_equal(result.y, expected.y));
				REQUIRE(math::nearly_equal(result.z, expected.z));
			}
			SECTION("Mat4x3 * Vec<3>(as Mat3x1)") {
				auto result = mat4x3_labeled * math::Vec<3>{ 1.0, 2.0, 3.0 };
				auto expected = math::Vec<4>{
					1*11 + 2*12 + 3*13,
					1*21 + 2*22 + 3*23,
					1*31 + 2*32 + 3*33,
					1*41 + 2*42 + 3*43,
				};

				REQUIRE(math::nearly_equal(result.x, expected.x));
				REQUIRE(math::nearly_equal(result.y, expected.y));
				REQUIRE(math::nearly_equal(result.z, expected.z));
				REQUIRE(math::nearly_equal(result.w, expected.w));
			}
		}
	}
	SECTION("Mat<R=2, C=2, T=f64>") {
		using Mat2x2 = math::Mat<2,2, f64>;

		SECTION("supports matrix multiplication") {
			auto a = Mat2x2{
				{ -3.0, 0.0 },
				{  5.0, 0.5 },
			};
			auto b = Mat2x2{
				{ -7.0, 2.0 },
				{  4.0, 6.0 },
			};

			auto ab = a * b;

			REQUIRE(math::nearly_equal(ab.m<11>(), 21.0));
			REQUIRE(math::nearly_equal(ab.m<12>(), -6.0));
			REQUIRE(math::nearly_equal(ab.m<21>(), -33.0));
			REQUIRE(math::nearly_equal(ab.m<22>(), 13.0));
		}
	}
}
