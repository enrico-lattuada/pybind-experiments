#ifndef __HELPER_FOO_H__
#define __HELPER_FOO_H__

#include "../python_defs.h"

#include <vector>

#include <cufft.h>

using namespace std;

void get_host_free_memory(unsigned long long &free_bytes);

void get_device_free_memory(unsigned long long &free_bytes);

void check_host_memory(vector<py::ssize_t> shape);

void check_device_memory(vector<py::ssize_t> shape);

#endif // __HELPER_FOO_H__
