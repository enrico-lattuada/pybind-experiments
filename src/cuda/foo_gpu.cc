#include "foo_gpu.h"
#include "foo_gpu.cuh"

double add_gpu(const double &x, const double &y)
{
    // Declare the output variable
    double result;

    // Add the two numbers on the GPU
    add_on_device(&x, &y, &result);

    // Return the result
    return result;
}

template <typename T>
py::array_t<T> add_arrays_gpu(const py::array_t<T, py::array::c_style> a,
                              const py::array_t<T, py::array::c_style> b)
{
    py::buffer_info a_info = a.request();
    py::buffer_info b_info = b.request();

    // Check that the two arrays are 1D
    if (a_info.ndim != 1 || b_info.ndim != 1) // can also be a.ndim() != 1
    {
        throw std::runtime_error("Number of dimensions must be one");
    }

    // Check that the two arrays have the same length
    if (a_info.size != b_info.size) // can also be a.size() != b.size()
    {
        throw std::runtime_error("Arrays must have the same length");
    }

    /* No pointer is passed, so NumPy will allocate the buffer */
    py::array_t<T> result = py::array_t<T>(a_info.size);

    py::buffer_info result_info = result.request();

    // Get pointers to data
    T *a_ptr = static_cast<T *>(a_info.ptr);
    T *b_ptr = static_cast<T *>(b_info.ptr);
    T *result_ptr = static_cast<T *>(result_info.ptr);
    // the last can also be
    //      auto result_ptr = result.mutable_data();

    // Add the two arrays on the GPU
    add_arrays_on_device(a_ptr, b_ptr, a_info.size, result_ptr);

    return result;
}
template py::array_t<double> PYBIND11_EXPORT add_arrays_gpu(const py::array_t<double, py::array::c_style> a,
                                                            const py::array_t<double, py::array::c_style> b);
template py::array_t<int64_t> PYBIND11_EXPORT add_arrays_gpu(const py::array_t<int64_t, py::array::c_style> a,
                                                             const py::array_t<int64_t, py::array::c_style> b);
