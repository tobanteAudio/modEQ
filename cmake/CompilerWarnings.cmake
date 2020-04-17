add_library(compiler_warnings INTERFACE)
add_library(tobanteAudio::CompilerWarnings ALIAS compiler_warnings)

# MSVC
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(compiler_warnings INTERFACE /W3)
endif()

# GCCish
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  target_compile_options(compiler_warnings INTERFACE -Wall -Wextra -Wpedantic -Wshadow)
endif()

# Clang
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  target_compile_options(compiler_warnings INTERFACE -Wshadow-all -Wno-gnu-zero-variadic-macro-arguments)
endif()

# GCC
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(compiler_warnings INTERFACE 
    -Wmisleading-indentation 
    -Wlogical-op 
    -Wduplicated-branches 
    -Wduplicated-cond 
    -Wno-parentheses
    -Wno-sequence-point
  )
endif()