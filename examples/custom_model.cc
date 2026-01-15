// Example: Using a custom model class with templated operator()
#include <fmt/core.h>
#include <gradual/dual.h>
#include <gradual/optimiser.h>

// Custom model class that works with both double and Dual<double>
class RosenbrockModel {
public:
  // Rosenbrock function: f(x,y) = (1-x)^2 + 100(y-x^2)^2
  // Templated to work with any numeric type
  template <typename T>
  T operator()(T x, T y) const {
    return pow(1 - x, 2) + 100 * pow(y - x * x, 2);
  }
};

int main() {
  Vector init{-1.0, 1.0}; // CTAD deduces Vector<double, 2>

  Optimiser opt(0.001, 1.e-6, 100000);

  fmt::print("Minimising Rosenbrock function from (-1, 1):\n");
  fmt::print("  f(x, y) = (1-x)² + 100(y-x²)²\n");
  fmt::print("  True minimum at (1, 1) with f(1,1) = 0\n\n");

  RosenbrockModel model;
  // auto res = opt.minimise([&](auto... p) { return model(p...); }, init); // This
  // works as well
  auto res = opt.minimise(model, init);

  fmt::print("Converged: {}\n", res.converged());
  auto p = res.point();
  fmt::print("Best point: ({:.4f}, {:.4f})\n", p[0], p[1]);
  fmt::print("Function value: {:.6f}\n", res.value());
  fmt::print("Iterations: {}\n", res.num_iterations());

  return 0;
}
