cmake_minimum_required(VERSION 3.15)
project(tobanteAudiomodEQ VERSION 0.4.0)

# Only do these if this is the main project,
# and not if it is included through add_subdirectory.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to conform to")
    set(CMAKE_CXX_STANDARD_REQUIRED YES)
    set(CMAKE_CXX_EXTENSIONS NO)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

     include(CheckCXXCompilerFlag)
     check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
     if(COMPILER_SUPPORTS_MARCH_NATIVE)
         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
     endif()

     if(MSVC)
         CHECK_CXX_COMPILER_FLAG("/arch:AVX2" COMPILER_OPT_ARCH_AVX2_SUPPORTED)
         if(COMPILER_OPT_ARCH_AVX2_SUPPORTED)
             set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
         endif()
     endif()

#    if(NOT APPLE)
#        include(CheckIPOSupported)
#        check_ipo_supported(RESULT TOBANTEAUDIO_BUILD_LTO)
#        if(TOBANTEAUDIO_BUILD_LTO)
#            message(STATUS "tobanteAudio: LTO is available.")
#            set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
#        endif()
#    endif()
endif()

option(TOBANTEAUDIO_BUILD_TESTS          "Build the unit tests with Catch2"  OFF)
option(TOBANTEAUDIO_BUILD_COVERAGE       "Build with coverage enabled"       OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CodeCoverage)
include(CompilerOptions)
include(CompilerWarnings)

# JUCE
add_subdirectory(3rd_party/JUCE EXCLUDE_FROM_ALL)
juce_add_module(${CMAKE_SOURCE_DIR}/3rd_party/ff_meters)
# juce_set_vst2_sdk_path(...)
# juce_set_aax_sdk_path(...)

# Company settings
set_directory_properties(PROPERTIES JUCE_COMPANY_COPYRIGHT "GNU GENERAL PUBLIC LICENSE Version 3")
set_directory_properties(PROPERTIES JUCE_COMPANY_NAME "tobanteAudio")
set_directory_properties(PROPERTIES JUCE_COMPANY_WEBSITE "https://github.com/tobanteAudio/modEQ")
set_directory_properties(PROPERTIES JUCE_COMPANY_EMAIL "tobanteAudio@gmail.com")

# Plugin
add_subdirectory(Source)

# Tests
#if(TOBANTEAUDIO_BUILD_TESTS)
#    list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/Catch2/contrib")
#    enable_testing()
#    add_subdirectory(3rd_party/Catch2 EXCLUDE_FROM_ALL)
#    include(CTest)
#    include(Catch)
#    add_subdirectory(tests)
#endif()