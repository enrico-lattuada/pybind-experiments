# Set C++ source files
SET(cpp_SOURCES
  cpp/mysum.cc
)

# Include external libraries
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/pybind11/include)

# Add C++ library
ADD_LIBRARY(mysum SHARED ${cpp_SOURCES})
TARGET_LINK_LIBRARIES(mysum PUBLIC ${Python3_LIBRARIES})
SET_TARGET_PROPERTIES(mysum PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${PYBINDEXP_OUTPUT_DIR}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PYBINDEXP_OUTPUT_DIR}
  WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# Add suffix .so for UNIX systems
IF(NOT WIN32)
  SET_TARGET_PROPERTIES(mysum PROPERTIES
    PREFIX ""
    SUFFIX ".so"
  )
ENDIF()

# Add bindings
pybind11_add_module(_core SHARED python/bindings.cc)
TARGET_LINK_LIBRARIES(_core PRIVATE mysum)
SET_TARGET_PROPERTIES(_core PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${PYBINDEXP_OUTPUT_DIR}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PYBINDEXP_OUTPUT_DIR}
)

# Add suffix .so for UNIX systems
IF(NOT WIN32)
  SET_TARGET_PROPERTIES(_core PROPERTIES
    SUFFIX ".so"
  )
ENDIF()

# Set Python files
SET(python_SOURCES
  python/__init__.py
  python/_mysum.py
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
