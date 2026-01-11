#pragma once

#include "dual.h"
#include "vector.h"
#include <cstddef>
#include <utility>

// construct the dual basis AT COMPILE TIME
//   - this function intended to get the vectors (1, ..., 0) , (0, 1, ..., 0),
//   ...., (0, ..., 1)
//   - given a point vec(a), this function returns vec(a) + vec(b) eps, where
//   vec(b) is (0, ..., 1, ...0) for index Dim (template)
template <typename T, std::size_t N, std::size_t Dim>
constexpr Vector<Dual<T>, N> make_dual_basis(const Vector<T, N> &point) {
  Vector<Dual<T>, N> result{};

  for (std::size_t j = 0; j < N; j++)
    result[j] = Dual<T>(point[j], j == Dim ? T(1) : T(0));

  return result;
}

// evaluate function with dual basis at dimension Dim
// this yields the gradient projected at dimension Dim
// i.e., ∂f/∂x_Dim
template <typename T, std::size_t N, std::size_t Dim, typename Func>
T get_partial_derivative(Func f, const Vector<T, N> &point) {
  auto duals = make_dual_basis<T, N, Dim>(point);

  // unpack duals into function call
  // evaluate f(vec(a) + vec(b) eps) = f(vec(a)) + grad_f * vec(b) eps
  // since vec(b) is non-zero only for index Dim, the dual part gives ∂f/∂x_Dim
  Dual<T> result =
      [&]<std::size_t... Indices>(std::index_sequence<Indices...>) {
        return f(duals[Indices]...);
      }(std::make_index_sequence<N>{});

  return result.dual();
}

// get all gradients using index_sequence
template <typename T, std::size_t N, typename Func, std::size_t... Dims>
Vector<T, N> get_gradient_impl(Func f, const Vector<T, N> &point,
                               std::index_sequence<Dims...>) {
  Vector<T, N> grad;
  // fill gradients
  (..., (grad[Dims] = get_partial_derivative<T, N, Dims, Func>(f, point)));

  return grad;
}

template <typename T, std::size_t N, typename Func>
Vector<T, N> gradient(Func f, const Vector<T, N> &point) {
  return get_gradient_impl(f, point, std::make_index_sequence<N>{});
}
