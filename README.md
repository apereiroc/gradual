# Gradual 

**Gradual** (gradients and duals) is a software for optimisation of functions, via automatic differentiation techniques.

## Compile

A SCons file is provided to build/test the software. The SCons is constructed to act like `cargo` in Rust, so by default binaries are constructed in debug mode; optimised software can be built with `--release` flag

```bash
scons                # normal build of main.cc
scons --release      # optimised build
scons test           # normal build+run of tests
scons test --release # optimised build+run of tests in
```
