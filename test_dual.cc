#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "dual.h"

using Catch::Matchers::WithinAbs;
using Catch::Approx;

TEST_CASE("Dual number construction", "[dual]") {
    SECTION("Constructor with real and dual parts") {
        Dual<double> d(3.0, 1.0);
        REQUIRE(d.real() == 3.0);
        REQUIRE(d.dual() == 1.0);
    }
}

TEST_CASE("Dual number addition", "[dual]") {
    Dual<double> a(2.0, 1.0);
    Dual<double> b(3.0, 1.0);
    
    auto result = a + b;
    
    REQUIRE(result.real() == 5.0);
    REQUIRE(result.dual() == 2.0);
}

TEST_CASE("Dual number subtraction", "[dual]") {
    Dual<double> a(5.0, 2.0);
    Dual<double> b(3.0, 1.0);

    auto result = a - b;

    REQUIRE(result.real() == 2.0);
    REQUIRE(result.dual() == 1.0);

    a = {100, 50};
    b = {70, 10};
    result = a - b;

    REQUIRE(result.real() == 30);
    REQUIRE(result.dual() == 40);
}

TEST_CASE("Dual number multiplication - product rule", "[dual]") {
    // f(x) = x, g(x) = x => f*g = x^2
    // At x=3: f'(3)*g(3) + f(3)*g'(3) = 1*3 + 3*1 = 6
    Dual<double> x(3.0, 1.0);
    
    auto x_squared = x * x;
    
    REQUIRE(x_squared.real() == 9.0);  // 3^2 = 9
    REQUIRE(x_squared.dual() == 6.0);  // d/dx(x^2) at x=3 is 2*3 = 6
}

TEST_CASE("Dual number division - quotient rule", "[dual]") {
    // f(x) = x, g(x) = 2x => f/g = 0.5, d/dx(f/g) = (1*2x - x*2)/ (2x)^2 = 0
    Dual<double> f(4.0, 1.0);   // x=4, df/dx=1
    Dual<double> g(8.0, 2.0);   // 2x=8, dg/dx=2

    auto result = f / g;

    REQUIRE(result.real() == 0.5);
    REQUIRE(result.dual() == 0.0);
}

TEST_CASE("Unary negation", "[dual]") {
    Dual<double> x(2.5, -1.5);
    auto neg = -x;
    REQUIRE(neg.real() == -2.5);
    REQUIRE(neg.dual() == 1.5);
}

TEST_CASE("Dual-scalar interactions", "[dual]") {
    Dual<double> x(4.0, 2.0);

    SECTION("Add scalar") {
        auto r1 = x + 3.0;
        auto r2 = 3.0 + x;
        REQUIRE(r1.real() == 7.0);
        REQUIRE(r1.dual() == 2.0);
        REQUIRE(r2.real() == 7.0);
        REQUIRE(r2.dual() == 2.0);
    }

    SECTION("Subtract scalar") {
        auto r1 = x - 1.0;
        auto r2 = 10.0 - x;
        REQUIRE(r1.real() == 3.0);
        REQUIRE(r1.dual() == 2.0);
        REQUIRE(r2.real() == 6.0);
        REQUIRE(r2.dual() == -2.0);
    }

    SECTION("Multiply scalar") {
        auto r1 = x * 2.0;
        auto r2 = 0.5 * x;
        REQUIRE(r1.real() == 8.0);
        REQUIRE(r1.dual() == 4.0);
        REQUIRE(r2.real() == 2.0);
        REQUIRE(r2.dual() == 1.0);
    }

    SECTION("Divide scalar") {
        auto r1 = x / 2.0;      // (4,2)/2 -> (2,1)
        auto r2 = 20.0 / x;     // (20/x, -20*grad/x^2)
        REQUIRE(r1.real() == 2.0);
        REQUIRE(r1.dual() == 1.0);
        REQUIRE(r2.real() == 5.0);
        REQUIRE(r2.dual() == Approx(-2.5));
    }
}

TEST_CASE("Derivative of x^2", "[dual][derivatives]") {
    // Test d/dx(x^2) = 2x at various points
    auto f = [](Dual<double> x) { return x * x; };
    
    SECTION("At x = 2") {
        Dual<double> x(2.0, 1.0);
        auto result = f(x);
        REQUIRE(result.real() == 4.0);    // 2^2 = 4
        REQUIRE(result.dual() == 4.0);    // 2*2 = 4
    }
    
    SECTION("At x = 5") {
        Dual<double> x(5.0, 1.0);
        auto result = f(x);
        REQUIRE(result.real() == 25.0);   // 5^2 = 25
        REQUIRE(result.dual() == 10.0);   // 2*5 = 10
    }
}

TEST_CASE("Derivative of x^3", "[dual][derivatives]") {
    // f(x) = x^3 = x * x * x
    // f'(x) = 3*x^2
    auto f = [](Dual<double> x) { 
        return x * x * x; 
    };
    
    SECTION("At x = 2") {
        Dual<double> x(2.0, 1.0);
        auto result = f(x);
        REQUIRE(result.real() == 8.0);    // 2^3 = 8
        REQUIRE(result.dual() == 12.0);   // 3*2^2 = 12
    }
    
    SECTION("At x = 3") {
        Dual<double> x(3.0, 1.0);
        auto result = f(x);
        REQUIRE(result.real() == 27.0);   // 3^3 = 27
        REQUIRE(result.dual() == 27.0);   // 3*3^2 = 27
    }
}

TEST_CASE("Derivative of reciprocal", "[dual][derivatives]") {
    // f(x) = 1/x, f'(x) = -1/x^2
    auto f = [](Dual<double> x) { return Dual<double>(1.0, 0.0) / x; };

    Dual<double> x(2.0, 1.0);
    auto result = f(x);

    REQUIRE(result.real() == 0.5);
    REQUIRE(result.dual() == Approx(-0.25));
}

TEST_CASE("Derivative of polynomial", "[dual][derivatives]") {
    // f(x) = 2x^2 + 3x + 1
    // f'(x) = 4x + 3
    auto f = [](Dual<double> x) {
        Dual<double> two(2.0, 0.0);
        Dual<double> three(3.0, 0.0);
        Dual<double> one(1.0, 0.0);
        return two * x * x + three * x + one;
    };
    
    SECTION("At x = 0") {
        Dual<double> x(0.0, 1.0);
        auto result = f(x);
        REQUIRE(result.real() == 1.0);    // 2*0 + 3*0 + 1 = 1
        REQUIRE(result.dual() == 3.0);    // 4*0 + 3 = 3
    }
    
    SECTION("At x = 1") {
        Dual<double> x(1.0, 1.0);
        auto result = f(x);
        REQUIRE(result.real() == 6.0);    // 2*1 + 3*1 + 1 = 6
        REQUIRE(result.dual() == 7.0);    // 4*1 + 3 = 7
    }
    
    SECTION("At x = 2") {
        Dual<double> x(2.0, 1.0);
        auto result = f(x);
        REQUIRE(result.real() == 15.0);   // 2*4 + 3*2 + 1 = 15
        REQUIRE(result.dual() == 11.0);   // 4*2 + 3 = 11
    }
}

TEST_CASE("Constant propagation", "[dual]") {
    // Adding/multiplying by constants should preserve gradient correctly
    Dual<double> x(5.0, 1.0);
    Dual<double> constant(10.0, 0.0);
    
    SECTION("Constant addition") {
        auto result = x + constant;
        REQUIRE(result.real() == 15.0);
        REQUIRE(result.dual() == 1.0);  // d/dx(x + c) = 1
    }
    
    SECTION("Constant multiplication") {
        auto result = x * constant;
        REQUIRE(result.real() == 50.0);
        REQUIRE(result.dual() == 10.0); // d/dx(c*x) = c
    }
}

TEST_CASE("Elementary function: sqrt", "[dual][elementary]") {
    // f(x) = sqrt(x), f'(x) = 1/(2*sqrt(x))
    
    SECTION("At x = 4") {
        Dual<double> x(4.0, 1.0);
        auto result = sqrt(x);
        REQUIRE(result.real() == 2.0);           // sqrt(4) = 2
        REQUIRE(result.dual() == Approx(0.25));  // 1/(2*sqrt(4)) = 1/4
    }
    
    SECTION("At x = 9") {
        Dual<double> x(9.0, 1.0);
        auto result = sqrt(x);
        REQUIRE(result.real() == 3.0);              // sqrt(9) = 3
        REQUIRE(result.dual() == Approx(1.0/6.0));  // 1/(2*sqrt(9)) = 1/6
    }
    
    SECTION("At x = 16") {
        Dual<double> x(16.0, 1.0);
        auto result = sqrt(x);
        REQUIRE(result.real() == 4.0);            // sqrt(16) = 4
        REQUIRE(result.dual() == Approx(0.125));  // 1/(2*sqrt(16)) = 1/8
    }
}

TEST_CASE("Elementary function: pow", "[dual][elementary]") {
    // f(x) = x^n, f'(x) = n*x^(n-1)
    
    SECTION("x^2 at x = 3") {
        Dual<double> x(3.0, 1.0);
        auto result = pow(x, 2.0);
        REQUIRE(result.real() == 9.0);    // 3^2 = 9
        REQUIRE(result.dual() == 6.0);    // 2*3^1 = 6
    }
    
    SECTION("x^3 at x = 2") {
        Dual<double> x(2.0, 1.0);
        auto result = pow(x, 3.0);
        REQUIRE(result.real() == 8.0);    // 2^3 = 8
        REQUIRE(result.dual() == 12.0);   // 3*2^2 = 12
    }
    
    SECTION("x^0.5 at x = 4") {
        Dual<double> x(4.0, 1.0);
        auto result = pow(x, 0.5);
        REQUIRE(result.real() == 2.0);           // 4^0.5 = 2
        REQUIRE(result.dual() == Approx(0.25));  // 0.5*4^(-0.5) = 0.25
    }
    
    SECTION("x^(-1) at x = 2") {
        Dual<double> x(2.0, 1.0);
        auto result = pow(x, -1.0);
        REQUIRE(result.real() == 0.5);            // 2^(-1) = 0.5
        REQUIRE(result.dual() == Approx(-0.25));  // -1*2^(-2) = -0.25
    }
}

TEST_CASE("Elementary function: exp", "[dual][elementary]") {
    // f(x) = exp(x), f'(x) = exp(x)
    
    SECTION("At x = 0") {
        Dual<double> x(0.0, 1.0);
        auto result = exp(x);
        REQUIRE(result.real() == Approx(1.0));  // e^0 = 1
        REQUIRE(result.dual() == Approx(1.0));  // e^0 = 1
    }
    
    SECTION("At x = 1") {
        Dual<double> x(1.0, 1.0);
        auto result = exp(x);
        REQUIRE(result.real() == Approx(std::exp(1.0)));   // e^1
        REQUIRE(result.dual() == Approx(std::exp(1.0)));   // e^1
    }
    
    SECTION("At x = 2") {
        Dual<double> x(2.0, 1.0);
        auto result = exp(x);
        REQUIRE(result.real() == Approx(std::exp(2.0)));   // e^2
        REQUIRE(result.dual() == Approx(std::exp(2.0)));   // e^2
    }
}

TEST_CASE("Elementary function: log", "[dual][elementary]") {
    // f(x) = log(x), f'(x) = 1/x
    
    SECTION("At x = 1") {
        Dual<double> x(1.0, 1.0);
        auto result = log(x);
        REQUIRE(result.real() == Approx(0.0));  // log(1) = 0
        REQUIRE(result.dual() == Approx(1.0));  // 1/1 = 1
    }
    
    SECTION("At x = e") {
        Dual<double> x(std::exp(1.0), 1.0);
        auto result = log(x);
        REQUIRE(result.real() == Approx(1.0));              // log(e) = 1
        REQUIRE(result.dual() == Approx(1.0/std::exp(1.0))); // 1/e
    }
    
    SECTION("At x = 2") {
        Dual<double> x(2.0, 1.0);
        auto result = log(x);
        REQUIRE(result.real() == Approx(std::log(2.0)));  // log(2)
        REQUIRE(result.dual() == Approx(0.5));            // 1/2
    }
}

TEST_CASE("Elementary function: sin", "[dual][elementary]") {
    // f(x) = sin(x), f'(x) = cos(x)
    
    SECTION("At x = 0") {
        Dual<double> x(0.0, 1.0);
        auto result = sin(x);
        REQUIRE(result.real() == Approx(0.0));  // sin(0) = 0
        REQUIRE(result.dual() == Approx(1.0));  // cos(0) = 1
    }
    
    SECTION("At x = π/2") {
        constexpr double pi = 3.14159265358979323846;
        Dual<double> x(pi/2.0, 1.0);
        auto result = sin(x);
        REQUIRE(result.real() == Approx(1.0));  // sin(π/2) = 1
        REQUIRE(result.dual() == Approx(0.0).margin(1e-10));  // cos(π/2) ≈ 0
    }
    
    SECTION("At x = π") {
        constexpr double pi = 3.14159265358979323846;
        Dual<double> x(pi, 1.0);
        auto result = sin(x);
        REQUIRE(result.real() == Approx(0.0).margin(1e-10));  // sin(π) ≈ 0
        REQUIRE(result.dual() == Approx(-1.0));               // cos(π) = -1
    }
}

TEST_CASE("Elementary function: cos", "[dual][elementary]") {
    // f(x) = cos(x), f'(x) = -sin(x)
    
    SECTION("At x = 0") {
        Dual<double> x(0.0, 1.0);
        auto result = cos(x);
        REQUIRE(result.real() == Approx(1.0));   // cos(0) = 1
        REQUIRE(result.dual() == Approx(0.0));   // -sin(0) = 0
    }
    
    SECTION("At x = π/2") {
        constexpr double pi = 3.14159265358979323846;
        Dual<double> x(pi/2.0, 1.0);
        auto result = cos(x);
        REQUIRE(result.real() == Approx(0.0).margin(1e-10));  // cos(π/2) ≈ 0
        REQUIRE(result.dual() == Approx(-1.0));               // -sin(π/2) = -1
    }
    
    SECTION("At x = π") {
        constexpr double pi = 3.14159265358979323846;
        Dual<double> x(pi, 1.0);
        auto result = cos(x);
        REQUIRE(result.real() == Approx(-1.0));               // cos(π) = -1
        REQUIRE(result.dual() == Approx(0.0).margin(1e-10));  // -sin(π) ≈ 0
    }
}

TEST_CASE("Elementary function: tan", "[dual][elementary]") {
    // f(x) = tan(x), f'(x) = 1 + tan^2(x) = sec^2(x)
    
    SECTION("At x = 0") {
        Dual<double> x(0.0, 1.0);
        auto result = tan(x);
        REQUIRE(result.real() == Approx(0.0));  // tan(0) = 0
        REQUIRE(result.dual() == Approx(1.0));  // 1 + tan^2(0) = 1
    }
    
    SECTION("At x = π/4") {
        constexpr double pi = 3.14159265358979323846;
        Dual<double> x(pi/4.0, 1.0);
        auto result = tan(x);
        REQUIRE(result.real() == Approx(1.0));  // tan(π/4) = 1
        REQUIRE(result.dual() == Approx(2.0));  // 1 + tan^2(π/4) = 1 + 1 = 2
    }
    
    SECTION("At x = π/6") {
        constexpr double pi = 3.14159265358979323846;
        Dual<double> x(pi/6.0, 1.0);
        auto result = tan(x);
        double expected_tan = std::tan(pi/6.0);  // 1/sqrt(3)
        double expected_deriv = 1.0 + expected_tan * expected_tan;
        REQUIRE(result.real() == Approx(expected_tan));
        REQUIRE(result.dual() == Approx(expected_deriv));
    }
}

TEST_CASE("Composite elementary functions", "[dual][elementary]") {
    // Test composition of elementary functions
    
    SECTION("exp(sin(x)) at x = 0") {
        // f(x) = exp(sin(x)), f'(x) = exp(sin(x)) * cos(x)
        Dual<double> x(0.0, 1.0);
        auto result = exp(sin(x));
        REQUIRE(result.real() == Approx(1.0));  // exp(sin(0)) = exp(0) = 1
        REQUIRE(result.dual() == Approx(1.0));  // exp(0) * cos(0) = 1 * 1 = 1
    }
    
    SECTION("log(sqrt(x)) at x = 4") {
        // f(x) = log(sqrt(x)) = 0.5*log(x), f'(x) = 0.5/x
        Dual<double> x(4.0, 1.0);
        auto result = log(sqrt(x));
        REQUIRE(result.real() == Approx(std::log(2.0)));  // log(2)
        REQUIRE(result.dual() == Approx(0.125));          // 1/(2*4) = 1/8
    }
    
    SECTION("sqrt(exp(x)) at x = 2") {
        // f(x) = sqrt(exp(x)) = exp(x/2), f'(x) = 0.5*exp(x/2)
        Dual<double> x(2.0, 1.0);
        auto result = sqrt(exp(x));
        double expected = std::sqrt(std::exp(2.0));
        REQUIRE(result.real() == Approx(expected));
        REQUIRE(result.dual() == Approx(0.5 * expected));
    }
}
