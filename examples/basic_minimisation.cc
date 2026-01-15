// Example: Basic gradient descent minimisation
#include <gradual/optimiser.h>
#include <fmt/core.h>

int main() {
  constexpr double step = 1.0e-3, grad_tol = 1.0e-6;

  Optimiser<double> opt(step, grad_tol);

  fmt::print("Minimising f(x, y) = x² + y²\n");
  fmt::print("Running optimiser... ");

  auto res = opt.minimise_from_zero<2>([](auto x, auto y) { return x * x + y * y; });

  fmt::print("done\n");

  fmt::print("  Converged: {}\n", res.converged());

  auto params = res.point();
  fmt::print("  Params at minimum: ({}, {})\n", params[0], params[1]);
  fmt::print("  Value at minimum: {}\n", res.value());
  fmt::print("  Gradient magnitude at minimum: {}\n", res.grad());
  fmt::print("  Number of iterations: {}\n", res.num_iterations());

  return 0;
}
