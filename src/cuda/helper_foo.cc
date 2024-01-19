#include "helper_foo.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <nvml.h>

void get_host_free_memory(unsigned long long &free_bytes)
{
#ifdef WIN32
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    free_bytes = statex.ullAvailPhys;
#else
    struct sysinfo info;
    sysinfo(&info);
    free_bytes = info.freeram;
#endif
}

void get_device_free_memory(unsigned long long &free_bytes)
{
    // get device memory
    nvmlInit_v2();
    nvmlDevice_t dev;
    nvmlReturn_t res = nvmlDeviceGetHandleByIndex_v2(0, &dev);
    nvmlMemory_t mem;
    nvmlDeviceGetMemoryInfo(dev, &mem);
    nvmlShutdown();

    free_bytes = mem.free;
}

void check_host_memory(vector<py::ssize_t> shape)
{
    unsigned long long free_bytes;
    get_host_free_memory(free_bytes);

    // Compute size of fft
    py::ssize_t dim_x = shape[2] / 2 + 1;

    if (free_bytes < 8ULL * 2 * dim_x * shape[1] * shape[0])
    {
        throw std::runtime_error("Not enough host memory");
    }
}

void check_device_memory(vector<py::ssize_t> shape)
{
    unsigned long long free_bytes;
    get_device_free_memory(free_bytes);
}
