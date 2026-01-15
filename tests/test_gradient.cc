#include <gradual/gradient.h>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using Catch::Approx;

// Test functions
template <typename T>
Dual<T> f_2d_quadratic(const Dual<T> &x, const Dual<T> &y) {
  // f(x, y) = x^2 + y^2
  // ∇f = (2x, 2y)
  return x * x + y * y;
}

template <typename T>
Dual<T> f_3d_quadratic(const Dual<T> &x, const Dual<T> &y, const Dual<T> &z) {
  // f(x, y, z) = x^2 + y^2 + z^2
  // ∇f = (2x, 2y, 2z)
  return x * x + y * y + z * z;
}

template <typename T>
Dual<T> f_2d_mixed(const Dual<T> &x, const Dual<T> &y) {
  // f(x, y) = x*y + x^2
  // ∂f/∂x = y + 2x, ∂f/∂y = x
  return x * y + x * x;
}

template <typename T>
Dual<T> f_3d_polynomial(const Dual<T> &x, const Dual<T> &y, const Dual<T> &z) {
  // f(x, y, z) = x^2 + 2*y^2 + 3*z^2 + x*y
  // ∂f/∂x = 2x + y, ∂f/∂y = 4y + x, ∂f/∂z = 6z
  return x * x + y * y * 2.0 + z * z * 3.0 + x * y;
}

TEST_CASE("Gradient of 2D quadratic function", "[gradient]") {
  SECTION("At point (0, 0)") {
    Vector point(0.0, 0.0);
    auto grad = gradient(f_2d_quadratic<double>, point);

    REQUIRE(grad[0] == Approx(0.0)); // 2*0 = 0
    REQUIRE(grad[1] == Approx(0.0)); // 2*0 = 0
  }

  SECTION("At point (1, 0)") {
    Vector point(1.0, 0.0);
    auto grad = gradient(f_2d_quadratic<double>, point);

    REQUIRE(grad[0] == Approx(2.0)); // 2*1 = 2
    REQUIRE(grad[1] == Approx(0.0)); // 2*0 = 0
  }

  SECTION("At point (3, 4)") {
    Vector point(3.0, 4.0);
    auto grad = gradient(f_2d_quadratic<double>, point);

    REQUIRE(grad[0] == Approx(6.0)); // 2*3 = 6
    REQUIRE(grad[1] == Approx(8.0)); // 2*4 = 8
  }

  SECTION("At point (-2, 5)") {
    Vector point(-2.0, 5.0);
    auto grad = gradient(f_2d_quadratic<double>, point);

    REQUIRE(grad[0] == Approx(-4.0)); // 2*(-2) = -4
    REQUIRE(grad[1] == Approx(10.0)); // 2*5 = 10
  }
}

TEST_CASE("Gradient of 3D quadratic function", "[gradient]") {
  SECTION("At point (0, 0, 0)") {
    Vector point(0.0, 0.0, 0.0);
    auto grad = gradient(f_3d_quadratic<double>, point);

    REQUIRE(grad[0] == Approx(0.0));
    REQUIRE(grad[1] == Approx(0.0));
    REQUIRE(grad[2] == Approx(0.0));
  }

  SECTION("At point (1, 2, 3)") {
    Vector point(1.0, 2.0, 3.0);
    auto grad = gradient(f_3d_quadratic<double>, point);

    REQUIRE(grad[0] == Approx(2.0)); // 2*1 = 2
    REQUIRE(grad[1] == Approx(4.0)); // 2*2 = 4
    REQUIRE(grad[2] == Approx(6.0)); // 2*3 = 6
  }

  SECTION("At point (5, -3, 2)") {
    Vector point(5.0, -3.0, 2.0);
    auto grad = gradient(f_3d_quadratic<double>, point);

    REQUIRE(grad[0] == Approx(10.0)); // 2*5 = 10
    REQUIRE(grad[1] == Approx(-6.0)); // 2*(-3) = -6
    REQUIRE(grad[2] == Approx(4.0));  // 2*2 = 4
  }
}

TEST_CASE("Gradient of 2D mixed function", "[gradient]") {
  // f(x, y) = x*y + x^2
  // ∂f/∂x = y + 2x, ∂f/∂y = x

  SECTION("At point (2, 3)") {
    Vector point(2.0, 3.0);
    auto grad = gradient(f_2d_mixed<double>, point);

    REQUIRE(grad[0] == Approx(7.0)); // 3 + 2*2 = 7
    REQUIRE(grad[1] == Approx(2.0)); // 2
  }

  SECTION("At point (0, 5)") {
    Vector point(0.0, 5.0);
    auto grad = gradient(f_2d_mixed<double>, point);

    REQUIRE(grad[0] == Approx(5.0)); // 5 + 2*0 = 5
    REQUIRE(grad[1] == Approx(0.0)); // 0
  }

  SECTION("At point (-1, 4)") {
    Vector point(-1.0, 4.0);
    auto grad = gradient(f_2d_mixed<double>, point);

    REQUIRE(grad[0] == Approx(2.0));  // 4 + 2*(-1) = 2
    REQUIRE(grad[1] == Approx(-1.0)); // -1
  }
}

TEST_CASE("Gradient of 3D polynomial function", "[gradient]") {
  // f(x, y, z) = x^2 + 2*y^2 + 3*z^2 + x*y
  // ∂f/∂x = 2x + y, ∂f/∂y = 4y + x, ∂f/∂z = 6z

  SECTION("At point (1, 1, 1)") {
    Vector point(1.0, 1.0, 1.0);
    auto grad = gradient(f_3d_polynomial<double>, point);

    REQUIRE(grad[0] == Approx(3.0)); // 2*1 + 1 = 3
    REQUIRE(grad[1] == Approx(5.0)); // 4*1 + 1 = 5
    REQUIRE(grad[2] == Approx(6.0)); // 6*1 = 6
  }

  SECTION("At point (2, -1, 3)") {
    Vector point(2.0, -1.0, 3.0);
    auto grad = gradient(f_3d_polynomial<double>, point);

    REQUIRE(grad[0] == Approx(3.0));  // 2*2 + (-1) = 3
    REQUIRE(grad[1] == Approx(-2.0)); // 4*(-1) + 2 = -2
    REQUIRE(grad[2] == Approx(18.0)); // 6*3 = 18
  }
}

TEST_CASE("Gradient with different types", "[gradient]") {
  SECTION("Float type") {
    Vector point(2.0f, 3.0f);
    auto grad = gradient(f_2d_quadratic<float>, point);

    REQUIRE(grad[0] == Approx(4.0f)); // 2*2 = 4
    REQUIRE(grad[1] == Approx(6.0f)); // 2*3 = 6
  }

  SECTION("Double type") {
    Vector point(2.0, 3.0);
    auto grad = gradient(f_2d_quadratic<double>, point);

    REQUIRE(grad[0] == Approx(4.0)); // 2*2 = 4
    REQUIRE(grad[1] == Approx(6.0)); // 2*3 = 6
  }
}

TEST_CASE("Gradient with elementary functions", "[gradient]") {
  // f(x, y) = exp(x) + sin(y)
  // ∂f/∂x = exp(x), ∂f/∂y = cos(y)
  auto f = [](const Dual<double> &x, const Dual<double> &y) {
    return exp(x) + sin(y);
  };

  SECTION("At point (0, 0)") {
    Vector point(0.0, 0.0);
    auto grad = gradient(f, point);

    REQUIRE(grad[0] == Approx(1.0)); // exp(0) = 1
    REQUIRE(grad[1] == Approx(1.0)); // cos(0) = 1
  }

  SECTION("At point (1, π/2)") {
    constexpr double pi = 3.14159265358979323846;
    Vector point(1.0, pi / 2.0);
    auto grad = gradient(f, point);

    REQUIRE(grad[0] == Approx(std::exp(1.0)));     // exp(1)
    REQUIRE(grad[1] == Approx(0.0).margin(1e-10)); // cos(π/2) = 0
  }
}

TEST_CASE("Gradient of 4D function", "[gradient]") {
  // f(w, x, y, z) = w^2 + x^2 + y^2 + z^2
  // ∇f = (2w, 2x, 2y, 2z)
  auto f = [](const Dual<double> &w,
              const Dual<double> &x,
              const Dual<double> &y,
              const Dual<double> &z) {
    return w * w + x * x + y * y + z * z;
  };

  Vector point(1.0, 2.0, 3.0, 4.0);
  auto grad = gradient(f, point);

  REQUIRE(grad[0] == Approx(2.0)); // 2*1 = 2
  REQUIRE(grad[1] == Approx(4.0)); // 2*2 = 4
  REQUIRE(grad[2] == Approx(6.0)); // 2*3 = 6
  REQUIRE(grad[3] == Approx(8.0)); // 2*4 = 8
}

TEST_CASE("Gradient of 1D function", "[gradient]") {
  // f(x) = x^2
  // df/dx = 2x
  auto f = [](const Dual<double> &x) {
    return x * x;
  };

  SECTION("At point (5)") {
    Vector point(5.0);
    auto grad = gradient(f, point);

    REQUIRE(grad[0] == Approx(10.0)); // 2*5 = 10
  }

  SECTION("At point (-3)") {
    Vector point(-3.0);
    auto grad = gradient(f, point);

    REQUIRE(grad[0] == Approx(-6.0)); // 2*(-3) = -6
  }
}
