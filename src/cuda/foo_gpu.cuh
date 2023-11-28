#ifndef __FOO_GPU_CUH__
#define __FOO_GPU_CUH__

#include <cstdint>
#include <cuda.h>
#include <cuda_runtime.h>

/*! \brief Add two numbers on device
    \param a First number
    \param b Second number
    \param c The sum of the two numbers
*/
void add_on_device(const double *a, const double *b, double *c);

/*! \brief Add two numbers on device (kernel)
    \param a First number
    \param b Second number
    \param c The sum of the two numbers
*/
__global__ void add_kernel(double *a, double *b, double *c);

/*! \brief Add two arrays on device
    \param a First array
    \param b Second array
    \param n Size of the arrays
    \param c The sum of the two arrays
*/
template <typename T>
void add_arrays_on_device(const T *a, const T *b, int n, T *c);

/*! \brief Add two arrays on device (kernel)
    \param a First array
    \param b Second array
    \param n Size of the arrays
    \param c The sum of the two arrays
*/
template <typename T>
__global__ void add_arrays_kernel(const T *a, const T *b, int n, T *c);

#endif // __FOO_GPU_CUH__
