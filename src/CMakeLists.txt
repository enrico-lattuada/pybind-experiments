# Set foo sources
set(foo_SOURCES
    cpp/foo.cc
)

# Add foo library
add_library(foo SHARED ${foo_SOURCES})
target_link_libraries(foo PUBLIC ${Python3_LIBRARIES})
set_target_properties(foo PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PYBINDEXP_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PYBINDEXP_OUTPUT_DIR}
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# Add suffix .so for UNIX systems
IF(NOT WIN32)
    SET_TARGET_PROPERTIES(foo PROPERTIES
        SUFFIX ".so"
    )
ENDIF()

# Add _core library (with Python bindings)
pybind11_add_module(_core SHARED python/module.cc)
target_link_libraries(_core PRIVATE foo)
set_target_properties(_core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PYBINDEXP_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PYBINDEXP_OUTPUT_DIR}
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)

IF(WIN32)
    set_target_properties(_core PROPERTIES
        SUFFIX ".pyd"
    )
ENDIF()

# Add suffix .so for UNIX systems
# IF(NOT WIN32)
# SET_TARGET_PROPERTIES(_core PROPERTIES
# SUFFIX ".so"
# )
# ENDIF()

# Set Python files
SET(python_SOURCES
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

# Manually configure the make_install_setup.py
configure_file(python/make_install_setup.py
    ${PYBINDEXP_OUTPUT_DIR}/setup.py)

# Install the module
install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} -m pip install ${PYBINDEXP_OUTPUT_DIR})")
