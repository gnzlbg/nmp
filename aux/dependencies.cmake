################################################################################
# Dependencies
################################################################################

#  Specify dependencies
macro(dep name cflags lflags)
  find_package(${name} REQUIRED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${${name}_COMPILE_FLAGS} ${cflags}")
  set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${${name}_LINK_FLAGS} ${lflags}")
  include_directories(SYSTEM ${${name}_INCLUDE_DIR})
endmacro()

# Dependencies
dep(MPI "" "")

set(NMP_LIBS  ${MPI_LIBRARIES})

# Set compiler flags:
include(join)
join(NMP_CXX_FL " " ${NMP_CXX_FL})
join(NMP_DBG_FL " " ${NMP_DBG_FL})
join(NMP_REL_FL " " ${NMP_REL_FL})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${NMP_CXX_FL}")
set(CMAKE_CXX_FLAGS_DEBUG "${NMP_DBG_FL}")
set(CMAKE_CXX_FLAGS_RELEASE "${NMP_REL_FL}")

# Verbose CMake config output output:
if(NMP_VERBOSE_CONFIGURE)
  message("Verbose flags:")
  message("Build type: ${CMAKE_BUILD_TYPE}")
  message("The compiler is: ${CMAKE_CXX_COMPILER}")
  message("CXX:")
  message(" * Flags: ${CMAKE_CXX_FLAGS}")
  message(" * Debug Flags: ${CMAKE_CXX_FLAGS_DEBUG}")
  message(" * Release Flags: ${CMAKE_CXX_FLAGS_RELEASE}")
  message(" * Compile Flags: ${CMAKE_CXX_COMPILE_FLAGS}")
  message("C:")
  message(" * Flags: ${CMAKE_C_FLAGS}")
  message(" * Compile Flags: ${CMAKE_C_COMPILE_FLAGS}")
  message("Linker flags:")
  message(" * exe:${CMAKE_EXE_LINKER_FLAGS}")
  message(" * cxx: ${CMAKE_CXX_LINK_FLAGS}")
  message(" * module: ${CMAKE_MODULE_LINKER_FLAGS}")
  get_directory_property(CMakeCompDirDefs COMPILE_DEFINITIONS)
  message("Definitions: ${CmakeCompDirDefs}")
endif()
