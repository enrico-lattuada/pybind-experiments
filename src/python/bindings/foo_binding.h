#ifndef __FOO_BINDING_H__
#define __FOO_BINDING_H__

#include "../../cpp/foo.h"

#include "../../python_defs.h"

void bind_foo(py::module &m)
{
    m.def("add", &add);
    m.def("add_arrays", &add_arrays<double>);
    m.def("add_arrays", &add_arrays<int64_t>);
}

#endif // __FOO_BINDING_H__
