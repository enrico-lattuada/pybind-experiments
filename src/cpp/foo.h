#ifndef __FOO_H__
#define __FOO_H__

#include "../python_defs.h"

/*! \brief Add two numbers
    \param x First number
    \param y Second number
    \return The sum of the two numbers
*/
double PYBIND11_EXPORT add(const double &x, const double &y);

/*! \brief Add two NumPy arrays
    \param a First array
    \param b Second array
    \return The sum of the two arrays
*/
template <typename T>
py::array_t<T> PYBIND11_EXPORT add_arrays(const py::array_t<T, py::array::c_style> a,
                                          const py::array_t<T, py::array::c_style> b);

#endif // __FOO_H__
