#include "optimiser.h"
#include <fmt/base.h>

template <typename T> T f(const T &x, const T &y) { return sin(x) * sin(y); }

int main() {

  Vector init_point{10.0, 20.0};

  constexpr double step = 1.0e-3, grad_tol = 1.0e-6;

  Optimiser opt(step, grad_tol);

  fmt::print("Running optimiser... ");

  auto res = opt.run([](auto x, auto y) { return x * x + y * y; }, init_point);

  fmt::print("done\n");

  fmt::print("  Converged: {}\n", res.converged());

  auto params = res.point();
  fmt::print("  Params at minimum: ({}, {})\n", params[0], params[1]);
  fmt::print("  Value at minimum: {}\n", res.value());
  fmt::print("  Grad at minimum: {}\n", res.grad());
  fmt::print("  Number of iterations: {}\n", res.num_iterations());

  return 0;
}
