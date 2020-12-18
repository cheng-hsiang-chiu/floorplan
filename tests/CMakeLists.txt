cmake_minimum_required(VERSION 3.10)

enable_testing()

include(${FP_3RD_PARTY_DIR}/doctest/doctest.cmake)

## --------------------------------------------------------
## unit test 1: basics
## --------------------------------------------------------
add_executable(basics basics.cpp)
target_include_directories(basics PRIVATE ${FP_3RD_PARTY_DIR}/doctest)
target_link_libraries(basics libfloorplantester)
doctest_discover_tests(basics)


