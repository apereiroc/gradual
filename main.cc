#include "gradient.h"
#include <fmt/base.h>

using dual_d = Dual<double>;
dual_d f(const dual_d &x, const dual_d &y, const dual_d &z) {
  return x * x + y * y + z * z;
}

int main() {

  Vector<double, 3> point(1, 2, 3); // Vector<double, 3>
  auto grad = gradient(f, point);

  fmt::print("∇f({},{},{}) = ({}, {}, {})\n", point[0], point[1], point[2],
             grad[0], grad[1], grad[2]);
  fmt::print("|∇f| = {}\n", grad.norm());

  return 0;
}
