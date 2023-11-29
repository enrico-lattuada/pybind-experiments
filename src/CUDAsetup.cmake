# setup nvcc to build for all CUDA architectures. Allow user to modify the list if desired
if (CMAKE_CUDA_COMPILER_VERSION VERSION_GREATER_EQUAL 11.0)
    set(CUDA_ARCH_LIST 60 70 80 CACHE STRING "List of target sm_ architectures to compile CUDA code for. Separate with semicolons.")
elseif (CMAKE_CUDA_COMPILER_VERSION VERSION_GREATER_EQUAL 9.0)
    set(CUDA_ARCH_LIST 60 70 CACHE STRING "List of target sm_ architectures to compile CUDA code for. Separate with semicolons.")
elseif (CMAKE_CUDA_COMPILER_VERSION VERSION_GREATER_EQUAL 8.0)
    set(CUDA_ARCH_LIST 60 CACHE STRING "List of target sm_ architectures to compile CUDA code for. Separate with semicolons.")
endif()

# need to know the minimum supported CUDA_ARCH
set(_cuda_arch_list_sorted ${CUDA_ARCH_LIST})
list(SORT _cuda_arch_list_sorted)
list(GET _cuda_arch_list_sorted 0 _cuda_min_arch)
list(GET _cuda_arch_list_sorted -1 _cuda_max_arch)

if (_cuda_min_arch LESS 60)
    message(SEND_ERROR "FastDDM requires compute 6.0 or newer")
endif()

# only generate ptx code for the maximum supported CUDA_ARCH (saves on file size)
list(REVERSE _cuda_arch_list_sorted)
list(GET _cuda_arch_list_sorted 0 _cuda_max_arch)

if (${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.18)
    # CMAKE 3.18 handles CUDA ARCHITECTURES with CMAKE_CUDA_ARCHITECTURES
    set(CMAKE_CUDA_ARCHITECTURES "")
    foreach(_cuda_arch ${CUDA_ARCH_LIST})
        list(APPEND CMAKE_CUDA_ARCHITECTURES "${_cuda_arch}-real")
    endforeach()
    list(APPEND CMAKE_CUDA_ARCHITECTURES "${_cuda_max_arch}-virtual")
else()
    foreach(_cuda_arch ${CUDA_ARCH_LIST})
        set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode=arch=compute_${_cuda_arch},code=sm_${_cuda_arch}")
    endforeach()
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode=arch=compute_${_cuda_max_arch},code=compute_${_cuda_max_arch}")
endif()
