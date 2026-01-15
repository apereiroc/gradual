// Example: Fitting a quadratic function to data points
#include <gradual/optimiser.h>
#include <fmt/core.h>

int main() {
    // Simulate data points from y = 2x^2 - 3x + 1
    const double data_x[] = {0.0, 1.0, 2.0, 3.0, 4.0};
    const double data_y[] = {1.0, 0.0, 3.0, 10.0, 21.0};
    const int n_points = 5;
    
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
    
    // Initial guess
    Vector init{0.0, 0.0, 0.0};
    
    Optimiser opt(0.001, 1.e-8);  // Smaller step size
    auto result = opt.minimise(cost, init);
    
    auto p = result.point();
    fmt::print("Quadratic fit: y = {:.3f}x² + {:.3f}x + {:.3f}\n", p[0], p[1], p[2]);
    fmt::print("Expected:      y = 2.000x² - 3.000x + 1.000\n");
    fmt::print("Converged: {}\n", result.converged());
    
    return 0;
}
