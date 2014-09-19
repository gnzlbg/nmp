################################################################################
# Compilation options:
################################################################################
include(CMakeDependentOption)

# Create a new option
macro(opt var name def)
  option(${var} "Enables ${name}." def)
  if(NMP_VERBOSE_CONFIGURE)
    if(${var})
      message("[O] ${name} enabled.")
    else()
      message("[O] ${name} disabled.")
    endif()
  endif()
endmacro()

# Options:
opt(NMP_VERBOSE_CONFIGURE "verbose configure" ON)
opt(NMP_WITH_ASAN "address sanitizer" ON)
opt(NMP_WITH_ASSERTIONS "assertions" ON)
opt(NMP_WITH_DEBUG "debug build" ON)
opt(NMP_WITH_DEBUG_INFO "debug information." ON)
opt(NMP_WITH_COVERAGE "coverage analysis" OFF)
opt(NMP_WITH_WARNINGS_AS_ERRORS  "warnings as errors" OFF)
