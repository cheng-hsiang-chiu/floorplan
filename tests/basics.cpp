#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>
#include "../src/floorplan.hpp"

TEST_CASE("testing is_valid_expression" * doctest::timeout(300)) {
  fp::FloorplanTester tester;
  
  std::vector<int> exp{1,2,1};
  REQUIRE(tester.is_valid_expression(exp) == false);

  exp = {-1,2,1};
  REQUIRE(tester.is_valid_expression(exp) == false);

  exp = {1,-2,1};
  REQUIRE(tester.is_valid_expression(exp) == false);

  exp = {1,2,-1};
  REQUIRE(tester.is_valid_expression(exp) == true);

  exp = {1,2,-1,3};
  REQUIRE(tester.is_valid_expression(exp) == false);

  exp = {1,2,-1,-1};
  REQUIRE(tester.is_valid_expression(exp) == false);

  exp = {1,2,3,4,5,-1,-2,-1,-2};
  REQUIRE(tester.is_valid_expression(exp) == true);
}


TEST_CASE("testing operation M1" * doctest::timeout(300)) {
  fp::FloorplanTester tester;
  
  std::vector<int> prop{1,2,3,4,5,-1,-1,-1,-1};

  REQUIRE(tester.operand_swap(prop) == true);
}


TEST_CASE("testing operation M2" * doctest::timeout(300)) {
  fp::FloorplanTester tester;
  
  std::vector<int> prop{1,2,3,4,5,-1,-1,-1,-1};

  REQUIRE(tester.complement_cutline(prop) == true);
}






// TODO: complete the following unittests
// 1. is_valid_expression?
// 2. pack
// 3. operations
