// Example: Minimisation from zero using explicit dimension specification
#include <gradual/optimiser.h>
#include <fmt/core.h>

int main() {
    constexpr double step = 1.0e-3, grad_tol = 1.0e-6;
    Optimiser<double> opt(step, grad_tol);

    fmt::print("Minimising from zero with explicit dimension specification:\n");
    fmt::print("  f(x, y) = x² + y²\n\n");

    // minimise_from_zero<N> requires explicit template argument for dimension
    auto result = opt.minimise_from_zero<2>([](auto x, auto y) {
        return x * x + y * y;
    });

    fmt::print("Converged: {}\n", result.converged());
    fmt::print("Best point: ({}, {})\n", result.point()[0], result.point()[1]);
    fmt::print("Function value: {}\n", result.value());
    fmt::print("Gradient magnitude: {}\n", result.grad());
    fmt::print("Iterations: {}\n", result.num_iterations());

    return 0;
}
