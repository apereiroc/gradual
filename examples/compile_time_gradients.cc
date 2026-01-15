// Example: Compile-time gradient computation using dual numbers
#include <gradual/gradient.h>
#include <gradual/dual.h>
#include <fmt/core.h>

int main() {
  // Compile-time polynomial derivative check
  constexpr auto poly = [](auto x) {
    return x * x * x;
  }; // x^3
  constexpr Dual<double> x(2.0, 1.0);
  constexpr auto result = poly(x);
  static_assert(result.real() == 8.0);  // 2^3 = 8
  static_assert(result.dual() == 12.0); // 3*2^2 = 12
  
  fmt::print("Polynomial derivative at x=2:\n");
  fmt::print("  f(x) = x^3\n");
  fmt::print("  f'(2) = {}\n", result.dual());

  // Compile-time gradient computation
  constexpr auto f = [](auto x, auto y) {
    return x * x + y * y;
  };
  constexpr Vector<double, 2> pt(3.0, 4.0);
  constexpr auto grad = gradient(f, pt);
  static_assert(grad[0] == 6.0); // ∂f/∂x = 2x = 6
  static_assert(grad[1] == 8.0); // ∂f/∂y = 2y = 8
  
  fmt::print("\nGradient at point (3, 4):\n");
  fmt::print("  f(x, y) = x^2 + y^2\n");
  fmt::print("  ∇f(3, 4) = ({}, {})\n", grad[0], grad[1]);
  
  fmt::print("\nAll compile-time checks passed.\n");

  return 0;
}
