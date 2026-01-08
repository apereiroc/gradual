#pragma once

template <typename T> class Dual {
private:
  T m_real;
  T m_dual;

public:
  // Constructor
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
