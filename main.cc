#include "optimiser.h"
#include <fmt/core.h>

template <typename T>
T f(const T &x, const T &y) {
  return sin(x) * sin(y);
}

int main() {

  constexpr double step = 1.0e-3, grad_tol = 1.0e-6;

  Optimiser<double> opt(step, grad_tol);

  fmt::print("Running optimiser... ");

  auto res = opt.minimise_from_zero<2>([](auto x, auto y) { return x * x + y * y; });

  fmt::print("done\n");

  fmt::print("  Converged: {}\n", res.converged());

  auto params = res.point();
  fmt::print("  Params at minimum: ({}, {})\n", params[0], params[1]);
  fmt::print("  Value at minimum: {}\n", res.value());
  fmt::print("  Grad at minimum: {}\n", res.grad());
  fmt::print("  Number of iterations: {}\n", res.num_iterations());

  // Compile-time polynomial derivative check
  constexpr auto poly = [](auto x) {
    return x * x * x;
  }; // x^3
  constexpr Dual<double> x(2.0, 1.0);
  constexpr auto result = poly(x);
  static_assert(result.real() == 8.0);  // 2^3 = 8
  static_assert(result.dual() == 12.0); // 3*2^2 = 12

  // Compile-time gradient
  constexpr auto f = [](auto x, auto y) {
    return x * x + y * y;
  };
  constexpr Vector<double, 2> pt(3.0, 4.0);
  constexpr auto grad = gradient(f, pt);
  static_assert(grad[0] == 6.0); // ∂f/∂x = 2x = 6
  static_assert(grad[1] == 8.0); // ∂f/∂y = 2y = 8
  fmt::print("Compile-time checks passed.\n");

  return 0;
}
