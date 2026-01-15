#pragma once

#include <array>
#include <cmath>
#include <concepts>
#include <cstddef>

// Fixed-size numeric vector. Supports element-wise +/- and scaling by scalars,
// and dot product via operator*(Vector).
// Template parameter T must be floating-point or a Dual-like type.
// Scalar-over-vector division (s / v) is intentionally unsupported.

// Concept: Dual-like type (has value_type and real()/dual() accessors)
template <typename U>
concept dual_like = requires(U u) {
  typename U::value_type;
  { u.real() } -> std::same_as<typename U::value_type>;
  { u.dual() } -> std::same_as<typename U::value_type>;
};

// Concept: numeric-like scalar for Vector elements
// Vector can be filled with either floating-point or floating-point Dual types
template <typename U>
concept numeric_like = std::floating_point<U> || dual_like<U>;

template <typename T, std::size_t N>
  requires numeric_like<T>
class Vector {
private:
  std::array<T, N> m_data{};

public:
  // Declare friend functions - forward declaration needed
  template <typename U, std::size_t M>
  friend constexpr Vector<U, M> operator*(const U &scalar, const Vector<U, M> &vec);

  // Constructor
  constexpr Vector() = default;
  template <typename... Args>
  constexpr Vector(Args... args) : m_data{static_cast<T>(args)...} {
  }

  // Accessors
  [[nodiscard]] constexpr size_t size() const {
    return m_data.size();
  }
  constexpr const T &operator[](size_t index) const {
    return m_data.at(index);
  }
  constexpr T &operator[](size_t index) {
    return m_data.at(index);
  }

  // Norm
  [[nodiscard]] constexpr T norm2() const {
    T result(0);
    for (const T &x : m_data)
      result += x * x;

    return result;
  }

  [[nodiscard]] constexpr T norm() const {
    return std::sqrt(this->norm2());
  }

  // Vector-Vector binary ops
  constexpr Vector operator+(const Vector &other) const {
    Vector result;
    for (size_t i = 0; i < N; ++i) {
      result.m_data[i] = this->m_data[i] + other.m_data[i];
    }
    return result;
  }

  constexpr Vector operator-(const Vector &other) const {
    Vector result;
    for (size_t i = 0; i < N; ++i) {
      result.m_data[i] = this->m_data[i] - other.m_data[i];
    }
    return result;
  }

  // Dot product
  constexpr T operator*(const Vector &other) const {
    T result(0);
    for (size_t i = 0; i < N; ++i)
      result += this->m_data[i] * other[i];
    return result;
  }

  constexpr Vector operator*(const T &scalar) const {
    Vector result;
    for (size_t i = 0; i < N; ++i) {
      result.m_data[i] = this->m_data[i] * scalar;
    }
    return result;
  }

  constexpr Vector operator/(const T &scalar) const {
    Vector result;
    for (size_t i = 0; i < N; ++i) {
      result.m_data[i] = this->m_data[i] / scalar;
    }
    return result;
  }
};

// deduction guide for Vector constructor
// thus:
//   Vector v(1.0, 2.0, 3.0); infers Vector<double, 3>
//   Vector vf(1.0f, 2.0f); infers Vector<float, 2>
//   ... etc
// at least one argument is required; integral types are not supported.
template <typename T, typename... Rest>
Vector(T, Rest...) -> Vector<T, 1 + sizeof...(Rest)>;

// Scalar-Vector binary ops (free functions)
template <typename T, std::size_t N>
constexpr Vector<T, N> operator*(const T &scalar, const Vector<T, N> &vec) {
  return vec * scalar;
}
