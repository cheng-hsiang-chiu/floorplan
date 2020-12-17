# CMake version
cmake_minimum_required (VERSION 3.10)
MESSAGE(STATUS "CMAKE_ROOT: " ${CMAKE_ROOT})

# Project name
project(Floorplan VERSION 1.0.0 LANGUAGES CXX)

# build options
option(FP_BUILD_TESTS "Enables builds of tests" ON)
option(FP_BUILD_MAIN "Enables builds of main programs" ON)

# project-specific variables
set(FP_INC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(FP_LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(FP_UTEST_DIR ${PROJECT_SOURCE_DIR}/unittests)
set(FP_3RD_PARTY_DIR ${PROJECT_SOURCE_DIR}/3rd-party)
set(FP_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(FP_MAIN_DIR ${PROJECT_SOURCE_DIR}/main)
set(FP_DEFAULT_BUILD_TYPE "Release")

# Turn on the verbose
set(CMAKE_VERBOSE_MAKEFILE ON)

# error setting
add_library(error_settings INTERFACE)

target_compile_options(
  error_settings
  INTERFACE
  $<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -Wfatal-errors>
  $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Wfatal-errors>
  $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -Wfatal-errors>
  $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:MSVC>>:/W3 /permissive->
  #$<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler=-Wall,-Wextra,-Wfatal-errors>
  #$<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wfatal-errors>
  #$<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wfatal-errors>
  #$<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive->
  #$<$<CXX_COMPILER_ID:NVIDIA>:-Xcompiler=-Wall,-Wextra,-Wfatal-errors>
)


# -----------------------------------------------------------------------------
# Floorplan Libraries
# -----------------------------------------------------------------------------
add_subdirectory(${FP_SRC_DIR})

# -----------------------------------------------------------------------------
# main floorplan programs
# -----------------------------------------------------------------------------
if(FP_BUILD_MAIN)
  add_subdirectory(${FP_MAIN_DIR})
endif(FP_BUILD_MAIN)

# -----------------------------------------------------------------------------
# Unittest
# -----------------------------------------------------------------------------

include(CTest)

if(FP_BUILD_TESTS)
  add_subdirectory(tests)
endif(FP_BUILD_TESTS)


