#pragma once

#include "gradient.h"
#include "vector.h"
#include <algorithm>
#include <cstddef>
#include <limits>

template <typename T, std::size_t N>
class Result {
private:
  Vector<T, N> m_point{}; // optimal point, where the function is minimal
  T m_value{};            // function value at minimum
  T m_grad{};             // gradient value at minimum, which made the engine to stop
  std::size_t m_num_iterations{};
  bool m_converged{};

public:
  Result(const Vector<T, N> &point,
         T value,
         T grad,
         std::size_t num_iterations,
         bool converged)
      : m_point(point), m_value(value), m_grad(grad), m_num_iterations(num_iterations),
        m_converged(converged) {
  }

  const Vector<T, N> &point() const {
    return m_point;
  }
  T value() const {
    return m_value;
  }
  T grad() const {
    return m_grad;
  }
  std::size_t num_iterations() const {
    return m_num_iterations;
  }
  bool converged() const {
    return m_converged;
  }
};

template <typename T>
class Optimiser {
private:
  T m_step{}, m_grad_tol{};
  std::size_t m_max_iterations{};

public:
  Optimiser(T step, T grad_tol, std::size_t max_iterations = 10000)
      : m_step(step), m_grad_tol(grad_tol), m_max_iterations(max_iterations) {
  }

  // bounded optimization, (lower, upper)
  template <std::size_t N, typename Func>
  Result<T, N> run(Func f,
                   const Vector<T, N> &start,
                   const Vector<T, N> &lower,
                   const Vector<T, N> &upper) {
    std::size_t num_iterations{0};
    Vector<T, N> params{start};
    Vector<T, N> grad_vec{gradient(f, params)}; //  gradient at initial params
    T grad_norm{grad_vec.norm()};               // initial |∇f|
    // TODO: early exit if starting point is out of bounds?

    // main optimisation loop
    // stop when |∇f| < tol or max iterations reached
    while (grad_norm > m_grad_tol and num_iterations < m_max_iterations) {
      num_iterations++;

      // update params, perform clamping
      for (std::size_t i = 0; i < N; i++) {
        params[i] = std::clamp(params[i] - m_step * grad_vec[i], lower[i], upper[i]);
      }

      // compute new gradient and its magnitude
      grad_vec = gradient(f, params);
      grad_norm = grad_vec.norm();

      if (grad_norm <= m_grad_tol or num_iterations >= m_max_iterations)
        break;
    }

    // if |∇f| < tol, convergence; if num it >= max it, divergence
    const bool converged = grad_norm <= m_grad_tol;

    // evalute f(x1, x2, ..., xN)
    // need to convert to duals first
    T value = [&]<std::size_t... Indices>(std::index_sequence<Indices...>) {
      return f(Dual<T>(params[Indices], T(0))...).real();
    }(std::make_index_sequence<N>{});

    // return result info to the user
    return Result<T, N>(params, value, grad_norm, num_iterations, converged);
  }

  // unbounded optimization, (-infty, infty)
  template <std::size_t N, typename Func>
  Result<T, N> run(Func f, const Vector<T, N> &start) {
    Vector<T, N> lower{}, upper{};
    for (std::size_t i = 0; i < N; i++) {
      lower[i] = -std::numeric_limits<T>::max();
      upper[i] = std::numeric_limits<T>::max();
    }
    return run(f, start, lower, upper);
  }

  // unbounded optimization from zero starting point
  template <std::size_t N, typename Func>
  Result<T, N> run_from_zero(Func f) {
    return run(f, Vector<T, N>{});
  }

  // bounded optimization from zero starting point
  template <std::size_t N, typename Func>
  Result<T, N>
  run_from_zero(Func f, const Vector<T, N> &lower, const Vector<T, N> &upper) {
    return run(f, Vector<T, N>{}, lower, upper);
  }
};
