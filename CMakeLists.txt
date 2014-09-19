################################################################################
# NMP - Non-blocking Message Passing
################################################################################
cmake_minimum_required (VERSION 2.8.7)
project(NMP CXX)

# Directories:
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/aux)

# Set up Hom3
include(options)
include(flags)
include(dependencies)
include(format)
include(coverage)

# Subdirectories
include_directories(include)
add_subdirectory(tests)
add_subdirectory(site)

# Create compilation data-base:
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
