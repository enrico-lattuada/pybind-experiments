# translate CMake true/false to python
if(ENABLE_GPU)
    set(IS_GPU_ENABLED True)
else()
    set(IS_GPU_ENABLED False)
endif()

# Set foo sources
set(foo_SOURCES
    cpp/foo.cc
)

# Set foo_gpu sources
set(foo_gpu_SOURCES
    cuda/foo_gpu.cc
)

# Set foo_gpu_cu sources
set(foo_gpu_cu_SOURCES
    cuda/foo_gpu.cu
)

if(ENABLE_GPU)
    set_source_files_properties(${foo_gpu_cu_SOURCES} PROPERTIES LANGUAGE CUDA)
endif(ENABLE_GPU)

# Add foo library
add_library(foo SHARED ${foo_SOURCES})
target_link_libraries(foo PUBLIC ${PYTHON_LIBRARIES})
set_target_properties(foo PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PYBINDEXP_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PYBINDEXP_OUTPUT_DIR}
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# Add suffix .so for UNIX systems
if(NOT WIN32)
    set_target_properties(foo PROPERTIES
        SUFFIX ".so"
    )
endif()

# Add _core library (with Python bindings)
pybind11_add_module(_core SHARED python/module.cc)
target_compile_definitions(_core PUBLIC ENABLE_GPU)
target_link_libraries(_core PRIVATE foo)
set_target_properties(_core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PYBINDEXP_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PYBINDEXP_OUTPUT_DIR}
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)

# Add foo_gpu library
if(ENABLE_GPU)
    include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
    include(CUDAsetup.cmake)

    add_library(foo_gpu SHARED ${foo_gpu_SOURCES} ${foo_gpu_cu_SOURCES})
    target_link_libraries(foo_gpu PUBLIC ${PYTHON_LIBRARIES})
    set_target_properties(foo_gpu PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PYBINDEXP_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PYBINDEXP_OUTPUT_DIR}
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )

    if(${CMAKE_VERSION} VERSION_LESS "3.18.0")
        target_compile_options(foo_gpu PUBLIC $<$<COMPILE_LANGUAGE:CUDA>:-arch=sm_${_cuda_min_arch})
    else()
        set_target_properties(foo_gpu PROPERTIES
            CUDA_ARCHITECTURES ${_cuda_min_arch}
        )
    endif()

    # Add suffix .so for UNIX systems
    if(NOT WIN32)
        set_target_properties(foo_gpu PROPERTIES
            SUFFIX ".so"
        )
    endif()

    # Add _core_gpu library (with Python bindings)
    pybind11_add_module(_core_gpu SHARED python/module_gpu.cc)
    target_link_libraries(_core_gpu PRIVATE foo_gpu)
    set_target_properties(_core_gpu PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PYBINDEXP_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PYBINDEXP_OUTPUT_DIR}
        PREFIX "${PYTHON_MODULE_PREFIX}"
        SUFFIX "${PYTHON_MODULE_EXTENSION}"
    )
endif(ENABLE_GPU)

# Set Python files
set(python_SOURCES
    python/__init__.py
    python/_foopy.py
)

# Configure Python files in the list and write them in the output directory
# Define the macro
macro(configure_files LIST_VARNAME OUTPUT_DIR)
    foreach(file ${${LIST_VARNAME}})
        get_filename_component(filename ${file} NAME)
        configure_file(${file} ${OUTPUT_DIR}/${filename})
    endforeach()
endmacro()

# Configure the files
configure_files(python_SOURCES ${PYBINDEXP_OUTPUT_DIR})

# Manually configure the make_install_setup.py and config.py.in
configure_file(python/make_install_setup.py
    ${PYBINDEXP_OUTPUT_DIR}/setup.py)

configure_file(python/config.py.in
    ${PYBINDEXP_OUTPUT_DIR}/config.py)

# Install the module
install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} -m pip install ${PYBINDEXP_OUTPUT_DIR})")
