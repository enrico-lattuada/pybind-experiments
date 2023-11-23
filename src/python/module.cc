#include "bindings/foo_binding.h"

#ifdef ENABLE_GPU
#include "bindings/foo_gpu_binding.h"
#endif

PYBIND11_MODULE(_core, m)
{
    bind_foo(m);
#ifdef ENABLE_GPU
    bind_foo_gpu(m);
#endif
}
