#include "mysum.h"

py::array_t<double> add_arrays(py::array_t<double, py::array::c_style> a,
                               py::array_t<double, py::array::c_style> b)
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
    py::array_t<double> result = py::array_t<double>(a_info.size);

    py::buffer_info result_info = result.request();

    // Get pointers to data
    double *a_ptr = static_cast<double *>(a_info.ptr);
    double *b_ptr = static_cast<double *>(b_info.ptr);
    double *result_ptr = static_cast<double *>(result_info.ptr);
    // the last can also be
    //      auto result_ptr = result.mutable_data();

    for (int i = 0; i < a_info.size; i++)
    {
        result_ptr[i] = a_ptr[i] + b_ptr[i];
    }

    return result;
}

/*!
    Export functions to Python
*/
void export_functions(py::module &m)
{
    m.def("add_arrays", &add_arrays, py::return_value_policy::take_ownership);
}
