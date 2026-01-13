#include "vector.h"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using Catch::Approx;

TEST_CASE("Vector construction", "[vector]") {
  SECTION("Default constructor") {
    Vector v1(0.0);
    REQUIRE(v1.size() == 1);
    REQUIRE(v1[0] == Approx(0.0));

    Vector<float, 5> v3{};
    REQUIRE(v3.size() == 5);
    REQUIRE(v3[0] == Approx(0.0f));
    REQUIRE(v3[1] == Approx(0.0f));
    REQUIRE(v3[2] == Approx(0.0f));
    REQUIRE(v3[3] == Approx(0.0f));
    REQUIRE(v3[4] == Approx(0.0f));
  }

  SECTION("Variadic constructor") {
    Vector v(1.0, 2.0, 3.0);
    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1.0);
    REQUIRE(v[1] == 2.0);
    REQUIRE(v[2] == 3.0);
  }

  SECTION("Vector of different types") {
    Vector vf(1.5f, 2.5f);
    REQUIRE(vf.size() == 2);
    REQUIRE(vf[0] == Approx(1.5f));
  }
}

TEST_CASE("Vector element access", "[vector]") {
  Vector v(5.0, 10.0, 15.0);

  SECTION("Read access") {
    REQUIRE(v[0] == 5.0);
    REQUIRE(v[1] == 10.0);
    REQUIRE(v[2] == 15.0);
  }

  SECTION("Write access") {
    v[0] = 100.0;
    v[1] = 200.0;
    REQUIRE(v[0] == 100.0);
    REQUIRE(v[1] == 200.0);
    REQUIRE(v[2] == 15.0); // unchanged
  }

  SECTION("Size query") {
    REQUIRE(v.size() == 3);
  }
}

TEST_CASE("Vector addition", "[vector]") {
  Vector a(1.0, 2.0, 3.0);
  Vector b(4.0, 5.0, 6.0);

  auto result = a + b;

  REQUIRE(result.size() == 3);
  REQUIRE(result[0] == 5.0);
  REQUIRE(result[1] == 7.0);
  REQUIRE(result[2] == 9.0);
}

TEST_CASE("Vector subtraction", "[vector]") {
  Vector a(10.0, 20.0, 30.0);
  Vector b(1.0, 2.0, 3.0);

  auto result = a - b;

  REQUIRE(result.size() == 3);
  REQUIRE(result[0] == 9.0);
  REQUIRE(result[1] == 18.0);
  REQUIRE(result[2] == 27.0);
}

TEST_CASE("Vector dot product", "[vector]") {
  Vector a(1.0, 2.0, 3.0);
  Vector b(4.0, 5.0, 6.0);

  auto result = a * b; // 4 + 10 + 18 = 32
  REQUIRE(result == Approx(32.0));

  Vector u(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0);
  Vector v(8.0, -7.0, 6.0, -5.0, 4.0, -3.0, 2.0, -1.0);
  result = u * v;
  REQUIRE(result == Approx(0.0));
}

TEST_CASE("Vector scalar multiplication", "[vector]") {
  Vector v(2.0, 3.0, 4.0);

  SECTION("Vector * Scalar") {
    auto result = v * 2.0;
    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == 4.0);
    REQUIRE(result[1] == 6.0);
    REQUIRE(result[2] == 8.0);
  }

  SECTION("Scalar * Vector") {
    auto result = 3.0 * v;
    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == 6.0);
    REQUIRE(result[1] == 9.0);
    REQUIRE(result[2] == 12.0);
  }

  SECTION("Multiplication by zero") {
    auto result = v * 0.0;
    REQUIRE(result[0] == 0.0);
    REQUIRE(result[1] == 0.0);
    REQUIRE(result[2] == 0.0);
  }

  SECTION("Multiplication by negative") {
    auto result = v * -1.0;
    REQUIRE(result[0] == -2.0);
    REQUIRE(result[1] == -3.0);
    REQUIRE(result[2] == -4.0);
  }
}

TEST_CASE("Vector scalar division", "[vector]") {
  Vector v(4.0, 6.0, 8.0);

  SECTION("Vector / Scalar") {
    auto result = v / 2.0;
    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == 2.0);
    REQUIRE(result[1] == 3.0);
    REQUIRE(result[2] == 4.0);
  }
}

TEST_CASE("Vector operations chain", "[vector]") {
  // Test: (a + b) * 2 - c
  Vector a(1.0, 2.0, 3.0);
  Vector b(4.0, 5.0, 6.0);
  Vector c(2.0, 2.0, 2.0);

  auto result = (a + b) * 2.0 - c;

  REQUIRE(result.size() == 3);
  REQUIRE(result[0] == 8.0);  // (1+4)*2 - 2 = 10 - 2 = 8
  REQUIRE(result[1] == 12.0); // (2+5)*2 - 2 = 14 - 2 = 12
  REQUIRE(result[2] == 16.0); // (3+6)*2 - 2 = 18 - 2 = 16
}

TEST_CASE("Vector magnitude", "[vector]") {
  Vector a(1.0, 2.0, 3.0); // norm2 = 1 + 4 + 9 = 14

  REQUIRE(a.norm2() == Approx(14.0));
  REQUIRE(a.norm() == Approx(std::sqrt(14.0)));

  Vector b(-1.0, -2.0, -3.0); // same norm

  REQUIRE(b.norm2() == Approx(14.0));
  REQUIRE(b.norm() == Approx(std::sqrt(14.0)));

  Vector c(1.0, 2.0, 3.0, 4.0); // norm2 = 1 + 4 + 9 + 16 = 30

  REQUIRE(c.norm2() == Approx(30.0));
  REQUIRE(c.norm() == Approx(std::sqrt(30.0)));
}
