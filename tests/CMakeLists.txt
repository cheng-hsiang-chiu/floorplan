enable_testing()

include(${FP_3RD_PARTY_DIR}/doctest/doctest.cmake)

add_executable(basics basics.cpp)
target_include_directories(basics PRIVATE ${FP_3RD_PARTY_DIR}/doctest)
doctest_discover_tests(basics)
