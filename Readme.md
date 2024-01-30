# Arbitrary Precision Number Implementation

This project is a C++ class implementation of ___BigInteger___ and ___BigRational___. It's designed not only as a numerical tool but also as a resource for `profiling`, `testing`, and `fuzzing` your own implementations.

## Prerequisites
- CMake version 3.14 or higher.
- Clang compiler (or modify the `CMakeLists.txt` if using a different compiler).

## Getting Started
Clone the repository and navigate to the project directory. Initialize the project using CMake:
```bash
cmake .
```

## Features
- **BigInteger Storage**: ___BigInteger___ is stored as a vector of `digits`. The digit size is configurable from `uint8_t` to `uint32_t`.
- **BigRational Representation**: ___BigRational___ is composed of two ___BigIntegers___, a numerator and a denominator.
- **Safety**: The implementation handles overflows/underflows, and casts are performed statically.
- **Testing**: Basic operations and edge cases are covered by tests. Run tests in release mode:
  ```bash
  make run
  ```
  For debug mode:
  ```bash
  make debug
  ```

- **Fuzz Testing**: The code has passed basic tests (e.g., a + a = a * 2) with over 200,000 inputs from fuzzing when compiled with UB and Address sanitizers. Run fuzz tests for both string and int64_t constructors:
  ```bash
  make fuzz
  ```
  This process is resource-intensive and may take some time (~20 min). For separate runs (~10 min each):
  ```bash
  make fuzz_str_addr_ub_finite
  make fuzz_int_addr_ub_finite
  ```

- **Profiling**: Profile the execution to analyze performance:
  ```bash
  make profile
  ```
  My average test execution time is between 0.5s-0.8s in release mode and 2.0-2.3s in debug mode.

- **Implementation Details**: Basic operators (`*`, `+`, `/`, `-`) are implemented with a focus on simplicity, incorporating optimizations where feasible.

## Acknowledgments
Inspiration for the project was drawn from the V8 JavaScript engine's bigint implementation, which can be found at [V8 GitHub repository](https://github.com/v8/v8/tree/main/src/bigint).
