################################################################################
# Builds the website (target: "make site")
#
# It builds on:
#  - "make fmt" formats the source code
#  - "make doc" generates doxygen documentation
#  - "make cov" generates code coverage information (performs analysis)
################################################################################

# Doxygen is required
find_package(Doxygen)
if (NOT DOXYGEN_FOUND)
  message(STATUS "Doxygen not found; 'site' target unavailable.")
  return()
endif()

# Generate API documentation
configure_file(${PROJECT_SOURCE_DIR}/site/doxygen/Doxyfile.in
               ${PROJECT_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)
add_custom_target(doc
  COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
  COMMENT "Generating API documentation with Doxygen"
  VERBATIM
  DEPENDS fmt
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# Generates the website site
add_custom_target(site
  COMMAND ${PROJECT_SOURCE_DIR}/aux/generate_site.py ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}
  COMMENT "Generating the website"
  VERBATIM
  DEPENDS doc
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
