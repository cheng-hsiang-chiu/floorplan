# CMake version
cmake_minimum_required (VERSION 3.10)
MESSAGE(STATUS "CMAKE_ROOT: " ${CMAKE_ROOT})

# Project name
project(Floorplan VERSION 1.0.0 LANGUAGES CXX)

# build options
option(FP_BUILD_TESTS "Enables builds of tests" ON)
option(FP_BUILD_EXAMPLES "Enables builds of examples" ON)

# project-specific variables
set(FP_INC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(FP_LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(FP_UTEST_DIR ${PROJECT_SOURCE_DIR}/unittests)
set(FP_3RD_PARTY_DIR ${PROJECT_SOURCE_DIR}/3rd-party)
set(FP_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(FP_DEFAULT_BUILD_TYPE "Release")

# Turn on the verbose
set(CMAKE_VERBOSE_MAKEFILE ON)

## TODO define the executables ----------------------------------------
add_executable(Floorplan ${FP_SRC_DIR}/main.cpp)

## please study this
##target_link_libraries(
##  ${example} ${PROJECT_NAME} tf::default_settings
##)
target_link_libraries(Floorplan fplib)

add_subdirectory(${FP_SRC_DIR})

## TODO define unittest
# -----------------------------------------------------------------------------
# Unittest
# -----------------------------------------------------------------------------

include(CTest)

if(FP_BUILD_TESTS)
  add_subdirectory(tests)
endif(FP_BUILD_TESTS)


