cmake_minimum_required(VERSION 3.12)

OPTION(ENABLE_GPU "Set to ON to compile with CUDA support" OFF)

project(pybind_experiments)

# The default behavior is to build in Release mode
set(CMAKE_BUILD_TYPE Release)

if(APPLE)
    # Enable the macOS rpath
    SET(CMAKE_MACOSX_RPATH ON)

    # Set the install rpath to the loader path
    SET(CMAKE_INSTALL_RPATH "@loader_path")

    # Build with the install rpath
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
else()
    # Use ORIGIN as the build rpath
    IF(${CMAKE_VERSION} VERSION_LESS "3.14")
        SET(CMAKE_INSTALL_RPATH "$ORIGIN")
    ELSE()
        SET(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
    ENDIF()
endif()

# Set output directory
set(PYBINDEXP_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/pybind_experiments)

enable_language(CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PYBIND11_PYTHON_VERSION 3 CACHE STRING "Python version to use for compiling modules")
add_subdirectory(lib/pybind11)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/lib/pybind11/include)

# Build with CUDA
if(ENABLE_GPU)
    enable_language(CUDA)

    # Set CUDA C++ standard
    set(CMAKE_CUDA_STANDARD 11)
endif(ENABLE_GPU)

add_subdirectory(src)
