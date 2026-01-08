#include "dual.h"
#include <fmt/base.h>

int main() {
  fmt::print("Hello, world!\n");

  Dual<int> x(1, 1), y(1, 1);

  auto z = x + y;
  fmt::print("z = ({}, {})\n", z.real(), z.dual());

  z = x * y;

  fmt::print("z = ({}, {})\n", z.real(), z.dual());

  return 0;
}
