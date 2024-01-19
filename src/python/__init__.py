from ._foopy import add, add_arrays  # noqa: F401
from ._core import add as add_cpp, add_arrays as add_arrays_cpp  # noqa: F401

from . import config

if config.IS_GPU_ENABLED:
    from ._core_gpu import add_gpu, add_arrays_gpu  # noqa: F401
    from ._core_gpu import set_device  # noqa: F401
