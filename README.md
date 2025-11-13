# Kalman 1D – Modern C++ Implementation

## Overview

Implementation of a one-dimensional Kalman filter using C++20, designed as a reproducible example for signal processing and telemetry scenarios. The project leverages modern C++ practices and industrial tooling (CMake, GoogleTest, clang-tidy/format) to provide robust architecture, numerical precision, and extensibility.

## Motivation

Estimating states (position, velocity) from noisy measurements is critical in fields like embedded systems, robotics, and control. This repository demonstrates:
- Reproducible experimentation with synthetic, customizable signals
- Best practices for C++ project layout, build, tests, and benchmarks
- A foundation for future extensions (higher-dimensional filters, real sensors, microcontrollers)

## Features

- Minimal, readable Kalman 1D implementation (header and source)
- Synthetic data generator: position/velocity signals with Gaussian noise
- Unit tests (GoogleTest) and example demo program
- CMake-based build system (cross-platform)
- Linting/formatting (clang-format, clang-tidy)
- Ready for benchmarking and profiling

## Directory Structure
```
kalman1d-cpp/
├── CMakeLists.txt
├── README.md
├── src/
│ ├── main.cpp # Demo entry point
| |── data_generator.cpp # data generation implementation
| |—— data_generator.hpp # data generation interface
│ ├── kalman.cpp # Kalman filter implementation
│ └── kalman.hpp # Kalman filter interface
├── tests/
│ └── test_kalman.cpp # Unit tests (GoogleTest)
├── .gitignore
```


## Getting Started

### Requirements

- GCC 10+ or Clang 11+ with C++20 support
- CMake 3.15+
- GoogleTest 
- Recommended: clang-format, clang-tidy, valgrind (optional)

### Build & Test (Linux/WSL)

```
git clone https://github.com/Nero25-3/Kalman_1D.git
cd Kalman_1D
mkdir build && cd build
cmake ..
make
./kalman1d_demo # runs the demo
./kalman1d_tests # runs unit tests
```

## Features
### Synthetic Data Generation

Generate synthetic position/velocity pairs with Gaussian noise for configurable length, time step, initial value and velocity. Export results as CSV for further analysis.

Basic usage:
auto data = generate_noisy_motion(100, 0.1, 0.0, 1.5, 0.3);
write_to_csv("my_output.csv", data);

Output columns: time, position, velocity (all in SI units).


## Development Roadmap

See the [project board](https://github.com/users/Nero25-3/projects/3/views/1) for tasks and progress.
- [x] Project skeleton, CMake and minimal test
- [x] Synthetic data generator
- [x] Core Kalman class & unit tests
- [x] Performance benchmarking
- [ ] Documentation and demos

## Contributing

PRs and suggestions welcome!  
Please create feature branches per major change and submit pull requests referencing the relevant board issue.

## License

MIT 


