#ifndef __FOO_H__
#define __FOO_H__

#include "../python_defs.h"

/*! \brief Add two numbers
    \param x First number
    \param y Second number
    \return The sum of the two numbers
*/
double PYBIND11_EXPORT add(double x, double y);

/*! \brief Add two NumPy arrays
    \param a First array
    \param b Second array
    \return The sum of the two arrays
*/
py::array_t<double> PYBIND11_EXPORT add_arrays(py::array_t<double, py::array::c_style> a,
                                               py::array_t<double, py::array::c_style> b);

#endif // __FOO_H__
