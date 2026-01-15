// Example: Custom model with variadic templated operator()
#include <gradual/optimiser.h>
#include <fmt/core.h>

// Custom model using variadic templates with exp, sin, and other operations
class ComplexModel {
public:
    // Variadic templated operator() works with any number of arguments
    // Demonstrates using elementary functions (exp, sin, pow) with fold expressions
    template<typename... Ts>
    auto operator()(Ts... args) const {
        // Create array-like access to arguments
        auto compute = [](auto... vals) {
            // Example function: sum of (exp(p_i) + sin(p_i) + p_i^2)
            // Using fold expression to combine results
            return (... + (exp(vals) + sin(vals) + vals * vals * 0.1));
        };
        return compute(args...);
    }
};

int main() {
    Vector init{0.5, 1.0, -0.5};

    Optimiser opt(0.01, 1.e-6);

    fmt::print("Minimising 3D function with exp and sin:\n");
    fmt::print("  f(p1, p2, p3) = Σ(exp(p_i) + sin(p_i) + 0.1*p_i²)\n\n");

    ComplexModel model;
    // Variadic lambdas work seamlessly!
    auto res = opt.minimise([&](auto... p) { return model(p...); }, init);

    fmt::print("Converged: {}\n", res.converged());
    auto point = res.point();
    fmt::print("Best point: ({:.4f}, {:.4f}, {:.4f})\n",
               point[0], point[1], point[2]);
    fmt::print("Function value: {:.6f}\n", res.value());
    fmt::print("Iterations: {}\n", res.num_iterations());

    return 0;
}
