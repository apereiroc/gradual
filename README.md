# Gradual 

**Gradual** (gradients and duals) is a minimalist C++20 automatic differentiation library prioritising clarity and modern design over raw performance.

Currently, it supports forward-mode differentiation built on templated *dual numbers*, providing machine-precision gradients and enabling optimisation of functions via gradient descent


## Build

A **SCons** file is provided to build and test the software. SCons is configured to act like `cargo` in Rust: by default, binaries are built in debug mode; optimised builds use the `--release` flag.

```bash
scons                # debug build of main.cc
scons --release      # optimised build
scons test           # debug build+run of tests
scons test --release # optimised build+run of tests
```

A traditional **Makefile** is also provided in case you don't have `scons` installed. However, it isn't as versatile as SCons and will likely be removed soon.

```bash
make       # build of main.cc and tests
make test  # execution of tests
```

## API examples 

The library provides elementary functions for templated dual numbers, so you can use `sqrt`, `pow`, `exp`, `log`, `sin`, `cos`, and `tan` in templated functions that work on both double-precision and dual numbers.

The template-based design with automatic dimension deduction allows you to write your function with `auto` parameters and optimise:

```c++
constexpr double step = 1.0e-3, grad_tol = 1.0e-6;
Optimiser opt(step, grad_tol); // Optimiser<double> is deduced here

fmt::print("Running optimiser... ");
// minimise_from_zero sets starting point to zeroed n-vector
// n must be provided
auto result = opt.minimise_from_zero<2>([](auto x, auto y){ return x*x + y*y; }); // result is Result<double, 2> and contains information about the minimisation


fmt::print("done\n");
// converged or hit maximum iterations?
fmt::print("Converged: {}\n", result.converged());
// best point
fmt::print("Best point: ({}, {})\n", result.point()[0], result.point()[1]);
```

Templates also help in dimension deduction if a starting point is provided

```c++
Vector init{10.0, -5.0, 3.0}; // CTAD deduces Vector<double, 3>

Optimiser opt(1.e-3, 1.e-6);

auto res = opt.minimise([](auto x, auto y, auto z) { 
    return pow(x-1, 2.0) + pow(y-2, 2.0) + pow(z-5, 2.0); 
}, init);

fmt::print("Converged: {}\n", res.converged());
auto p = res.point(); // deduced to Vector<double, 3>
fmt::print("Best point: ({:.2f}, {:.2f}, {:.2f})\n", p[0], p[1], p[2]);
```

If you have a complex function you can't easily express as a lambda—e.g., it touches files or GPU code—you can still use Gradual and templates to find its minimum

```c++
template<typename T>
class MyModel {
public:
    T operator()(T p1, T p2, T p3, T p4, T p5) const {
        // Hundreds of lines
        // ...
    }
};


Vector init{1.0, -1.0, 278.0, -3.14, 0.0};

MyModel model;
auto res = opt.minimise([&](auto... p) { return model(p...); }, init);
```


## How does this work behind the scenes

Dual numbers are exploited to compute automatic derivatives of your function and ask the gradient *excuse me sir, how do I get to the minimum?*. The gradient is very kind and points you in the right direction.

Fortunately, users don't have to write their functions in terms of template duals. The compiler does this for them, as was shown in the examples.

### What are dual numbers

Dual numbers constitute an extension of real numbers, in a similar way as complex numbers do, and are of the form

$$a + b \hspace{0.05cm} \epsilon$$

with $\epsilon^2 = 0$ (and $\epsilon \neq 0$). With this property, addition and multiplication of dual numbers are given by

$$ (a + b \hspace{0.05cm} \epsilon) + (c + d \hspace{0.05cm} \epsilon) = (a + c) + (b + d) \hspace{0.05cm} \epsilon $$

$$ (a + b \hspace{0.05cm} \epsilon)  (c + d \hspace{0.05cm} \epsilon) = a c + (a d + b c) \hspace{0.05cm} \epsilon $$

Among many other interesting properties, dual numbers play an important role in differentiation. Given an analytic function $f$ which can be expanded in a Taylor series, it can be shown that

$$ f(a + b \hspace{0.05cm} \epsilon) = f(a) + b f^\prime(a) \hspace{0.05cm} \epsilon $$

When an analytic function acts on a dual number, the result is a dual number as well. The real part is the function value at the real part of the input, and the dual part is the derivative at that point scaled by the input's dual part. Higher-order terms vanish since $\epsilon^2 = 0$. 

This can be exploited to compute **exact** derivatives at a point $x$ by evaluating the function on the dual number $x + 1 \cdot \epsilon$ and reading off the dual part. No finite differences, no symbolic differentiation—just function calls on tiny structures. 

### Extension to N dimensions

This theory extends naturally to multivariable functions. Given $\mathbf{x}_0, \mathbf{y}_0 \in \mathbb{R}^n$, and an analytic function $f(x_1, \dots, x_n)$:

$$f(\mathbf{x}_0 + \mathbf{y}_0 \epsilon) = f(\mathbf{x}_0) + \nabla f(\mathbf{x}_0) \cdot \mathbf{y}_0 \hspace{0.05cm} \epsilon$$

To extract the $j$-th partial derivative at $\mathbf{x}_0$, $\partial_j \hspace{0.01cm} f \hspace{0.01cm} (\mathbf{x}_0)$, set $\mathbf{y}_0 = \mathbf{e}_j$ (the $j$-th standard basis vector): $\mathbf{e}_j = (0, \ldots, 1, \ldots, 0)$ with 1 at position $j$. This is an ordinary projection onto the direction $\mathbf{e}_j$.
