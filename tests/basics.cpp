#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>

TEST_CASE("basics" * doctest::timeout(300)) {

  int result = 0;

  for(int i=0; i<10; i++) result += 2;

  REQUIRE(result == 20);

  //FloorplanTester tester;

  //std::vector<int> expression = {1, 2, 3, -1, -1};

  //REQUIRE(tester.is_valid_expression(expression) == true);
}

// TODO: complete the following unittests
// 1. is_valid_expression?
// 2. pack
// 3. operations
