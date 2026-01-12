# Gradual 

**Gradual** (gradients and duals) is a minimalist C++20 automatic differentiation library prioritising clarity and modern design over raw performance.

Currently, it supports forward-mode differentiation built around templated *dual numbers*, providing machine-precision gradients and enabling optimisation of functions via gradient descent.


## Build

A SCons file is provided to build and test the software. SCons is configured to act like `cargo` in Rust: by default, binaries are built in debug mode; optimised builds are created with the `--release` flag.

```bash
scons                # debug build of main.cc
scons --release      # optimised build
scons test           # debug build+run of tests
scons test --release # optimised build+run of tests
```

### What are dual numbers

Dual numbers constitute an extension of real numbers, in a similar way as complex numbers do, and are of the form

$$a + b \epsilon$$

with $\epsilon^2 = 0$ (and $\epsilon \neq 0$). With this property, addition and multiplication of dual numbers are given by

$$ (a + b \epsilon) + (c + d \epsilon) = (a + c) + (b + d) \epsilon $$

$$ (a + b \epsilon)  (c + d \epsilon) = a c + (a d + b c) \epsilon $$

Among many other interesting properties, dual numbers play an important role in differentiation. Given an analytic function $f$ which can be expanded in a Taylor series, it can be shown that

$$ f(a + b \epsilon) = f(a) + b f^\prime(a) \epsilon $$

When an analytic function acts on a dual number, the result is a dual number as well. The real part is the function value at the real part of the input, and the dual part is the derivative at that point scaled by the input's dual part. Higher-order terms vanish since $\epsilon^2 = 0$. 

This can be exploited to compute **exact** derivatives at a point $x$ by evaluating the function on the dual number $x + 1 \cdot \epsilon$ and reading off the dual part. No finite differences, no symbolic differentiation—just function calls on tiny structures. 

### Extension to N dimensions

This theory extends naturally to multivariable functions. Given $\mathbf{x}_0, \mathbf{y}_0 \in \mathbb{R}^n$, and an analytic function $f(x_1, \dots, x_n)$:

$$f(\mathbf{x}_0 + \mathbf{y}_0 \epsilon) = f(\mathbf{x}_0) + \nabla f(\mathbf{x}_0) \cdot \mathbf{y}_0 \, \epsilon$$

To extract the $j$-th partial derivative at $\mathbf{x}_0$, $\nabla_j f (\mathbf{x}_0)$, set $\mathbf{y}_0 = \mathbf{e}_j$ (the $j$-th standard basis vector): $\mathbf{e}_j = (0, \ldots, 1, \ldots, 0)$ with 1 at position $j$.
