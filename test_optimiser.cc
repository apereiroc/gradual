#include "dual.h"
#include "gradient.h"
#include "optimiser.h"
#include "vector.h"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using Catch::Approx;

TEST_CASE("Optimiser: 1D unbounded quadratic", "[optimiser]") {
  // f(x) = x^2, minimum at x = 0
  auto f = [](const Dual<double> &x) { return x * x; };

  Vector<double, 1> start{5.0};
  Optimiser<double> opt(0.1, 1e-6, 1000);

  auto result = opt.run(f, start);

  REQUIRE(result.converged());
  REQUIRE(result.point()[0] == Approx(0.0).margin(1e-4));
  REQUIRE(result.value() == Approx(0.0).margin(1e-6));
  REQUIRE(result.grad() <= 1e-6); // tolerance passed to optimiser
}

TEST_CASE("Optimiser: 2D unbounded quadratic bowl", "[optimiser]") {
  // f(x,y) = (x-1)^2 + (y-2)^2, minimum at (1, 2)
  auto f = [](const Dual<double> &x, const Dual<double> &y) {
    return (x - 1.0) * (x - 1.0) + (y - 2.0) * (y - 2.0);
  };

  Vector<double, 2> start{10.0, 10.0};
  Optimiser<double> opt(0.1, 1e-6, 1000);

  auto result = opt.run(f, start);

  REQUIRE(result.converged());
  REQUIRE(result.point()[0] == Approx(1.0).margin(1e-4));
  REQUIRE(result.point()[1] == Approx(2.0).margin(1e-4));
  REQUIRE(result.value() == Approx(0.0).margin(1e-6));
  REQUIRE(result.grad() <= 1e-6);
}

TEST_CASE("Optimiser: 3D unbounded quadratic (MVP example)", "[optimiser]") {
  // f(x,y,z) = (x-1)^2 + (y-2)^2 + (z-3)^2, minimum at (1, 2, 3)
  auto f = [](const Dual<double> &x, const Dual<double> &y,
              const Dual<double> &z) {
    return pow(x - 1, 2.0) + pow(y - 2, 2.0) + pow(z - 3, 2.0);
  };

  Vector<double, 3> start{10.0, 10.0, 10.0};
  Optimiser<double> opt(0.1, 1e-6, 1000);

  auto result = opt.run(f, start);

  REQUIRE(result.converged());
  REQUIRE(result.point()[0] == Approx(1.0).margin(1e-4));
  REQUIRE(result.point()[1] == Approx(2.0).margin(1e-4));
  REQUIRE(result.point()[2] == Approx(3.0).margin(1e-4));
  REQUIRE(result.value() == Approx(0.0).margin(1e-6));
  REQUIRE(result.num_iterations() > 0);
  REQUIRE(result.grad() <= 1e-6);
}

TEST_CASE("Optimiser: 2D bounded, minimum inside box", "[optimiser]") {
  // f(x,y) = (x-1)^2 + (y-2)^2, minimum at (1, 2)
  // Box: [-10, 10] x [-10, 10]
  auto f = [](const Dual<double> &x, const Dual<double> &y) {
    return (x - 1.0) * (x - 1.0) + (y - 2.0) * (y - 2.0);
  };

  Vector lower{-10.0, -10.0};
  Vector upper{10.0, 10.0};
  Vector start{5.0, 5.0};

  Optimiser<double> opt(0.1, 1e-6, 1000);

  auto result = opt.run(f, start, lower, upper);

  REQUIRE(result.converged());
  REQUIRE(result.point()[0] == Approx(1.0).margin(1e-4));
  REQUIRE(result.point()[1] == Approx(2.0).margin(1e-4));
  REQUIRE(result.value() == Approx(0.0).margin(1e-6));
}

TEST_CASE("Optimiser: 1D bounded, minimum outside box", "[optimiser]") {
  // f(x) = (x-10)^2, minimum at x = 10
  // Box: [-1, 1], so should clamp to boundary at x = 1
  auto f = [](const Dual<double> &x) { return (x - 10.0) * (x - 10.0); };

  Vector<double, 1> lower{-1.0};
  Vector<double, 1> upper{1.0};
  Vector<double, 1> start{0.0};

  Optimiser<double> opt(0.1, 1e-6, 1000);

  auto result = opt.run(f, start, lower, upper);

  // Should reach the boundary at x = 1
  REQUIRE(result.point()[0] == Approx(1.0).margin(1e-4));

  // Function value at x=1 is (1-10)^2 = 81
  REQUIRE(result.value() == Approx(81.0).margin(1e-3));

  // Gradient at boundary should be non-zero (pointing outside box)
  // f'(x) = 2(x-10), at x=1: f'(1) = 2(1-10) = -18
  REQUIRE(result.grad() > 1e-6); // Should NOT have converged to zero gradient
}

TEST_CASE("Optimiser: max iterations limit", "[optimiser]") {
  // f(x) = x^2, but with very small step and low max_iterations
  auto f = [](const Dual<double> &x) { return x * x; };

  Vector<double, 1> start{100.0};

  // Small step, tight tolerance, but only 10 iterations
  Optimiser<double> opt(0.001, 1e-10, 10);

  auto result = opt.run(f, start);

  // Should hit max iterations before converging
  REQUIRE(result.num_iterations() == 10);
  REQUIRE_FALSE(result.converged()); // Should NOT converge with these settings
}

TEST_CASE("Optimiser: Result struct accessors", "[optimiser]") {
  // Simple test to verify all Result getters work
  auto f = [](const Dual<double> &x, const Dual<double> &y) {
    return x * x + y * y;
  };

  Vector<double, 2> start{5.0, 5.0};
  Optimiser<double> opt(0.1, 1e-6, 1000);

  auto result = opt.run(f, start);

  // Verify all accessors return valid values
  REQUIRE(result.point()[0] >= -1e10);
  REQUIRE(result.point()[1] >= -1e10);
  REQUIRE(result.value() >= 0.0);
  REQUIRE(result.grad() >= 0.0);
  REQUIRE(result.num_iterations() > 0);

  // For this convex problem, should converge
  REQUIRE(result.converged());
}

TEST_CASE("Optimiser: 4D function", "[optimiser]") {
  // f(w,x,y,z) = (w-1)^2 + (x-2)^2 + (y-3)^2 + (z-4)^2
  auto f = [](const Dual<double> &w, const Dual<double> &x,
              const Dual<double> &y, const Dual<double> &z) {
    return pow(w - 1, 2.0) + pow(x - 2, 2.0) + pow(y - 3, 2.0) +
           pow(z - 4, 2.0);
  };

  Vector start{0.0, 0.0, 0.0, 0.0};
  Optimiser<double> opt(0.1, 1e-6, 1000);

  auto result = opt.run(f, start);

  REQUIRE(result.converged());
  REQUIRE(result.point()[0] == Approx(1.0).margin(1e-4));
  REQUIRE(result.point()[1] == Approx(2.0).margin(1e-4));
  REQUIRE(result.point()[2] == Approx(3.0).margin(1e-4));
  REQUIRE(result.point()[3] == Approx(4.0).margin(1e-4));
}

TEST_CASE("Optimiser: CTAD with explicit starting point", "[optimiser][api]") {
  // Test that CTAD works for Vector construction
  auto f = [](const Dual<double> &x, const Dual<double> &y) {
    return (x - 3.0) * (x - 3.0) + (y - 4.0) * (y - 4.0);
  };

  Vector start{10.0, 10.0};  // CTAD deduces Vector<double, 2>
  Optimiser<double> opt(0.1, 1e-6, 1000);

  auto result = opt.run(f, start);

  REQUIRE(result.converged());
  REQUIRE(result.point()[0] == Approx(3.0).margin(1e-4));
  REQUIRE(result.point()[1] == Approx(4.0).margin(1e-4));
}

TEST_CASE("Optimiser: run_from_zero unbounded", "[optimiser][api]") {
  // Test run_from_zero for clean zero-initialization
  auto f = [](const Dual<double> &x, const Dual<double> &y,
              const Dual<double> &z) {
    return x * x + y * y + z * z;
  };

  Optimiser<double> opt(0.1, 1e-6, 1000);

  auto result = opt.run_from_zero<3>(f);

  REQUIRE(result.converged());
  REQUIRE(result.point()[0] == Approx(0.0).margin(1e-4));
  REQUIRE(result.point()[1] == Approx(0.0).margin(1e-4));
  REQUIRE(result.point()[2] == Approx(0.0).margin(1e-4));
  REQUIRE(result.value() == Approx(0.0).margin(1e-6));
}

TEST_CASE("Optimiser: run_from_zero bounded", "[optimiser][api]") {
  // Test run_from_zero with bounds
  auto f = [](const Dual<double> &x, const Dual<double> &y) {
    return (x - 5.0) * (x - 5.0) + (y - 5.0) * (y - 5.0);
  };

  Vector<double, 2> lower{-2.0, -2.0};
  Vector<double, 2> upper{2.0, 2.0};

  Optimiser<double> opt(0.1, 1e-6, 1000);

  auto result = opt.run_from_zero<2>(f, lower, upper);

  // Minimum is at (5,5) but box is [-2,2] x [-2,2]
  // Should clamp to boundary at (2,2)
  REQUIRE(result.point()[0] == Approx(2.0).margin(1e-4));
  REQUIRE(result.point()[1] == Approx(2.0).margin(1e-4));
  REQUIRE(result.grad() > 1e-6);  // Non-zero gradient at boundary
}

TEST_CASE("Optimiser: CTAD with high-dimensional vector", "[optimiser][api]") {
  // Test CTAD works for high-dimensional vectors
  auto f = [](const Dual<double> &a, const Dual<double> &b,
              const Dual<double> &c, const Dual<double> &d,
              const Dual<double> &e) {
    return a * a + b * b + c * c + d * d + e * e;
  };

  Vector start{1.0, 2.0, 3.0, 4.0, 5.0};  // CTAD: Vector<double, 5>
  Optimiser<double> opt(0.1, 1e-6, 1000);

  auto result = opt.run(f, start);

  REQUIRE(result.converged());
  for (size_t i = 0; i < 5; i++) {
    REQUIRE(result.point()[i] == Approx(0.0).margin(1e-4));
  }
}
