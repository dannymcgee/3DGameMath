#include <cstdlib>
#include <limits>

#include <catch2/catch_all.hpp>
#include <fmt/format.h>

#include <math/euler.h>
#include <math/literals.h>
#include <math/matrix.h>
#include <math/matrix/rotation.h>
#include <math/quat.h>
#include <math/spaces.h>
#include <math/utility.h>
#include <math/vector.h>
#include <sized.h>

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

				auto in_place = Vec3::Zero;
				in_place.normalize();
				REQUIRE(!std::isnan(in_place.x));
				REQUIRE(!std::isnan(in_place.y));
				REQUIRE(!std::isnan(in_place.z));
			}
		}
		SECTION("can calculate the length and direction as a single operation") {
			auto vec = Vec3{ 0.f, 10.f, 0.f };
			auto [length, direction] = vec.length_and_direction();

			REQUIRE(math::nearly_equal<f32>(length, 10.f));
			REQUIRE(math::nearly_equal<f32>(direction.x, 0.f));
			REQUIRE(math::nearly_equal<f32>(direction.y, 1.f));
			REQUIRE(math::nearly_equal<f32>(direction.z, 0.f));
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

			REQUIRE(math::nearly_equal(ab.m11, 21.0));
			REQUIRE(math::nearly_equal(ab.m12, -6.0));
			REQUIRE(math::nearly_equal(ab.m21, -33.0));
			REQUIRE(math::nearly_equal(ab.m22, 13.0));
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

			REQUIRE(math::nearly_equal(a.determinant(), 5.0));
			REQUIRE(math::nearly_equal(b.determinant(), -23.0));
		}
		SECTION("can compute the inverse") {
			auto mat = Mat2x2{
				{ -3.0, 4.0 },
				{  2.0, 5.0 },
			};
			auto inverted = mat.inverse();

			REQUIRE(inverted);

			auto id = mat * (*inverted);

			REQUIRE(math::nearly_equal(id.m11, 1.0));
			REQUIRE(math::nearly_equal(id.m12, 0.0));

			REQUIRE(math::nearly_equal(id.m21, 0.0));
			REQUIRE(math::nearly_equal(id.m22, 1.0));
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

			REQUIRE(math::nearly_equal(mat.determinant(), -24.0));
		}
		SECTION("can compute the inverse") {
			auto mat = Mat3x3{
				{ -4.0, -3.0,  3.0 },
				{  0.0,  2.0, -2.0 },
				{  1.0,  4.0, -1.0 },
			};
			auto inverted = mat.inverse();

			REQUIRE(inverted);

			auto id = mat * (*inverted);

			REQUIRE(math::nearly_equal(id.m11, 1.0));
			REQUIRE(math::nearly_equal(id.m12, 0.0));
			REQUIRE(math::nearly_equal(id.m13, 0.0));

			REQUIRE(math::nearly_equal(id.m21, 0.0));
			REQUIRE(math::nearly_equal(id.m22, 1.0));
			REQUIRE(math::nearly_equal(id.m23, 0.0));

			REQUIRE(math::nearly_equal(id.m31, 0.0));
			REQUIRE(math::nearly_equal(id.m32, 0.0));
			REQUIRE(math::nearly_equal(id.m33, 1.0));
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

			REQUIRE(math::nearly_equal(mat.determinant(), -24.0));
		}
		SECTION("can compute the inverse") {
			auto mat = Mat4x4{
				{ -4.0, -3.0,  3.0,  1.0 },
				{  0.0,  2.0, -2.0,  0.0 },
				{  1.0,  4.0, -1.0,  1.0 },
				{  0.0,  2.0, -2.0,  1.0 },
			};
			auto inverted = mat.inverse();

			REQUIRE(inverted);

			auto id = mat * (*inverted);

			REQUIRE(math::nearly_equal(id.m11, 1.0));
			REQUIRE(math::nearly_equal(id.m12, 0.0));
			REQUIRE(math::nearly_equal(id.m13, 0.0));
			REQUIRE(math::nearly_equal(id.m14, 0.0));

			REQUIRE(math::nearly_equal(id.m21, 0.0));
			REQUIRE(math::nearly_equal(id.m22, 1.0));
			REQUIRE(math::nearly_equal(id.m23, 0.0));
			REQUIRE(math::nearly_equal(id.m24, 0.0));

			REQUIRE(math::nearly_equal(id.m31, 0.0));
			REQUIRE(math::nearly_equal(id.m32, 0.0));
			REQUIRE(math::nearly_equal(id.m33, 1.0));
			REQUIRE(math::nearly_equal(id.m34, 0.0));

			REQUIRE(math::nearly_equal(id.m41, 0.0));
			REQUIRE(math::nearly_equal(id.m42, 0.0));
			REQUIRE(math::nearly_equal(id.m43, 0.0));
			REQUIRE(math::nearly_equal(id.m44, 1.0));
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
			REQUIRE(math::nearly_equal(mat4x3_labeled.m11, 11.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m12, 12.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m13, 13.0));

			REQUIRE(math::nearly_equal(mat4x3_labeled.m21, 21.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m22, 22.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m23, 23.0));

			REQUIRE(math::nearly_equal(mat4x3_labeled.m31, 31.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m32, 32.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m33, 33.0));

			REQUIRE(math::nearly_equal(mat4x3_labeled.m41, 41.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m42, 42.0));
			REQUIRE(math::nearly_equal(mat4x3_labeled.m43, 43.0));
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

			REQUIRE(math::nearly_equal(mat3x4.m11, 11.0));
			REQUIRE(math::nearly_equal(mat3x4.m12, 21.0));
			REQUIRE(math::nearly_equal(mat3x4.m13, 31.0));
			REQUIRE(math::nearly_equal(mat3x4.m14, 41.0));

			REQUIRE(math::nearly_equal(mat3x4.m21, 12.0));
			REQUIRE(math::nearly_equal(mat3x4.m22, 22.0));
			REQUIRE(math::nearly_equal(mat3x4.m23, 32.0));
			REQUIRE(math::nearly_equal(mat3x4.m24, 42.0));

			REQUIRE(math::nearly_equal(mat3x4.m31, 13.0));
			REQUIRE(math::nearly_equal(mat3x4.m32, 23.0));
			REQUIRE(math::nearly_equal(mat3x4.m33, 33.0));
			REQUIRE(math::nearly_equal(mat3x4.m34, 43.0));
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

				REQUIRE(math::nearly_equal(result.m11, 2.0));
				REQUIRE(math::nearly_equal(result.m12, 4.0));
				REQUIRE(math::nearly_equal(result.m13, 6.0));

				REQUIRE(math::nearly_equal(result.m21, 8.0));
				REQUIRE(math::nearly_equal(result.m22, 10.0));
				REQUIRE(math::nearly_equal(result.m23, 12.0));

				REQUIRE(math::nearly_equal(result.m31, 14.0));
				REQUIRE(math::nearly_equal(result.m32, 16.0));
				REQUIRE(math::nearly_equal(result.m33, 18.0));

				REQUIRE(math::nearly_equal(result.m41, 20.0));
				REQUIRE(math::nearly_equal(result.m42, 22.0));
				REQUIRE(math::nearly_equal(result.m43, 24.0));
			}
			SECTION("...f64 * Mat") {
				auto result = 2.0 * mat;

				REQUIRE(math::nearly_equal(result.m11, 2.0));
				REQUIRE(math::nearly_equal(result.m12, 4.0));
				REQUIRE(math::nearly_equal(result.m13, 6.0));

				REQUIRE(math::nearly_equal(result.m21, 8.0));
				REQUIRE(math::nearly_equal(result.m22, 10.0));
				REQUIRE(math::nearly_equal(result.m23, 12.0));

				REQUIRE(math::nearly_equal(result.m31, 14.0));
				REQUIRE(math::nearly_equal(result.m32, 16.0));
				REQUIRE(math::nearly_equal(result.m33, 18.0));

				REQUIRE(math::nearly_equal(result.m41, 20.0));
				REQUIRE(math::nearly_equal(result.m42, 22.0));
				REQUIRE(math::nearly_equal(result.m43, 24.0));
			}
			SECTION("...Mat *= f64") {
				auto result = mat;
				result *= 2.0;

				REQUIRE(math::nearly_equal(result.m11, 2.0));
				REQUIRE(math::nearly_equal(result.m12, 4.0));
				REQUIRE(math::nearly_equal(result.m13, 6.0));

				REQUIRE(math::nearly_equal(result.m21, 8.0));
				REQUIRE(math::nearly_equal(result.m22, 10.0));
				REQUIRE(math::nearly_equal(result.m23, 12.0));

				REQUIRE(math::nearly_equal(result.m31, 14.0));
				REQUIRE(math::nearly_equal(result.m32, 16.0));
				REQUIRE(math::nearly_equal(result.m33, 18.0));

				REQUIRE(math::nearly_equal(result.m41, 20.0));
				REQUIRE(math::nearly_equal(result.m42, 22.0));
				REQUIRE(math::nearly_equal(result.m43, 24.0));
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

				REQUIRE(math::nearly_equal(result.x, expected.x));
				REQUIRE(math::nearly_equal(result.y, expected.y));
				REQUIRE(math::nearly_equal(result.z, expected.z));
			}
			SECTION("Mat4x3 * [Mat3x1] Vec3") {
				auto result = mat4x3_labeled * math::Vector<3>{ 1.0, 2.0, 3.0 };
				auto expected = math::Vector<4>{
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
			REQUIRE(math::nearly_equal<f64>(euler.yaw, 45_deg));
			REQUIRE(math::nearly_equal<f64>(euler.pitch, -15_deg));
			REQUIRE(math::nearly_equal<f64>(euler.roll, 3.3_deg));
		}
		SECTION("Matrix conversion") {
			SECTION("Local -> Parent") {
				auto result = euler.matrix(Space::Local2Parent);

				auto yaw_mat   = RotationMatrix(45_deg, Axis::Up);
				auto pitch_mat = RotationMatrix(-15_deg, Axis::Right);
				auto roll_mat  = RotationMatrix(3.3_deg, Axis::Forward);
				auto expected = roll_mat * pitch_mat * yaw_mat;

				REQUIRE(math::nearly_equal(result.m11, expected.m11, epsilon));
				REQUIRE(math::nearly_equal(result.m12, expected.m12, epsilon));
				REQUIRE(math::nearly_equal(result.m13, expected.m13, epsilon));

				REQUIRE(math::nearly_equal(result.m21, expected.m21, epsilon));
				REQUIRE(math::nearly_equal(result.m22, expected.m22, epsilon));
				REQUIRE(math::nearly_equal(result.m23, expected.m23, epsilon));

				REQUIRE(math::nearly_equal(result.m31, expected.m31, epsilon));
				REQUIRE(math::nearly_equal(result.m32, expected.m32, epsilon));
				REQUIRE(math::nearly_equal(result.m33, expected.m33, epsilon));
			}
			SECTION("Parent -> Local") {
				auto result = euler.matrix(Space::Parent2Local);

				auto yaw_mat   = RotationMatrix(45_deg, Vec3::up());
				auto pitch_mat = RotationMatrix(-15_deg, Vec3::right());
				auto roll_mat  = RotationMatrix(3.3_deg, Vec3::forward());
				auto expected = (roll_mat * pitch_mat * yaw_mat).inverse();

				REQUIRE(math::nearly_equal(result.m11, expected.m11, epsilon));
				REQUIRE(math::nearly_equal(result.m12, expected.m12, epsilon));
				REQUIRE(math::nearly_equal(result.m13, expected.m13, epsilon));

				REQUIRE(math::nearly_equal(result.m21, expected.m21, epsilon));
				REQUIRE(math::nearly_equal(result.m22, expected.m22, epsilon));
				REQUIRE(math::nearly_equal(result.m23, expected.m23, epsilon));

				REQUIRE(math::nearly_equal(result.m31, expected.m31, epsilon));
				REQUIRE(math::nearly_equal(result.m32, expected.m32, epsilon));
				REQUIRE(math::nearly_equal(result.m33, expected.m33, epsilon));
			}
		}
		SECTION("Quaternion conversion") {
			SECTION("Local -> Parent") {
				auto result = euler.quat(Space::Local2Parent);

				auto yaw_quat   = Quat::angle_axis(45_deg, Vec3::up());
				auto pitch_quat = Quat::angle_axis(-15_deg, Vec3::right());
				auto roll_quat  = Quat::angle_axis(3.3_deg, Vec3::forward());
				auto expected = yaw_quat * pitch_quat * roll_quat;

				REQUIRE(math::nearly_equal(result.w, expected.w, epsilon));
				REQUIRE(math::nearly_equal(result.x, expected.x, epsilon));
				REQUIRE(math::nearly_equal(result.y, expected.y, epsilon));
				REQUIRE(math::nearly_equal(result.z, expected.z, epsilon));
			}
			SECTION("Parent -> Local") {
				auto result = euler.quat(Space::Parent2Local);

				auto yaw_quat   = Quat::angle_axis(45_deg, Vec3::up());
				auto pitch_quat = Quat::angle_axis(-15_deg, Vec3::right());
				auto roll_quat  = Quat::angle_axis(3.3_deg, Vec3::forward());
				auto expected = (yaw_quat * pitch_quat * roll_quat).inverse();

				REQUIRE(math::nearly_equal(result.w, expected.w, epsilon));
				REQUIRE(math::nearly_equal(result.x, expected.x, epsilon));
				REQUIRE(math::nearly_equal(result.y, expected.y, epsilon));
				REQUIRE(math::nearly_equal(result.z, expected.z, epsilon));
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
			REQUIRE(math::nearly_equal(yaw_angle, 45_deg, epsilon));
			REQUIRE(math::nearly_equal(yaw_axis.x, 0.0, epsilon));
			REQUIRE(math::nearly_equal(yaw_axis.y, 1.0, epsilon));
			REQUIRE(math::nearly_equal(yaw_axis.z, 0.0, epsilon));

			auto [pitch_angle, pitch_axis] = pitch_neg15.angle_axis();
			REQUIRE(math::nearly_equal(pitch_angle, 15_deg, epsilon));
			REQUIRE(math::nearly_equal(pitch_axis.x, -1.0, epsilon));
			REQUIRE(math::nearly_equal(pitch_axis.y, 0.0, epsilon));
			REQUIRE(math::nearly_equal(pitch_axis.z, 0.0, epsilon));

			auto [roll_angle, roll_axis] = roll_3_3.angle_axis();
			REQUIRE(math::nearly_equal(roll_angle, 3.3_deg, epsilon));
			REQUIRE(math::nearly_equal(roll_axis.x, 0.0, epsilon));
			REQUIRE(math::nearly_equal(roll_axis.y, 0.0, epsilon));
			REQUIRE(math::nearly_equal(roll_axis.z, 1.0, epsilon));
		}
		SECTION("Euler conversion") {
			SECTION("Local -> Parent") {
				auto euler = quat.euler(Space::Local2Parent);

				REQUIRE(math::nearly_equal(euler.yaw, 45_deg, epsilon));
				REQUIRE(math::nearly_equal(euler.pitch, -15_deg, epsilon));
				REQUIRE(math::nearly_equal(euler.roll, 3.3_deg, epsilon));
			}
			SECTION("Parent -> Local") {
				auto euler = quat.euler(Space::Parent2Local);

				// TODO: _Are_ these the expected values? I think so, but this is
				// just a snapshot of the current behavior, not externally verified
				REQUIRE(math::nearly_equal(euler.yaw, -46.369_deg, 0.001));
				REQUIRE(math::nearly_equal(euler.pitch, 8.164_deg, 0.001));
				REQUIRE(math::nearly_equal(euler.roll, -13.044_deg, 0.001));
			}
		}
		SECTION("Matrix conversion") {
			SECTION("Local -> Parent") {
				auto result = quat.matrix(Space::Local2Parent);

				auto yaw_mat   = RotationMatrix(45_deg, Axis::Up);
				auto pitch_mat = RotationMatrix(-15_deg, Axis::Right);
				auto roll_mat  = RotationMatrix(3.3_deg, Axis::Forward);
				auto expected = roll_mat * pitch_mat * yaw_mat;

				REQUIRE(math::nearly_equal(result.m11, expected.m11, epsilon));
				REQUIRE(math::nearly_equal(result.m12, expected.m12, epsilon));
				REQUIRE(math::nearly_equal(result.m13, expected.m13, epsilon));

				REQUIRE(math::nearly_equal(result.m21, expected.m21, epsilon));
				REQUIRE(math::nearly_equal(result.m22, expected.m22, epsilon));
				REQUIRE(math::nearly_equal(result.m23, expected.m23, epsilon));

				REQUIRE(math::nearly_equal(result.m31, expected.m31, epsilon));
				REQUIRE(math::nearly_equal(result.m32, expected.m32, epsilon));
				REQUIRE(math::nearly_equal(result.m33, expected.m33, epsilon));
			}
			SECTION("Parent -> Local") {
				auto result = quat.matrix(Space::Parent2Local);

				auto yaw_mat   = RotationMatrix(45_deg, Vec3::up());
				auto pitch_mat = RotationMatrix(-15_deg, Vec3::right());
				auto roll_mat  = RotationMatrix(3.3_deg, Vec3::forward());
				auto expected = (roll_mat * pitch_mat * yaw_mat).inverse();

				REQUIRE(math::nearly_equal(result.m11, expected.m11, epsilon));
				REQUIRE(math::nearly_equal(result.m12, expected.m12, epsilon));
				REQUIRE(math::nearly_equal(result.m13, expected.m13, epsilon));

				REQUIRE(math::nearly_equal(result.m21, expected.m21, epsilon));
				REQUIRE(math::nearly_equal(result.m22, expected.m22, epsilon));
				REQUIRE(math::nearly_equal(result.m23, expected.m23, epsilon));

				REQUIRE(math::nearly_equal(result.m31, expected.m31, epsilon));
				REQUIRE(math::nearly_equal(result.m32, expected.m32, epsilon));
				REQUIRE(math::nearly_equal(result.m33, expected.m33, epsilon));
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

			REQUIRE(math::nearly_equal(result.w, expected.w, epsilon));
			REQUIRE(math::nearly_equal(result.x, expected.x, epsilon));
			REQUIRE(math::nearly_equal(result.y, expected.y, epsilon));
			REQUIRE(math::nearly_equal(result.z, expected.z, epsilon));
		}
		SECTION("Euler angles conversion") {
			auto result = matrix.euler();
			auto expected = Euler{ 45_deg, -15_deg, 3.3_deg };

			REQUIRE(math::nearly_equal(result.yaw, expected.yaw, epsilon));
			REQUIRE(math::nearly_equal(result.pitch, expected.pitch, epsilon));
			REQUIRE(math::nearly_equal(result.roll, expected.roll, epsilon));
		}
	}
}
