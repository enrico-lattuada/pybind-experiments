#ifndef __FOO_GPU_H__
#define __FOO_GPU_H__

#include "../python_defs.h"

/*! \brief Add two numbers on the GPU
    \param x First number
    \param y Second number
    \return The sum of the two numbers
*/
double PYBIND11_EXPORT add_gpu(const double &x, const double &y);

/*! \brief Add two NumPy arrays on the GPU
    \param a First array
    \param b Second array
    \return The sum of the two arrays
*/
template <typename T>
py::array_t<T> PYBIND11_EXPORT add_arrays_gpu(const py::array_t<T, py::array::c_style> a,
                                              const py::array_t<T, py::array::c_style> b);

/*! \brief Set the device to be used
    \param device_id ID of the device to use
*/
void PYBIND11_EXPORT set_device(int device_id);

#endif // __FOO_GPU_H__
