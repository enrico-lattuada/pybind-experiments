#include "../cpp/mysum.h"

/*! \brief Export module with pybind11
 */
PYBIND11_MODULE(_core, m)
{
    export_functions(m);
}
