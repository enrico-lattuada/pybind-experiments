# CMake minimum version
CMAKE_MINIMUM_REQUIRED(VERSION 3.12 FATAL_ERROR)

# Set project name
PROJECT(pybind_experiments)

# The default behavior is to build with optimization
SET(CMAKE_BUILD_TYPE Release)

# Set the macOS rpath option
IF(APPLE)
    # Enable the macOS rpath
    SET(CMAKE_MACOSX_RPATH ON)

    # Set the install rpath to the loader path
    SET(CMAKE_INSTALL_RPATH "@loader_path")

    # Build with the install rpath
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
ELSE()
    # Use the origin as the build rpath
    IF(${CMAKE_VERSION} VERSION_LESS "3.14")
        SET(CMAKE_INSTALL_RPATH "$ORIGIN")
    ELSE()
        SET(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
    ENDIF()
ENDIF(APPLE)

# The library should be in a folder with the same name to make it easier to use python distutils
SET(PYBINDEXP_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/python/pybind_experiments)

# Enable C++ language
ENABLE_LANGUAGE(CXX)

# Set C++ standard
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

# Build with python3 bindings
FIND_PACKAGE(Python3 COMPONENTS Interpreter Development REQUIRED)
ADD_SUBDIRECTORY(lib/pybind11)
INCLUDE_DIRECTORIES(${Python3_INCLUDE_DIRS})

# Add src directory
ADD_SUBDIRECTORY(src)
