#pragma once

#include <cmath>

template <typename T> class Dual {
private:
  T m_real;
  T m_dual;

public:
  // Constructor
  Dual() : m_real(T(0)), m_dual(T(0)) {}
  Dual(T real, T dual) : m_real(real), m_dual(dual) {}

  // Accessors
  T real() const { return m_real; }
  T dual() const { return m_dual; }

  // Operator Overloads

  // Dual-Dual binary ops
  Dual operator+(const Dual &other) const {
    return Dual(this->m_real + other.m_real, this->m_dual + other.m_dual);
  }

  Dual operator-(const Dual &other) const {
    return Dual(this->m_real - other.m_real, this->m_dual - other.m_dual);
  }

  Dual operator*(const Dual &other) const {
    return Dual(this->m_real * other.m_real,
                this->m_dual * other.m_real + this->m_real * other.m_dual);
  }

  Dual operator/(const Dual &other) const {
    const T denom = other.m_real * other.m_real;
    return Dual(this->m_real / other.m_real,
                (this->m_dual * other.m_real - this->m_real * other.m_dual) /
                    denom);
  }

  // Dual-Scalar binary ops
  Dual operator+(const T &scalar) const {
    return Dual(this->m_real + scalar, this->m_dual);
  }

  Dual operator-(const T &scalar) const {
    return Dual(this->m_real - scalar, this->m_dual);
  }

  Dual operator*(const T &scalar) const {
    return Dual(this->m_real * scalar, this->m_dual * scalar);
  }

  Dual operator/(const T &scalar) const {
    return Dual(this->m_real / scalar, this->m_dual / scalar);
  }

  // Unary ops
  Dual operator-() const { return Dual(-this->m_real, -this->m_dual); }
};

// Scalar-Dual binary ops (free functions)
template <typename T> Dual<T> operator+(const T &scalar, const Dual<T> &dual) {
  return Dual<T>(scalar + dual.real(), dual.dual());
}

template <typename T> Dual<T> operator-(const T &scalar, const Dual<T> &dual) {
  return Dual<T>(scalar - dual.real(), -dual.dual());
}

template <typename T> Dual<T> operator*(const T &scalar, const Dual<T> &dual) {
  return Dual<T>(scalar * dual.real(), scalar * dual.dual());
}

template <typename T> Dual<T> operator/(const T &scalar, const Dual<T> &dual) {
  const T denom = dual.real() * dual.real();
  return Dual<T>(scalar / dual.real(), -scalar * dual.dual() / denom);
}

// Elementary operations

// sqrt(a + b eps) = sqrt(a) + b / (2 sqrt(a)) eps
template <typename T> Dual<T> sqrt(const Dual<T> &x) {
  const T r = std::sqrt(x.real());
  return Dual<T>(r, x.dual() / (T(2) * r));
}

// (a + b eps)^n = a^n + b n (a + b eps)^(n-1) eps
template <typename T> Dual<T> pow(const Dual<T> &x, const T &n) {
  const T a = x.real();
  const T r = std::pow(a, n);
  return Dual<T>(r, x.dual() * n * std::pow(a, n - T(1)));
}

// exp(a + b eps) = exp(a) + b exp(a) eps
template <typename T> Dual<T> exp(const Dual<T> &x) {
  const T r = std::exp(x.real());
  return Dual<T>(r, r * x.dual());
}

// log(a + b eps) = log(a) + b / a eps
template <typename T> Dual<T> log(const Dual<T> &x) {
  return Dual<T>(std::log(x.real()), x.dual() / x.real());
}

// sin(a + b eps) = sin(a) + b cos(a)
template <typename T> Dual<T> sin(const Dual<T> &x) {
  return Dual<T>(std::sin(x.real()), x.dual() * std::cos(x.real()));
}

// cos(a + b eps) = cos(a) - b sin(a)
template <typename T> Dual<T> cos(const Dual<T> &x) {
  return Dual<T>(std::cos(x.real()), -x.dual() * std::sin(x.real()));
}

// tan(a + b eps) = tan(a) + b ( 1 + tan(a)^2) eps
template <typename T> Dual<T> tan(const Dual<T> &x) {
  const T r = std::tan(x.real());
  return Dual<T>(r, x.dual() * (T(1) + r * r));
}
