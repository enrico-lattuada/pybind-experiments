import os
import re
import sys
import sysconfig
import platform
import subprocess
import multiprocessing

from distutils.version import LooseVersion
from setuptools import setup, find_packages, Extension
from setuptools.command.build_ext import build_ext
from setuptools.command.install_lib import install_lib


# Set package name
PACKAGE_NAME = "pybind_experiments"

# Convert distutils Windows platform specifiers to CMake -A arguments
PLAT_TO_CMAKE = {
    "win32": "Win32",
    "win-amd64": "x64",
    "win-arm32": "ARM",
    "win-arm64": "ARM64",
}


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class InstallCMakeLibs(install_lib):
    """
    Get the libraries from the parent distribution, use those as the outfiles

    Skip building anything; everything is already built, forward libraries to
    the installation step
    """

    def run(self):
        """
        Copy libraries from the bin directory and place them as appropriate
        """

        self.announce("Moving library files", level=3)

        # We have already built the libraries in the previous build_ext step
        self.skip_build = True

        # Folder where the package has been placed by CMake.
        # It is used by self.install
        self.build_dir = self.distribution.lib_dir
        self.outfiles = self.install()

        # I have copied this bit from the parent class
        if self.outfiles is not None:
            # always compile, in case we have any extension stubs to deal with
            self.byte_compile(self.outfiles)

    def get_outputs(self):
        """
        Overrides the parent class' method.
        Returns a list of the files copied over by the `run` method
        """
        return self.outfiles


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(["cmake", "--version"])
        except OSError:
            raise RuntimeError(
                "CMake must be installed to build the following extensions: "
                + ", ".join(e.name for e in self.extensions)
            )

        self.cmake_version = LooseVersion(
            re.search(r"version\s*([\d.]+)", out.decode()).group(1)
        )

        if platform.system() == "windows":
            if self.cmake_version < "3.1.0":
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext) -> None:
        self.announce("Preparing the build environment", level=3)

        cmake_args = []

        # get CMake generator
        cmake_generator = os.environ.get("CMAKE_GENERATOR", "")

        cfg = "Debug" if self.debug else "Release"
        build_args = ["--config", cfg]
        native_generator_args = ["--"]

        if self.compiler.compiler_type == "msvc":
            # Single config generators are handled "normally"
            single_config = any(x in cmake_generator for x in {"NMake", "Ninja"})

            # CMake allows an arch-in-generator style for backward compatibility
            contains_arch = any(x in cmake_generator for x in {"ARM", "Win64"})

            # Specify the arch if using MSVC generator, but only if it doesn't
            # contain a backward-compatibility arch spec already in the
            # generator name.
            if not single_config and not contains_arch:
                cmake_args += ["-A", PLAT_TO_CMAKE[self.plat_name]]
            native_generator_args += ["/m"]
        else:
            cmake_args += ["-DCMAKE_BUILD_TYPE=" + cfg]

        # Set CMAKE_BUILD_PARALLEL_LEVEL to control the parallel build level
        # across all generators
        if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
            try:
                cpu_cores = int(os.getenv("SLURM_NTASKS"))
            except:
                cpu_cores = int(multiprocessing.cpu_count() / 2)

            if self.cmake_version < "3.14.0":
                native_generator_args += [f"-j{cpu_cores}"]
            else:
                build_args += [f"-j{cpu_cores}"]

        build_args += native_generator_args

        # Create temp build directory if it does not exist
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        # Set the library distribution directory
        self.distribution.lib_dir = os.path.join(self.build_temp, "src")

        # Configure CMake project
        self.announce("Configuring CMake project", level=3)
        subprocess.check_call(
            ["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp
        )

        # Build CMake project
        self.announce("Building CMake project", level=3)
        subprocess.check_call(
            ["cmake", "--build", "."] + build_args, cwd=self.build_temp
        )

        # Compilation done
        self.announce("Compilation done", level=3)


setup(
    name=PACKAGE_NAME,
    use_scm_version={
        "fallback_version": "0.0.0",
    },
    packages=find_packages(),
    url="https://github.com/enrico-lattuada/pybind-experiments",
    long_description=open("./README.md", "r").read(),
    long_description_content_type="text/markdown",
    license="GPL-3.0-only",
    ext_modules=[CMakeExtension("pybind_experiments")],
    # Add custom commands
    cmdclass={
        "build_ext": CMakeBuild,
        "install_lib": InstallCMakeLibs,
    },
    zip_safe=False,
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "Topic :: Software Development :: Build Tools",
        "License :: OSI Approved :: GNU GPL v3",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Operating System :: OS Independent",
    ],
)
