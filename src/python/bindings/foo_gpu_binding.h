#ifndef __FOO_GPU_BINDING_H__
#define __FOO_GPU_BINDING_H__

#include "../../cuda/foo_gpu.h"

#include "../../python_defs.h"

void bind_foo_gpu(py::module &m)
{
    m.def("add_gpu", &add_gpu);
    m.def("add_arrays_gpu", &add_arrays_gpu<double>);
    m.def("add_arrays_gpu", &add_arrays_gpu<int64_t>);
    m.def("set_device", &set_device);
}

#endif // __FOO_GPU_BINDING_H__
