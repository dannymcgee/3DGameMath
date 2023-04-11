#include <array>
#include <cstdlib>
#include <limits>

#include <catch2/catch_all.hpp>
#include <fmt/format.h>

#include <math/euler.h>
#include <math/geo/circle.h>
#include <math/geo/plane.h>
#include <math/geo/tri.h>
#include <math/literals.h>
#include <math/matrix.h>
#include <math/matrix/rotation.h>
#include <math/quat.h>
#include <math/spaces.h>
#include <math/utility.h>
#include <math/vector.h>
#include <sized.h>

using Catch::Matchers::WithinAbs;
using Catch::Matchers::WithinRel;

using math::Vec3;
using math::Vec4;

using math::Mat2x2;
using math::Mat3x3;
using math::Mat4x4;
using math::Mat4x3;

TEST_CASE("math::Vector", "[vector]") {
	using namespace sized; // NOLINT

	SECTION("Vector<3, f32>") {
		using Vec3 = math::Vector<3,f32>;  // NOLINT(clang-diagnostic-shadow)

		static constexpr f32 epsilon_practical = 0.00001f;

		SECTION("supports construction with initializer list") {
			auto vec = Vec3{ 1.f, 2.f, 3.f };

			CHECK(vec.x == 1.f);
			CHECK(vec.y == 2.f);
			CHECK(vec.z == 3.f);
		}
		SECTION("supports structured binding") {
			auto [x,y,z] = Vec3{ 1.f, 2.f, 3.f };

			CHECK(x == 1.f);
			CHECK(y == 2.f);
			CHECK(z == 3.f);
		}
		SECTION("has a ::Zero const static member") {
			auto vec = Vec3::Zero;

			CHECK(vec.x == 0.f);
			CHECK(vec.y == 0.f);
			CHECK(vec.z == 0.f);
		}
		SECTION("support unary negation operator") {
			auto pos = Vec3{ 1.f, 2.f, 3.f };
			auto neg = -pos;

			CHECK_THAT(neg.x, WithinRel(-1.f));
			CHECK_THAT(neg.y, WithinRel(-2.f));
			CHECK_THAT(neg.z, WithinRel(-3.f));
		}
		SECTION("supports Vec + Vec operator...") {
			auto lhs = Vec3{ 1.f, 2.f, 3.f };

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 1.f, 2.f, 3.f };
				auto result = lhs + rhs;

				CHECK(result.x == 2.f);
				CHECK(result.y == 4.f);
				CHECK(result.z == 6.f);
			}
			SECTION("...with rvalue rhs") {
				auto result = lhs + Vec3{ 1.f, 2.f, 3.f };

				CHECK(result.x == 2.f);
				CHECK(result.y == 4.f);
				CHECK(result.z == 6.f);
			}
		}
		SECTION("supports Vec += Vec operator...") {
			auto result = Vec3{ 1.f, 2.f, 3.f };

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 1.f, 2.f, 3.f };
				result += rhs;

				CHECK(result.x == 2.f);
				CHECK(result.y == 4.f);
				CHECK(result.z == 6.f);
			}
			SECTION("...with rvalue rhs") {
				result += Vec3{ 1.f, 2.f, 3.f };

				CHECK(result.x == 2.f);
				CHECK(result.y == 4.f);
				CHECK(result.z == 6.f);
			}
		}
		SECTION("supports Vec - Vec operator...") {
			auto lhs = Vec3{ 1.f, 2.f, 3.f };

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 3.f, 2.f, 1.f };
				auto result = lhs - rhs;

				CHECK_THAT(result.x, WithinRel(-2.f));
				CHECK(result.y == 0.f);
				CHECK(result.z == 2.f);
			}

			SECTION("...with rvalue rhs") {
				auto result = lhs - Vec3{ 3.f, 2.f, 1.f };

				CHECK_THAT(result.x, WithinRel(-2.f));
				CHECK(result.y == 0.f);
				CHECK(result.z == 2.f);
			}
		}
		SECTION("supports Vec -= Vec operator...") {
			auto result = Vec3{ 1.f, 2.f, 3.f };

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 3.f, 2.f, 1.f };
				result -= rhs;

				CHECK_THAT(result.x, WithinRel(-2.f));
				CHECK(result.y == 0.f);
				CHECK(result.z == 2.f);
			}
			SECTION("...with rvalue rhs") {
				result -= Vec3{ 3.f, 2.f, 1.f };

				CHECK_THAT(result.x, WithinRel(-2.f));
				CHECK(result.y == 0.f);
				CHECK(result.z == 2.f);
			}
		}
		SECTION("supports Vec * f32 operator") {
			auto lhs = Vec3{ 1.f, 2.f, 3.f };
			auto result = lhs * 2.f;

			CHECK(result.x == 2.f);
			CHECK(result.y == 4.f);
			CHECK(result.z == 6.f);
		}
		SECTION("supports Vec *= f32 operator") {
			auto result = Vec3{ 1.f, 2.f, 3.f };
			result *= 2.f;

			CHECK(result.x == 2.f);
			CHECK(result.y == 4.f);
			CHECK(result.z == 6.f);
		}
		SECTION("supports f32 * Vec operator...") {
			auto lhs = 2.f;

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 1.f, 2.f, 3.f };
				auto result = lhs * rhs;

				CHECK(result.x == 2.f);
				CHECK(result.y == 4.f);
				CHECK(result.z == 6.f);
			}
			SECTION("...with rvalue rhs") {
				auto result = lhs * Vec3{ 1.f, 2.f, 3.f };

				CHECK(result.x == 2.f);
				CHECK(result.y == 4.f);
				CHECK(result.z == 6.f);
			}
		}
		SECTION("supports Vec / f32 operator") {
			auto lhs = Vec3{ 1.f, 2.f, 3.f };
			auto result = lhs / 2.f;

			CHECK(result.x == 0.5f);
			CHECK(result.y == 1.f);
			CHECK(result.z == 1.5f);

			SECTION("avoids NaN") {
				auto imposibble = lhs / 0.f;

				CHECK(!std::isnan(imposibble.x));
				CHECK(!std::isnan(imposibble.y));
				CHECK(!std::isnan(imposibble.z));
			}
		}
		SECTION("supports Vec /= f32 operator") {
			auto result = Vec3{ 1.f, 2.f, 3.f };
			result /= 2.f;

			CHECK(result.x == 0.5f);
			CHECK(result.y == 1.f);
			CHECK(result.z == 1.5f);
		}
		SECTION("supports Vec == Vec operator...") {
			auto lhs = Vec3{ 1.f, 2.f, 3.f };

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 1.f, 2.f, 3.f };
				auto result = lhs == rhs;

				CHECK(result == true);
			}
			SECTION("... with rvalue rhs") {
				auto result = lhs == Vec3{ 1.f, 2.f, 3.f };

				CHECK(result == true);
			}
		}
		SECTION("supports Vec != Vec operator...") {
			auto lhs = Vec3{ 1.f, 2.f, 3.f };

			SECTION("...with lvalue rhs") {
				auto rhs = Vec3{ 1.1f, 2.1f, 3.1f };
				auto result = lhs != rhs;

				CHECK(result == true);
			}
			SECTION("...with rvalue rhs") {
				auto result = lhs != Vec3{ 1.1f, 2.1f, 3.1f };

				CHECK(result == true);
			}
		}
		SECTION("can calculate the vector length") {
			auto vec = Vec3{ 1.5f, 2.25f, 3.34f };
			auto expected = 4.29745f;

			auto length = vec.length();
			auto magnitude = vec.magnitude();

			CHECK_THAT(length, WithinRel(expected, epsilon_practical));
			CHECK_THAT(magnitude, WithinRel(expected, epsilon_practical));

			// Unit vector length == 1
			auto up = Vec3{ 0.f, 1.f, 0.f };
			CHECK(up.length() == 1.f);

			// Zero Vector length == 0
			CHECK(Vec3::Zero.length() == 0.f);

			// Vector length is always non-negative
			CHECK(Vec3{ -1.f, -2.f, -3.f }.length() > 0.f);
		}
		SECTION("can calculate its unit-length direction") {
			auto vec = Vec3{ 12.f, -5.f, 0.f };
			auto expected = Vec3{ 0.92308f, -0.38462f, 0.f };

			// `Unit()`
			auto unit = vec.unit();
			CHECK_THAT(unit.x, WithinAbs(expected.x, epsilon_practical));
			CHECK_THAT(unit.y, WithinAbs(expected.y, epsilon_practical));
			CHECK_THAT(unit.z, WithinAbs(expected.z, epsilon_practical));

			// `Normal()` alias
			auto norm = vec.normal();
			CHECK_THAT(norm.x, WithinRel(unit.x));
			CHECK_THAT(norm.y, WithinRel(unit.y));
			CHECK_THAT(norm.z, WithinRel(unit.z));

			// `Direction()` alias
			auto dir = vec.direction();
			CHECK_THAT(dir.x, WithinRel(unit.x));
			CHECK_THAT(dir.y, WithinRel(unit.y));
			CHECK_THAT(dir.z, WithinRel(unit.z));

			SECTION("in-place") {
				vec.normalize();
				CHECK_THAT(vec.x, WithinRel(unit.x));
				CHECK_THAT(vec.y, WithinRel(unit.y));
				CHECK_THAT(vec.z, WithinRel(unit.z));
			}

			SECTION("avoids NaN") {
				auto impossible = Vec3::Zero.unit();
				CHECK(!std::isnan(impossible.x));
				CHECK(!std::isnan(impossible.y));
				CHECK(!std::isnan(impossible.z));

				auto in_place = Vec3::Zero;
				in_place.normalize();
				CHECK(!std::isnan(in_place.x));
				CHECK(!std::isnan(in_place.y));
				CHECK(!std::isnan(in_place.z));
			}
		}
		SECTION("can calculate the length and direction as a single operation") {
			auto vec = Vec3{ 0.f, 10.f, 0.f };
			auto [length, direction] = vec.length_and_direction();

			CHECK_THAT(length, WithinRel(10.f));
			CHECK_THAT(direction.x, WithinRel(0.f));
			CHECK_THAT(direction.y, WithinRel(1.f));
			CHECK_THAT(direction.z, WithinRel(0.f));
		}
		SECTION("can calculate the distance to another point") {
			auto expected = 2.828427f;

			SECTION("static method") {
				SECTION("rvalue, rvalue") {
					auto result = Vec3::dist({ 1, 2, 3 }, { 3, 2, 1 });

					CHECK_THAT(result, WithinAbs(expected, epsilon_practical));
				}
				SECTION("lvalue, lvalue") {
					auto vec1 = Vec3{ 1, 2, 3 };
					auto vec2 = Vec3{ 3, 2, 1 };
					auto result = Vec3::dist(vec1, vec2);

					CHECK_THAT(result, WithinAbs(expected, epsilon_practical));
				}
				SECTION("lvalue, rvalue") {
					auto vec1 = Vec3{ 1, 2, 3 };
					auto result = Vec3::dist(vec1, { 3, 2, 1 });

					CHECK_THAT(result, WithinAbs(expected, epsilon_practical));
				}
				SECTION("rvalue, lvalue") {
					auto vec2 = Vec3{ 3, 2, 1 };
					auto result = Vec3::dist({ 1, 2, 3 }, vec2);

					CHECK_THAT(result, WithinAbs(expected, epsilon_practical));
				}
			}
			SECTION("instance method") {
				auto vec1 = Vec3{ 1, 2, 3 };

				SECTION("lvalue arg") {
					auto vec2 = Vec3{ 3, 2, 1 };
					auto result = vec1.dist(vec2);

					CHECK_THAT(result, WithinAbs(expected, epsilon_practical));
				}
				SECTION("rvalue arg") {
					auto result = vec1.dist({ 3, 2, 1 });

					CHECK_THAT(result, WithinAbs(expected, epsilon_practical));
				}
			}
		}
		SECTION("can calculate the dot-product with another vector") {
			auto vec1 = Vec3{ 3.f, -2.f, 7.f };
			auto vec2 = Vec3{ 0.f, 4.f, -1.f };
			auto expected = -15.f;

			SECTION("rvalue rhs") {
				CHECK_THAT(vec1.dot({ 0.f, 4.f, -1.f }), WithinRel(expected));
				CHECK_THAT((vec1 | Vec3{ 0.f, 4.f, -1.f }), WithinRel(expected));
			}
			SECTION("lvalue rhs") {
				CHECK_THAT(vec1.dot(vec2), WithinRel(expected));
				CHECK_THAT((vec1 | vec2), WithinRel(expected));
			}
		}
		SECTION("can calculate the cross-product with another vector") {
			auto vec1 = Vec3{ 1.f, 3.f, 4.f };
			auto vec2 = Vec3{ 2.f, -5.f, 8.f };
			auto expected = Vec3{ 44.f, 0.f, -11.f };

			SECTION("named method with rvalue arg") {
				auto result = vec1.cross({ 2.f, -5.f, 8.f });

				CHECK_THAT(result.x, WithinRel(expected.x));
				CHECK_THAT(result.y, WithinRel(expected.y));
				CHECK_THAT(result.z, WithinRel(expected.z));
			}
			SECTION("named method with lvalue arg") {
				auto result = vec1.cross(vec2);

				CHECK_THAT(result.x, WithinRel(expected.x));
				CHECK_THAT(result.y, WithinRel(expected.y));
				CHECK_THAT(result.z, WithinRel(expected.z));
			}
			SECTION("^ operator with rvalue rhs") {
				auto result = vec1 ^ Vec3{ 2.f, -5.f, 8.f };

				CHECK_THAT(result.x, WithinRel(expected.x));
				CHECK_THAT(result.y, WithinRel(expected.y));
				CHECK_THAT(result.z, WithinRel(expected.z));
			}
			SECTION("^ operator with lvalue rhs") {
				auto result = vec1 ^ vec2;

				CHECK_THAT(result.x, WithinRel(expected.x));
				CHECK_THAT(result.y, WithinRel(expected.y));
				CHECK_THAT(result.z, WithinRel(expected.z));
			}
			SECTION("^= operator with rvalue rhs") {
				auto result = vec1;
				result ^= Vec3{ 2.f, -5.f, 8.f };

				CHECK_THAT(result.x, WithinRel(expected.x));
				CHECK_THAT(result.y, WithinRel(expected.y));
				CHECK_THAT(result.z, WithinRel(expected.z));
			}
			SECTION("^= operator with lvalue rhs") {
				auto result = vec1;
				result ^= vec2;

				CHECK_THAT(result.x, WithinRel(expected.x));
				CHECK_THAT(result.y, WithinRel(expected.y));
				CHECK_THAT(result.z, WithinRel(expected.z));
			}
		}
	}
}

TEST_CASE("math::Matrix<R,C,T>", "[matrix]") {
	using namespace sized; // NOLINT

	SECTION("Mat2x2") {
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

			CHECK_THAT(ab.m11, WithinRel(21.0));
			CHECK_THAT(ab.m12, WithinRel(-6.0));
			CHECK_THAT(ab.m21, WithinRel(-33.0));
			CHECK_THAT(ab.m22, WithinRel(13.0));
		}
		SECTION("can compute the determinant") {
			auto a = Mat2x2{
				{  2, 1 },
				{ -1, 2 },
			};
			auto b = Mat2x2{
				{ -3, 4 },
				{  2, 5 },
			};

			CHECK_THAT(a.determinant(), WithinRel(5.0));
			CHECK_THAT(b.determinant(), WithinRel(-23.0));
		}
		SECTION("can compute the inverse") {
			auto mat = Mat2x2{
				{ -3.0, 4.0 },
				{  2.0, 5.0 },
			};
			auto inverted = mat.inverse();

			CHECK(inverted);

			auto id = mat * (*inverted);

			CHECK_THAT(id.m11, WithinRel(1.0));
			CHECK_THAT(id.m12, WithinRel(0.0));

			CHECK_THAT(id.m21, WithinRel(0.0));
			CHECK_THAT(id.m22, WithinRel(1.0));
		}
	}
	SECTION("Mat3x3") {
		[[maybe_unused]]
		auto mat3x3_labeled = Mat3x3{
			{ 11, 12, 13 },
			{ 21, 22, 23 },
			{ 31, 32, 33 },
		};

		SECTION("can compute the determinant") {
			auto mat = Mat3x3{
				{ -4, -3,  3 },
				{  0,  2, -2 },
				{  1,  4, -1 },
			};

			CHECK_THAT(mat.determinant(), WithinRel(-24.0));
		}
		SECTION("can compute the inverse") {
			auto mat = Mat3x3{
				{ -4.0, -3.0,  3.0 },
				{  0.0,  2.0, -2.0 },
				{  1.0,  4.0, -1.0 },
			};
			auto inverted = mat.inverse();

			CHECK(inverted);

			auto id = mat * (*inverted);

			CHECK_THAT(id.m11, WithinRel(1.0));
			CHECK_THAT(id.m12, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));
			CHECK_THAT(id.m13, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));

			CHECK_THAT(id.m21, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));
			CHECK_THAT(id.m22, WithinRel(1.0));
			CHECK_THAT(id.m23, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));

			CHECK_THAT(id.m31, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));
			CHECK_THAT(id.m32, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));
			CHECK_THAT(id.m33, WithinRel(1.0));
		}
	}
	SECTION("Mat4x4") {
		[[maybe_unused]]
		auto mat4x4_labeled = Mat4x4{
			{ 11, 12, 13, 14 },
			{ 21, 22, 23, 24 },
			{ 31, 32, 33, 34 },
			{ 41, 42, 43, 44 },
		};

		SECTION("can compute the determinant") {
			auto mat = Mat4x4{
				{ -4, -3,  3,  1 },
				{  0,  2, -2,  0 },
				{  1,  4, -1,  1 },
				{  0,  2, -2,  1 },
			};

			CHECK_THAT(mat.determinant(), WithinRel(-24.0));
		}
		SECTION("can compute the inverse") {
			auto mat = Mat4x4{
				{ -4.0, -3.0,  3.0,  1.0 },
				{  0.0,  2.0, -2.0,  0.0 },
				{  1.0,  4.0, -1.0,  1.0 },
				{  0.0,  2.0, -2.0,  1.0 },
			};
			auto inverted = mat.inverse();

			CHECK(inverted);

			auto id = mat * (*inverted);

			CHECK_THAT(id.m11, WithinRel(1.0));
			CHECK_THAT(id.m12, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));
			CHECK_THAT(id.m13, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));
			CHECK_THAT(id.m14, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));

			CHECK_THAT(id.m21, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));
			CHECK_THAT(id.m22, WithinRel(1.0));
			CHECK_THAT(id.m23, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));
			CHECK_THAT(id.m24, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));

			CHECK_THAT(id.m31, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));
			CHECK_THAT(id.m32, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));
			CHECK_THAT(id.m33, WithinRel(1.0));
			CHECK_THAT(id.m34, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));

			CHECK_THAT(id.m41, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));
			CHECK_THAT(id.m42, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));
			CHECK_THAT(id.m43, WithinAbs(0.0, std::numeric_limits<f64>::epsilon()));
			CHECK_THAT(id.m44, WithinRel(1.0));
		}
	}
	SECTION("Mat4x3") {
		auto mat4x3_labeled = Mat4x3{
			{ 11, 12, 13 },
			{ 21, 22, 23 },
			{ 31, 32, 33 },
			{ 41, 42, 43 },
		};

		SECTION("supports member access with 2D index") {
			CHECK_THAT(mat4x3_labeled.m11, WithinRel(11.0));
			CHECK_THAT(mat4x3_labeled.m12, WithinRel(12.0));
			CHECK_THAT(mat4x3_labeled.m13, WithinRel(13.0));

			CHECK_THAT(mat4x3_labeled.m21, WithinRel(21.0));
			CHECK_THAT(mat4x3_labeled.m22, WithinRel(22.0));
			CHECK_THAT(mat4x3_labeled.m23, WithinRel(23.0));

			CHECK_THAT(mat4x3_labeled.m31, WithinRel(31.0));
			CHECK_THAT(mat4x3_labeled.m32, WithinRel(32.0));
			CHECK_THAT(mat4x3_labeled.m33, WithinRel(33.0));

			CHECK_THAT(mat4x3_labeled.m41, WithinRel(41.0));
			CHECK_THAT(mat4x3_labeled.m42, WithinRel(42.0));
			CHECK_THAT(mat4x3_labeled.m43, WithinRel(43.0));
		}
		SECTION("supports getting an individual row as a math::Vec<C>") {
			auto row2 = mat4x3_labeled.row<2>();

			CHECK_THAT(row2.x, WithinRel(21.0));
			CHECK_THAT(row2.y, WithinRel(22.0));
			CHECK_THAT(row2.z, WithinRel(23.0));
		}
		SECTION("supports getting an individual column as a math::Vec<R>") {
			auto col2 = mat4x3_labeled.col<2>();

			CHECK_THAT(col2.x, WithinRel(12.0));
			CHECK_THAT(col2.y, WithinRel(22.0));
			CHECK_THAT(col2.z, WithinRel(32.0));
			CHECK_THAT(col2.w, WithinRel(42.0));
		}
		SECTION("supports matrix transposition") {
			auto mat3x4 = mat4x3_labeled.transpose();

			CHECK_THAT(mat3x4.m11, WithinRel(11.0));
			CHECK_THAT(mat3x4.m12, WithinRel(21.0));
			CHECK_THAT(mat3x4.m13, WithinRel(31.0));
			CHECK_THAT(mat3x4.m14, WithinRel(41.0));

			CHECK_THAT(mat3x4.m21, WithinRel(12.0));
			CHECK_THAT(mat3x4.m22, WithinRel(22.0));
			CHECK_THAT(mat3x4.m23, WithinRel(32.0));
			CHECK_THAT(mat3x4.m24, WithinRel(42.0));

			CHECK_THAT(mat3x4.m31, WithinRel(13.0));
			CHECK_THAT(mat3x4.m32, WithinRel(23.0));
			CHECK_THAT(mat3x4.m33, WithinRel(33.0));
			CHECK_THAT(mat3x4.m34, WithinRel(43.0));
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

				CHECK_THAT(result.m11, WithinRel(2.0));
				CHECK_THAT(result.m12, WithinRel(4.0));
				CHECK_THAT(result.m13, WithinRel(6.0));

				CHECK_THAT(result.m21, WithinRel(8.0));
				CHECK_THAT(result.m22, WithinRel(10.0));
				CHECK_THAT(result.m23, WithinRel(12.0));

				CHECK_THAT(result.m31, WithinRel(14.0));
				CHECK_THAT(result.m32, WithinRel(16.0));
				CHECK_THAT(result.m33, WithinRel(18.0));

				CHECK_THAT(result.m41, WithinRel(20.0));
				CHECK_THAT(result.m42, WithinRel(22.0));
				CHECK_THAT(result.m43, WithinRel(24.0));
			}
			SECTION("...f64 * Mat") {
				auto result = 2.0 * mat;

				CHECK_THAT(result.m11, WithinRel(2.0));
				CHECK_THAT(result.m12, WithinRel(4.0));
				CHECK_THAT(result.m13, WithinRel(6.0));

				CHECK_THAT(result.m21, WithinRel(8.0));
				CHECK_THAT(result.m22, WithinRel(10.0));
				CHECK_THAT(result.m23, WithinRel(12.0));

				CHECK_THAT(result.m31, WithinRel(14.0));
				CHECK_THAT(result.m32, WithinRel(16.0));
				CHECK_THAT(result.m33, WithinRel(18.0));

				CHECK_THAT(result.m41, WithinRel(20.0));
				CHECK_THAT(result.m42, WithinRel(22.0));
				CHECK_THAT(result.m43, WithinRel(24.0));
			}
			SECTION("...Mat *= f64") {
				auto result = mat;
				result *= 2.0;

				CHECK_THAT(result.m11, WithinRel(2.0));
				CHECK_THAT(result.m12, WithinRel(4.0));
				CHECK_THAT(result.m13, WithinRel(6.0));

				CHECK_THAT(result.m21, WithinRel(8.0));
				CHECK_THAT(result.m22, WithinRel(10.0));
				CHECK_THAT(result.m23, WithinRel(12.0));

				CHECK_THAT(result.m31, WithinRel(14.0));
				CHECK_THAT(result.m32, WithinRel(16.0));
				CHECK_THAT(result.m33, WithinRel(18.0));

				CHECK_THAT(result.m41, WithinRel(20.0));
				CHECK_THAT(result.m42, WithinRel(22.0));
				CHECK_THAT(result.m43, WithinRel(24.0));
			}
		}
		SECTION("supports vector multiplication") {
			SECTION("Vec4 [Mat1x4] * Mat4x3") {
				auto result = math::Vector<4>{ 1.0, 2.0, 3.0, 4.0 } * mat4x3_labeled;
				auto expected = math::Vector<3>{
					1*11 + 2*21 + 3*31 + 4*41,
					1*12 + 2*22 + 3*32 + 4*42,
					1*13 + 2*23 + 3*33 + 4*43,
				};

				CHECK_THAT(result.x, WithinRel(expected.x));
				CHECK_THAT(result.y, WithinRel(expected.y));
				CHECK_THAT(result.z, WithinRel(expected.z));
			}
			SECTION("Mat4x3 * [Mat3x1] Vec3") {
				auto result = mat4x3_labeled * math::Vector<3>{ 1.0, 2.0, 3.0 };
				auto expected = math::Vector<4>{
					1*11 + 2*12 + 3*13,
					1*21 + 2*22 + 3*23,
					1*31 + 2*32 + 3*33,
					1*41 + 2*42 + 3*43,
				};

				CHECK_THAT(result.x, WithinRel(expected.x));
				CHECK_THAT(result.y, WithinRel(expected.y));
				CHECK_THAT(result.z, WithinRel(expected.z));
				CHECK_THAT(result.w, WithinRel(expected.w));
			}
		}
	}
}

TEST_CASE("Orientation", "[orientation]") {
	using namespace sized; // NOLINT(*-using-namespace)
	using namespace math::literals; // NOLINT(*-using-namespace)

	using math::Axis;
	using math::Space;
	using Euler = math::Euler<f64>;
	using Quat = math::Quat<f64>;
	using RotationMatrix = math::RotationMatrix<f64>;

	constexpr f64 epsilon = 1e-13;

	SECTION("Euler Angles") {
		auto euler = Euler{
			45_deg,
			-15_deg,
			3.3_deg,
		};

		SECTION("Literal constructor") {
			CHECK_THAT(euler.yaw, WithinRel(45_deg));
			CHECK_THAT(euler.pitch, WithinRel(-15_deg));
			CHECK_THAT(euler.roll, WithinRel(3.3_deg));
		}
		SECTION("Matrix conversion") {
			SECTION("Local -> Parent") {
				auto result = euler.matrix(Space::Local2Parent);

				auto yaw_mat   = RotationMatrix(45_deg, Axis::Up);
				auto pitch_mat = RotationMatrix(-15_deg, Axis::Right);
				auto roll_mat  = RotationMatrix(3.3_deg, Axis::Forward);
				auto expected = roll_mat * pitch_mat * yaw_mat;

				CHECK_THAT(result.m11, WithinRel(expected.m11, epsilon));
				CHECK_THAT(result.m12, WithinRel(expected.m12, epsilon));
				CHECK_THAT(result.m13, WithinRel(expected.m13, epsilon));

				CHECK_THAT(result.m21, WithinRel(expected.m21, epsilon));
				CHECK_THAT(result.m22, WithinRel(expected.m22, epsilon));
				CHECK_THAT(result.m23, WithinRel(expected.m23, epsilon));

				CHECK_THAT(result.m31, WithinRel(expected.m31, epsilon));
				CHECK_THAT(result.m32, WithinRel(expected.m32, epsilon));
				CHECK_THAT(result.m33, WithinRel(expected.m33, epsilon));
			}
			SECTION("Parent -> Local") {
				auto result = euler.matrix(Space::Parent2Local);

				auto yaw_mat   = RotationMatrix(45_deg, Vec3::up());
				auto pitch_mat = RotationMatrix(-15_deg, Vec3::right());
				auto roll_mat  = RotationMatrix(3.3_deg, Vec3::forward());
				auto expected = (roll_mat * pitch_mat * yaw_mat).inverse();

				CHECK_THAT(result.m11, WithinRel(expected.m11, epsilon));
				CHECK_THAT(result.m12, WithinRel(expected.m12, epsilon));
				CHECK_THAT(result.m13, WithinRel(expected.m13, epsilon));

				CHECK_THAT(result.m21, WithinRel(expected.m21, epsilon));
				CHECK_THAT(result.m22, WithinRel(expected.m22, epsilon));
				CHECK_THAT(result.m23, WithinRel(expected.m23, epsilon));

				CHECK_THAT(result.m31, WithinRel(expected.m31, epsilon));
				CHECK_THAT(result.m32, WithinRel(expected.m32, epsilon));
				CHECK_THAT(result.m33, WithinRel(expected.m33, epsilon));
			}
		}
		SECTION("Quaternion conversion") {
			SECTION("Local -> Parent") {
				auto result = euler.quat(Space::Local2Parent);

				auto yaw_quat   = Quat::angle_axis(45_deg, Vec3::up());
				auto pitch_quat = Quat::angle_axis(-15_deg, Vec3::right());
				auto roll_quat  = Quat::angle_axis(3.3_deg, Vec3::forward());
				auto expected = yaw_quat * pitch_quat * roll_quat;

				CHECK_THAT(result.w, WithinRel(expected.w, epsilon));
				CHECK_THAT(result.x, WithinRel(expected.x, epsilon));
				CHECK_THAT(result.y, WithinRel(expected.y, epsilon));
				CHECK_THAT(result.z, WithinRel(expected.z, epsilon));
			}
			SECTION("Parent -> Local") {
				auto result = euler.quat(Space::Parent2Local);

				auto yaw_quat   = Quat::angle_axis(45_deg, Vec3::up());
				auto pitch_quat = Quat::angle_axis(-15_deg, Vec3::right());
				auto roll_quat  = Quat::angle_axis(3.3_deg, Vec3::forward());
				auto expected = (yaw_quat * pitch_quat * roll_quat).inverse();

				CHECK_THAT(result.w, WithinRel(expected.w, epsilon));
				CHECK_THAT(result.x, WithinRel(expected.x, epsilon));
				CHECK_THAT(result.y, WithinRel(expected.y, epsilon));
				CHECK_THAT(result.z, WithinRel(expected.z, epsilon));
			}
		}
	}
	SECTION("Quaternion") {
		auto yaw_45 = Quat::angle_axis(45_deg, Vec3::up());
		auto pitch_neg15 = Quat::angle_axis(-15_deg, Vec3::right());
		auto roll_3_3 = Quat::angle_axis(3.3_deg, Vec3::forward());

		auto quat = yaw_45 * pitch_neg15 * roll_3_3;

		SECTION("Angle/axis extraction") {

			auto [yaw_angle, yaw_axis] = yaw_45.angle_axis();
			CHECK_THAT(yaw_angle, WithinRel(45_deg, epsilon));
			CHECK_THAT(yaw_axis.x, WithinRel(0.0, epsilon));
			CHECK_THAT(yaw_axis.y, WithinRel(1.0, epsilon));
			CHECK_THAT(yaw_axis.z, WithinRel(0.0, epsilon));

			auto [pitch_angle, pitch_axis] = pitch_neg15.angle_axis();
			CHECK_THAT(pitch_angle, WithinRel(15_deg, epsilon));
			CHECK_THAT(pitch_axis.x, WithinRel(-1.0, epsilon));
			CHECK_THAT(pitch_axis.y, WithinRel(0.0, epsilon));
			CHECK_THAT(pitch_axis.z, WithinRel(0.0, epsilon));

			auto [roll_angle, roll_axis] = roll_3_3.angle_axis();
			CHECK_THAT(roll_angle, WithinRel(3.3_deg, epsilon));
			CHECK_THAT(roll_axis.x, WithinRel(0.0, epsilon));
			CHECK_THAT(roll_axis.y, WithinRel(0.0, epsilon));
			CHECK_THAT(roll_axis.z, WithinRel(1.0, epsilon));
		}
		SECTION("Euler conversion") {
			SECTION("Local -> Parent") {
				auto euler = quat.euler(Space::Local2Parent);

				CHECK_THAT(euler.yaw, WithinRel(45_deg, epsilon));
				CHECK_THAT(euler.pitch, WithinRel(-15_deg, epsilon));
				CHECK_THAT(euler.roll, WithinRel(3.3_deg, epsilon));
			}
			SECTION("Parent -> Local") {
				auto euler = quat.euler(Space::Parent2Local);

				// TODO: _Are_ these the expected values? I think so, but this is
				// just a snapshot of the current behavior, not externally verified
				CHECK_THAT(euler.yaw, WithinRel(-46.369_deg, 0.001));
				CHECK_THAT(euler.pitch, WithinRel(8.164_deg, 0.001));
				CHECK_THAT(euler.roll, WithinRel(-13.044_deg, 0.001));
			}
		}
		SECTION("Matrix conversion") {
			SECTION("Local -> Parent") {
				auto result = quat.matrix(Space::Local2Parent);

				auto yaw_mat   = RotationMatrix(45_deg, Axis::Up);
				auto pitch_mat = RotationMatrix(-15_deg, Axis::Right);
				auto roll_mat  = RotationMatrix(3.3_deg, Axis::Forward);
				auto expected = roll_mat * pitch_mat * yaw_mat;

				CHECK_THAT(result.m11, WithinRel(expected.m11, epsilon));
				CHECK_THAT(result.m12, WithinRel(expected.m12, epsilon));
				CHECK_THAT(result.m13, WithinRel(expected.m13, epsilon));

				CHECK_THAT(result.m21, WithinRel(expected.m21, epsilon));
				CHECK_THAT(result.m22, WithinRel(expected.m22, epsilon));
				CHECK_THAT(result.m23, WithinRel(expected.m23, epsilon));

				CHECK_THAT(result.m31, WithinRel(expected.m31, epsilon));
				CHECK_THAT(result.m32, WithinRel(expected.m32, epsilon));
				CHECK_THAT(result.m33, WithinRel(expected.m33, epsilon));
			}
			SECTION("Parent -> Local") {
				auto result = quat.matrix(Space::Parent2Local);

				auto yaw_mat   = RotationMatrix(45_deg, Vec3::up());
				auto pitch_mat = RotationMatrix(-15_deg, Vec3::right());
				auto roll_mat  = RotationMatrix(3.3_deg, Vec3::forward());
				auto expected = (roll_mat * pitch_mat * yaw_mat).inverse();

				CHECK_THAT(result.m11, WithinRel(expected.m11, epsilon));
				CHECK_THAT(result.m12, WithinRel(expected.m12, epsilon));
				CHECK_THAT(result.m13, WithinRel(expected.m13, epsilon));

				CHECK_THAT(result.m21, WithinRel(expected.m21, epsilon));
				CHECK_THAT(result.m22, WithinRel(expected.m22, epsilon));
				CHECK_THAT(result.m23, WithinRel(expected.m23, epsilon));

				CHECK_THAT(result.m31, WithinRel(expected.m31, epsilon));
				CHECK_THAT(result.m32, WithinRel(expected.m32, epsilon));
				CHECK_THAT(result.m33, WithinRel(expected.m33, epsilon));
			}
		}
	}
	SECTION("Matrix") {
		auto yaw_mat   = RotationMatrix(45_deg, Axis::Up);
		auto pitch_mat = RotationMatrix(-15_deg, Axis::Right);
		auto roll_mat  = RotationMatrix(3.3_deg, Axis::Forward);
		auto matrix = roll_mat * pitch_mat * yaw_mat;

		SECTION("Quaternion conversion") {
			auto result = matrix.quat();

			auto yaw_quat   = Quat::angle_axis(45_deg, Vec3::up());
			auto pitch_quat = Quat::angle_axis(-15_deg, Vec3::right());
			auto roll_quat  = Quat::angle_axis(3.3_deg, Vec3::forward());
			auto expected = yaw_quat * pitch_quat * roll_quat;

			CHECK_THAT(result.w, WithinRel(expected.w, epsilon));
			CHECK_THAT(result.x, WithinRel(expected.x, epsilon));
			CHECK_THAT(result.y, WithinRel(expected.y, epsilon));
			CHECK_THAT(result.z, WithinRel(expected.z, epsilon));
		}
		SECTION("Euler angles conversion") {
			auto result = matrix.euler();
			auto expected = Euler{ 45_deg, -15_deg, 3.3_deg };

			CHECK_THAT(result.yaw, WithinRel(expected.yaw, epsilon));
			CHECK_THAT(result.pitch, WithinRel(expected.pitch, epsilon));
			CHECK_THAT(result.roll, WithinRel(expected.roll, epsilon));
		}
	}
}

TEST_CASE("Geometric Primitives", "[geoprim]") {
	using namespace sized;
	using Circle = math::geo::Circle<f64>;
	using Plane = math::geo::Plane<f64>;
	using Tri = math::geo::Tri<f64>;

	SECTION("Triangle") {
		auto t = Tri{
			{ -1.0,  0.0, -0.5 },
			{  0.0,  0.0,  1.0 },
			{  1.0,  0.0, -0.5 },
		};

		SECTION("barycentric coordinates") {
			SECTION("vertices") {
				Vec3 v1 = t.bary2cart(1, 0, 0);
				Vec3 v2 = t.bary2cart(0, 1, 0);
				Vec3 v3 = t.bary2cart(0, 0, 1);

				CHECK_THAT(v1.x, WithinRel(t.v1.x));
				CHECK_THAT(v1.y, WithinRel(t.v1.y));
				CHECK_THAT(v1.z, WithinRel(t.v1.z));

				CHECK_THAT(v2.x, WithinRel(t.v2.x));
				CHECK_THAT(v2.y, WithinRel(t.v2.y));
				CHECK_THAT(v2.z, WithinRel(t.v2.z));

				CHECK_THAT(v3.x, WithinRel(t.v3.x));
				CHECK_THAT(v3.y, WithinRel(t.v3.y));
				CHECK_THAT(v3.z, WithinRel(t.v3.z));
			}
			SECTION("edge midpoints") {
				Vec3 e1_mid = t.bary2cart(0.0, 0.5, 0.5);
				Vec3 e2_mid = t.bary2cart(0.5, 0.0, 0.5);
				Vec3 e3_mid = t.bary2cart(0.5, 0.5, 0.0);

				CHECK_THAT(e1_mid.x, WithinRel( 0.50));
				CHECK_THAT(e1_mid.y, WithinRel( 0.00));
				CHECK_THAT(e1_mid.z, WithinRel( 0.25));

				CHECK_THAT(e2_mid.x, WithinRel( 0.00));
				CHECK_THAT(e2_mid.y, WithinRel( 0.00));
				CHECK_THAT(e2_mid.z, WithinRel(-0.50));

				CHECK_THAT(e3_mid.x, WithinRel(-0.50));
				CHECK_THAT(e3_mid.y, WithinRel( 0.00));
				CHECK_THAT(e3_mid.z, WithinRel( 0.25));
			}
			SECTION("center") {
				constexpr f64 one_third = 1.0 / 3.0;
				Vec3 c = t.bary2cart(one_third, one_third, one_third);

				CHECK_THAT(c.x, WithinRel(0.0));
				CHECK_THAT(c.y, WithinRel(0.0));
				CHECK_THAT(c.z, WithinRel(0.0));
			}
		}
		SECTION("centroid") {
			Vec3 c = t.centroid();

			CHECK_THAT(c.x, WithinRel(0.0));
			CHECK_THAT(c.y, WithinRel(0.0));
			CHECK_THAT(c.z, WithinRel(0.0));
		}
		SECTION("incenter / inscribed circle") {
			auto t_eq = Tri{
				{ 1, 0, 0 },
				{ 0, 1, 0 },
				{ 0, 0, 1 },
			};
			Vec3 incenter = t_eq.incenter();
			Circle incirc = t_eq.inscribed_circle();

			SECTION("consistent center point") {
				CHECK_THAT(incenter.x, WithinRel(incirc.center.x));
				CHECK_THAT(incenter.y, WithinRel(incirc.center.y));
				CHECK_THAT(incenter.z, WithinRel(incirc.center.z));
			}

			Vec3 e1_mid = t_eq.bary2cart(0.0, 0.5, 0.5);
			Vec3 e2_mid = t_eq.bary2cart(0.5, 0.0, 0.5);
			Vec3 e3_mid = t_eq.bary2cart(0.5, 0.5, 0.0);

			SECTION("tangent with all triangle edges") {
				CHECK_THAT(Vec3::dist(incenter, e1_mid), WithinRel(incirc.radius));
				CHECK_THAT(Vec3::dist(incenter, e2_mid), WithinRel(incirc.radius));
				CHECK_THAT(Vec3::dist(incenter, e3_mid), WithinRel(incirc.radius));
			}
			SECTION("gives the same normal as `Plane::from_points`") {
				auto p = Plane::from_points(t_eq.v1, t_eq.v2, t_eq.v3);
				CHECK_THAT(incirc.normal.x, WithinRel(p.normal.x));
				CHECK_THAT(incirc.normal.y, WithinRel(p.normal.y));
				CHECK_THAT(incirc.normal.z, WithinRel(p.normal.z));
			}
		}
		SECTION("circumcenter / circumscribed circle") {
			Vec3 circumcenter = t.circumcenter();
			Circle circumcirc = t.circumscribed_circle();

			SECTION("consistent center point") {
				CHECK_THAT(circumcenter.x, WithinRel(circumcirc.center.x));
				CHECK_THAT(circumcenter.y, WithinRel(circumcirc.center.y));
				CHECK_THAT(circumcenter.z, WithinRel(circumcirc.center.z));
			}
			SECTION("coincident with all triangle vertices") {
				CHECK_THAT(Vec3::dist(circumcenter, t.v1), WithinRel(circumcirc.radius));
				CHECK_THAT(Vec3::dist(circumcenter, t.v2), WithinRel(circumcirc.radius));
				CHECK_THAT(Vec3::dist(circumcenter, t.v3), WithinRel(circumcirc.radius));
			}
			SECTION("gives the same normal as `Plane::from_points`") {
				auto p = Plane::from_points(t.v1, t.v2, t.v3);

				CHECK_THAT(circumcirc.normal.x, WithinRel(p.normal.x));
				CHECK_THAT(circumcirc.normal.y, WithinRel(p.normal.y));
				CHECK_THAT(circumcirc.normal.z, WithinRel(p.normal.z));
			}
		}
	}
}
