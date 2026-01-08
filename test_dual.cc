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
