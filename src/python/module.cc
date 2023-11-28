#include "bindings/foo_binding.h"

PYBIND11_MODULE(_core, m)
{
    bind_foo(m);
}
