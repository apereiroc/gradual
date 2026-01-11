#pragma once

#include <array>

template <typename T, std::size_t N> class Vector {
private:
  std::array<T, N> m_data{};

public:
  // Declare friend functions - forward declaration needed
  template <typename U, std::size_t M>
  friend Vector<U, M> operator*(const U &scalar, const Vector<U, M> &vec);

  template <typename U, std::size_t M>
  friend Vector<U, M> operator/(const U &scalar, const Vector<U, M> &vec);

  // Constructor
  Vector() = default;
  template <typename... Args>
  Vector(Args... args) : m_data{static_cast<T>(args)...} {}

  // Accessors
  size_t size() const { return m_data.size(); }
  const T &operator[](size_t index) const { return m_data.at(index); }
  T &operator[](size_t index) { return m_data.at(index); }

  // Norm
  T norm2() const {
    T result(0);
    for (const T &x : m_data)
      result += x * x;

    return result;
  }

  T norm() const { return std::sqrt(this->norm2()); }

  // Vector-Vector binary ops
  Vector operator+(const Vector &other) const {
    Vector result;
    // result.m_data.resize(this->size());
    for (size_t i = 0; i < this->size(); ++i) {
      result.m_data[i] = this->m_data[i] + other.m_data[i];
    }
    return result;
  }

  Vector operator-(const Vector &other) const {
    Vector result;
    // result.m_data.resize(this->size());
    for (size_t i = 0; i < this->size(); ++i) {
      result.m_data[i] = this->m_data[i] - other.m_data[i];
    }
    return result;
  }

  T operator*(const Vector &other) const {
    T result(0);
    for (size_t i = 0; i < N; ++i)
      result += this->m_data[i] * other[i];
    return result;
  }

  Vector operator*(const T &scalar) const {
    Vector result;
    // result.m_data.resize(this->size());
    for (size_t i = 0; i < this->size(); ++i) {
      result.m_data[i] = this->m_data[i] * scalar;
    }
    return result;
  }

  Vector operator/(const T &scalar) const {
    Vector result;
    // result.m_data.resize(this->size());
    for (size_t i = 0; i < this->size(); ++i) {
      result.m_data[i] = this->m_data[i] / scalar;
    }
    return result;
  }
};

// deduction guide for Vector constructor
// thus:
//   Vector v(1.0, 2.0, 3.0); infers Vector<double, 3>
//   Vector vi(1, 2, 3, 4); infers Vector<int, 4>
//   Vector vf(1.0f, 2.0f); infers Vector<float, 2>
//   ... etc
// Note: at least one argument is required
template <typename T, typename... Rest>
Vector(T, Rest...) -> Vector<T, 1 + sizeof...(Rest)>;

// Scalar-Vector binary ops (free functions)
template <typename T, std::size_t N>
Vector<T, N> operator*(const T &scalar, const Vector<T, N> &vec) {
  return vec * scalar;
}

template <typename T, std::size_t N>
Vector<T, N> operator/(const T &scalar, const Vector<T, N> &vec) {
  Vector<T, N> result;
  // Create result by dividing scalar by each element
  for (size_t i = 0; i < vec.size(); ++i) {
    // Use a temporary vector approach
    T element_result = scalar / vec[i];
    if (result.size() <= i) {
      result.m_data.push_back(element_result);
    }
  }
  return result;
}
