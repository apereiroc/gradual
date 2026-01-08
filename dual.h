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
  Dual operator+(const Dual &other) const {
    return Dual(this->m_real + other.m_real, this->m_dual + other.m_dual);
  }

  Dual operator*(const Dual &other) const {
    return Dual(this->m_real * other.m_real,
                this->m_dual * other.m_real + this->m_real * other.m_dual);
  }
};
