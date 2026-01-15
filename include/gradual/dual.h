#pragma once

#include <cmath>
#include <concepts>

// Dual numbers represent values of the form a + b ε where ε^2 = 0.
// Template parameter T must be a floating-point type.
template <typename T>
  requires std::floating_point<T>
class Dual {
private:
  T m_real;
  T m_dual;

public:
  // Public type alias to support concept detection in Vector
  using value_type = T;

  // Constructor
  constexpr Dual() : m_real(T(0)), m_dual(T(0)) {
  }
  constexpr Dual(T real, T dual) : m_real(real), m_dual(dual) {
  }

  // Accessors
  [[nodiscard]] constexpr T real() const {
    return m_real;
  }
  [[nodiscard]] constexpr T dual() const {
    return m_dual;
  }

  // Operator Overloads

  // Dual-Dual binary ops
  constexpr Dual operator+(const Dual &other) const {
    return Dual(this->m_real + other.m_real, this->m_dual + other.m_dual);
  }

  constexpr Dual operator-(const Dual &other) const {
    return Dual(this->m_real - other.m_real, this->m_dual - other.m_dual);
  }

  constexpr Dual operator*(const Dual &other) const {
    return Dual(this->m_real * other.m_real,
                this->m_dual * other.m_real + this->m_real * other.m_dual);
  }

  constexpr Dual operator/(const Dual &other) const {
    const T denom = other.m_real * other.m_real;
    return Dual(this->m_real / other.m_real,
                (this->m_dual * other.m_real - this->m_real * other.m_dual) / denom);
  }

  // Dual-Scalar binary ops
  constexpr Dual operator+(const T &scalar) const {
    return Dual(this->m_real + scalar, this->m_dual);
  }

  constexpr Dual operator-(const T &scalar) const {
    return Dual(this->m_real - scalar, this->m_dual);
  }

  constexpr Dual operator*(const T &scalar) const {
    return Dual(this->m_real * scalar, this->m_dual * scalar);
  }

  constexpr Dual operator/(const T &scalar) const {
    return Dual(this->m_real / scalar, this->m_dual / scalar);
  }

  // Unary ops
  constexpr Dual operator-() const {
    return Dual(-this->m_real, -this->m_dual);
  }
};

// Scalar-Dual binary ops (free functions)
template <typename T>
constexpr Dual<T> operator+(const T &scalar, const Dual<T> &dual) {
  return Dual<T>(scalar + dual.real(), dual.dual());
}

template <typename T>
constexpr Dual<T> operator-(const T &scalar, const Dual<T> &dual) {
  return Dual<T>(scalar - dual.real(), -dual.dual());
}

template <typename T>
constexpr Dual<T> operator*(const T &scalar, const Dual<T> &dual) {
  return Dual<T>(scalar * dual.real(), scalar * dual.dual());
}

template <typename T>
constexpr Dual<T> operator/(const T &scalar, const Dual<T> &dual) {
  const T denom = dual.real() * dual.real();
  return Dual<T>(scalar / dual.real(), -scalar * dual.dual() / denom);
}

// Integer-Dual binary ops (allows operations like 1 + x where x is Dual)
template <typename T, std::integral I>
  requires std::floating_point<T>
constexpr Dual<T> operator+(I scalar, const Dual<T> &dual) {
  return T(scalar) + dual;
}

template <typename T, std::integral I>
  requires std::floating_point<T>
constexpr Dual<T> operator-(I scalar, const Dual<T> &dual) {
  return T(scalar) - dual;
}

template <typename T, std::integral I>
  requires std::floating_point<T>
constexpr Dual<T> operator*(I scalar, const Dual<T> &dual) {
  return T(scalar) * dual;
}

template <typename T, std::integral I>
  requires std::floating_point<T>
constexpr Dual<T> operator/(I scalar, const Dual<T> &dual) {
  return T(scalar) / dual;
}

// Elementary operations
// These functions follow standard derivative rules and operate on Dual<T> where
// T is floating-point
// Note that std::cmath does not support (yet) constexpr functions

// sqrt(a + b ε) = sqrt(a) + (b / (2 sqrt(a))) ε
template <typename T>
  requires std::floating_point<T>
Dual<T> sqrt(const Dual<T> &x) {
  const T r = std::sqrt(x.real());
  return {r, x.dual() / (T(2) * r)};
}

// (a + b ε)^n = a^n + b·n·a^{n-1} ε
template <typename T>
  requires std::floating_point<T>
Dual<T> pow(const Dual<T> &x, const T &n) {
  const T a = x.real();
  const T r = std::pow(a, n);
  return {r, x.dual() * n * std::pow(a, n - T(1))};
}

// Overload for integer exponents (allows pow(x, 2) instead of pow(x, 2.0))
template <typename T, std::integral I>
  requires std::floating_point<T>
Dual<T> pow(const Dual<T> &x, I n) {
  return pow(x, T(n));
}

// exp(a + b ε) = exp(a) + b·exp(a) ε
template <typename T>
  requires std::floating_point<T>
Dual<T> exp(const Dual<T> &x) {
  const T r = std::exp(x.real());
  return {r, r * x.dual()};
}

// log(a + b ε) = log(a) + (b / a) ε
template <typename T>
  requires std::floating_point<T>
Dual<T> log(const Dual<T> &x) {
  return {std::log(x.real()), x.dual() / x.real()};
}

// sin(a + b ε) = sin(a) + b·cos(a) ε
template <typename T>
  requires std::floating_point<T>
Dual<T> sin(const Dual<T> &x) {
  return {std::sin(x.real()), x.dual() * std::cos(x.real())};
}

// cos(a + b ε) = cos(a) − b·sin(a) ε
template <typename T>
  requires std::floating_point<T>
Dual<T> cos(const Dual<T> &x) {
  return {std::cos(x.real()), -x.dual() * std::sin(x.real())};
}

// tan(a + b ε) = tan(a) + b·(1 + tan(a)^2) ε
template <typename T>
  requires std::floating_point<T>
Dual<T> tan(const Dual<T> &x) {
  const T r = std::tan(x.real());
  return {r, x.dual() * (T(1) + r * r)};
}
