################################################################################
# Compilation flags
################################################################################

# Build type:
if(NMP_WITH_DEBUG)
  set(CMAKE_BUILD_TYPE "Debug")
else()
  set(CMAKE_BUILD_TYPE "Release")
endif()

# Compiler flags:
set(NMP_CXX_FL "-std=c++1y" "-Wall" "-Wextra" "-Wdocumentation" "-pedantic"
  "-Woverloaded-virtual" "-Wcast-align" "-Wcomment" "-Wcast-qual" "-Wshadow"
  "-Wchar-subscripts" "-Wdisabled-optimization" "-Wfloat-equal" "-Wformat=2"
  "-Winvalid-pch" "-Wformat-nonliteral" "-Wformat-security" "-Wformat-y2k"
  "-Wimport" "-Winit-self" "-Winline" "-Wreturn-type" "-Wmissing-braces"
  "-Wmissing-field-initializers" "-Wmissing-include-dirs" "-Wredundant-decls"
  "-Wpacked" "-Wparentheses" "-Wpointer-arith" "-Wsequence-point"
  "-Wsign-compare" "-Wstack-protector" "-Wstrict-aliasing=2" "-Wswitch"
  "-Wswitch-default" "-Wtrigraphs" "-Wuninitialized" "-Wunknown-pragmas"
  "-Wunreachable-code" "-Wunused" "-Wunused-function" "-Wunused-label"
  "-Wunused-parameter" "-Wunused-value" "-Wunused-variable"
  "-Wvariadic-macros" "-Wvolatile-register-var" "-Wwrite-strings"
  "-Woverloaded-virtual" "-Wsign-promo" "-Wstrict-overflow=5"
  "-fdiagnostics-show-template-tree" "-ftemplate-backtrace-limit=0"
  "-Wno-attributes" "-Wdeprecated" "-Wunneeded-internal-declaration"
  "-D__extern_always_inline=inline")

if (NMP_WITH_WARNINGS_AS_ERRORS)
  list(APPEND NMP_CXX_FL "-Werror" "-pedantic-errors")
endif()

if (NOT NMP_WITH_ASSERTIONS)
  list(APPEND NMP_CXX_FL "-DNDEBUG -DHOM3_DISABLE_ASSERTIONS -DEIGEN_NO_DEBUG")
endif()

if (NMP_WITH_ASAN)
  list(APPEND NMP_CXX_FL "-fsanitize=address,integer,undefined"
    "-fno-omit-frame-pointer" "-fno-sanitize=unsigned-integer-overflow")
endif()

if (NMP_WITH_COVERAGE)
  if(NOT NMP_WITH_DEBUG_INFO)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
      message("[E] Coverage analysis requires debug information.")
      exit()
    endif()
  endif()
  list(APPEND NMP_CXX_FL "--coverage" "-Xclang -coverage-cfg-checksum"
    "-Xclang -coverage-no-function-names-in-data"
    "-Xclang -coverage-version='407*'")
endif()

#  Debug optimization flags:
set(NMP_DBG_FL "-O0" "-fno-inline" "-fstack-protector-all")
set(NMP_REL_FL "-Ofast" "-march=native" "-mtune=native"
  "-fstrict-aliasing" "-pipe" "-fdata-sections" "-ffunction-sections"
  "-fvectorize" "-fslp-vectorize-aggressive")
if(NMP_WITH_DEBUG_INFO)
  list(APPEND NMP_CXX_FL "-g3")
else()
  list(APPEND NMP_CXX_FL "-g0")
endif()
if (NOT NMP_WITH_ASAN)
  list(APPEND NMP_CXX_FL "-fomit-frame-pointer")
endif()
