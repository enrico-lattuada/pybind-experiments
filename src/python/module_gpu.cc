#include "bindings/foo_gpu_binding.h"

PYBIND11_MODULE(_core_gpu, m)
{
    bind_foo_gpu(m);
}
