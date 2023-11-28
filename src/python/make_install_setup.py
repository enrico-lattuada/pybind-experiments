from setuptools import setup

import sys

if sys.version_info < (3, 6):
    sys.exit("Error, Python < 3.6 is not supported.")

setup(
    name="pybind_experiments",
    use_scm_version={
        "root": "${CMAKE_SOURCE_DIR}",
        "fallback_version": "0.3.0",
    },
    setup_requires=["setuptools_scm"],
    packages=["pybind_experiments"],
    package_dir={"pybind_experiments": "${PYBINDEXP_OUTPUT_DIR}"},
    package_data={"pybind_experiments": ["_core.*", "_core_gpu.*"]},
)
