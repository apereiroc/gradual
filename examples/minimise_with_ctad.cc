// Example: Dimension deduction via CTAD (Class Template Argument Deduction)
#include <gradual/optimiser.h>
#include <fmt/core.h>

int main() {
    // CTAD deduces Vector<double, 3> from initializer list
    Vector init{10.0, -5.0, 3.0};

    Optimiser opt(1.e-3, 1.e-6);

    fmt::print("Minimising 3D quadratic from initial point (10, -5, 3):\n");
    fmt::print("  f(x, y, z) = (x-1)² + (y-2)² + (z-5)²\n\n");

    auto res = opt.minimise([](auto x, auto y, auto z) {
        return pow(x - 1, 2.0) + pow(y - 2, 2.0) + pow(z - 5, 2.0);
    }, init);

    fmt::print("Converged: {}\n", res.converged());
    auto p = res.point();  // deduced to Vector<double, 3>
    fmt::print("Best point: ({:.2f}, {:.2f}, {:.2f})\n", p[0], p[1], p[2]);
    fmt::print("Iterations: {}\n", res.num_iterations());

    return 0;
}
