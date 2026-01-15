// Example: Fitting a quadratic function to randomly generated data points
#include <gradual/optimiser.h>
#include <fmt/core.h>
#include <random>
#include <array>

int main() {
    // Generate random data points from y = a*x^2 + b*x + c
    // True parameters: a=0.5, b=1.0, c=2.0
    constexpr int n_points = 8;
    constexpr double true_a = 0.5, true_b = 1.0, true_c = 2.0;
    
    std::mt19937 gen(42);  // Fixed seed for reproducibility
    std::uniform_real_distribution<> x_dist(-2.0, 2.0);
    std::uniform_real_distribution<> noise_dist(-0.2, 0.2);
    
    std::array<double, n_points> data_x;
    std::array<double, n_points> data_y;
    
    for (int i = 0; i < n_points; ++i) {
        data_x[i] = x_dist(gen);
        double y_true = true_a * data_x[i] * data_x[i] + true_b * data_x[i] + true_c;
        data_y[i] = y_true + noise_dist(gen);  // Add noise
    }
    
    // Define cost function (sum of squared residuals)
    auto cost = [&](auto a, auto b, auto c) {
        decltype(a) sum = a * 0.0;  // Initialize with correct type
        for (int i = 0; i < n_points; ++i) {
            auto predicted = a * data_x[i] * data_x[i] + b * data_x[i] + c;
            auto residual = predicted - data_y[i];
            sum = sum + residual * residual;
        }
        return sum;
    };
    
    // Initial guess (different from true parameters)
    Vector init{0.1, 0.5, 1.0};
    
    Optimiser opt(0.01, 1.e-6);
    auto result = opt.minimise(cost, init);
    
    auto p = result.point();
    fmt::print("Fitted parameters:    a={:.3f}, b={:.3f}, c={:.3f}\n", p[0], p[1], p[2]);
    fmt::print("True parameters:      a={:.3f}, b={:.3f}, c={:.3f}\n", true_a, true_b, true_c);
    fmt::print("Iterations: {}, Converged: {}\n", result.num_iterations(), result.converged());
    
    return 0;
}
