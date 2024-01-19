#include "foo_gpu.h"
#include "foo_gpu.cuh"

// #include "helper_foo.h"

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

void PYBIND11_EXPORT set_device(int device_id)
{
    // Get number of available devices
    int deviceCount;
    cudaError_t status = cudaGetDeviceCount(&deviceCount);

    if (status != cudaSuccess)
    {
        throw std::runtime_error("Failed to get CUDA device count. Error: " + std::string(cudaGetErrorString(status)));
    }

    // Set device
    if (device_id < 0 || device_id >= deviceCount)
    {
        throw std::runtime_error("Invalid GPU ID provided. Valid ID range: 0 <= id < " + std::to_string(deviceCount));
    }
    else
    {
        int valid_devices[] = {device_id};
        cudaSetValidDevices(valid_devices, 1);
    }
}
