#ifndef __FOO_BINDING_H__
#define __FOO_BINDING_H__

#include "../../cpp/foo.h"

#include "../../python_defs.h"

void bind_foo(py::module &m)
{
    m.def("add", &add);
}

#endif // __FOO_BINDING_H__
