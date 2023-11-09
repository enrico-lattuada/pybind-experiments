#ifndef __MYSUM_H__
#define __MYSUM_H__

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

/*! \brief Add two NumPy arrays
    \param a First array
    \param b Second array
    \return The sum of the two arrays
*/
py::array_t<double> add_arrays(py::array_t<double, py::array::c_style> a,
                               py::array_t<double, py::array::c_style> b);

/*! \brief Export functions to Python
    \param m Module
*/
void export_functions(py::module &m);

#endif // __MYSUM_H__
