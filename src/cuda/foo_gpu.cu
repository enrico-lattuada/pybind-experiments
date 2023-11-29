#include "foo_gpu.cuh"

__global__ void add_kernel(double *a, double *b, double *c)
{
    *c = *a + *b;
}

void add_on_device(const double *a, const double *b, double *c)
{
    // Declare device pointers
    double *d_a, *d_b, *d_c;

    // Allocate device memory
    cudaMalloc((void **)&d_a, sizeof(double));
    cudaMalloc((void **)&d_b, sizeof(double));
    cudaMalloc((void **)&d_c, sizeof(double));

    // Copy data from host to device
    cudaMemcpy(d_a, a, sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, sizeof(double), cudaMemcpyHostToDevice);

    // Launch kernel
    add_kernel<<<1, 1>>>(d_a, d_b, d_c);

    // Copy data from device to host
    cudaMemcpy(c, d_c, sizeof(double), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
}

template <typename T>
__global__ void add_arrays_kernel(T *a, T *b, int n, T *c)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
    {
        c[i] = a[i] + b[i];
    }
}

template __global__ void add_arrays_kernel<double>(double *a, double *b, int n, double *c);
template __global__ void add_arrays_kernel<int64_t>(int64_t *a, int64_t *b, int n, int64_t *c);

template <typename T>
void add_arrays_on_device(const T *a, const T *b, int n, T *c)
{
    // Declare device pointers
    T *d_a, *d_b, *d_c;

    // Allocate device memory
    cudaMalloc((void **)&d_a, n * sizeof(T));
    cudaMalloc((void **)&d_b, n * sizeof(T));
    cudaMalloc((void **)&d_c, n * sizeof(T));

    // Copy data from host to device
    cudaMemcpy(d_a, a, n * sizeof(T), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, n * sizeof(T), cudaMemcpyHostToDevice);

    // Launch kernel
    int block_size = 256;
    int num_blocks = (n + block_size - 1) / block_size;
    add_arrays_kernel<<<num_blocks, block_size>>>(d_a, d_b, n, d_c);

    // Copy data from device to host
    cudaMemcpy(c, d_c, n * sizeof(T), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
}

template void add_arrays_on_device<double>(const double *a, const double *b, int n, double *c);
template void add_arrays_on_device<int64_t>(const int64_t *a, const int64_t *b, int n, int64_t *c);
