# pybind-experiments

[![GitHub Actions](https://github.com/enrico-lattuada/pybind-experiments/actions/workflows/test.yml/badge.svg?branch=main)](https://github.com/enrico-lattuada/pybind-experiments/actions/workflows/test.yml)
[![GitHub Actions](https://github.com/enrico-lattuada/pybind-experiments/actions/workflows/test.yml/badge.svg?branch=main)](https://github.com/enrico-lattuada/pybind-experiments/actions/workflows/test_gpu.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)
![Python](https://img.shields.io/badge/python-3.8%20%7C%203.11-blue)

Test repository to experiment with pybind11, C++, and CUDA.

## setup.py

The library distribution directory should mirror the project name set in top-level CMakeLists.txt,
as well as the `OUTPUT_DIR` set in the same file (minus the package name).

```cmake
project(pybind_experiments)

...

set(PYBINDEXP_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/pybind_experiments)
```

## src/CMakeLists.txt

Set the C++ files as in `foo_SOURCES`. These are then compiled into a shared library `foo`.

Set the python files to be configured as in `python_SOURCES`.

## C++ files

The functions that need to be exported, must have `PYBIND11_EXPORT` in their definition.
For templated functions, the same flag must be present also in the specialization.
This is to override the `-fvisibility=hidden` setting used by pybind11 and is only needed
to compile shared libraries separately from the pybind module.

## Binders

Add the definitions to `src/python/bindings/foo_binding.h`.
These are then added to the pybind module in `src/python/module.cc`.
