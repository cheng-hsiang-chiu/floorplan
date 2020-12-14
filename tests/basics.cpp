#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>
#include "../src/floorplan.hpp"

TEST_CASE("testing is_valid_expression" * doctest::timeout(300)) {
  fp::FloorplanTester tester;
  
  std::vector<int> expression{1,2,1};
  REQUIRE(tester.is_valid_expression(expression) == false);

  expression = {-1,2,1};
  REQUIRE(tester.is_valid_expression(expression) == false);

  expression = {1,-2,1};
  REQUIRE(tester.is_valid_expression(expression) == false);

  expression = {1,2,-1};
  REQUIRE(tester.is_valid_expression(expression) == true);

  expression = {1,2,-1,3};
  REQUIRE(tester.is_valid_expression(expression) == false);

  expression = {1,2,-1,-1};
  REQUIRE(tester.is_valid_expression(expression) == false);

  expression = {1,2,3,4,5,-1,-2,-1,-2};
  REQUIRE(tester.is_valid_expression(expression) == true);
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



TEST_CASE("testing operation M3" * doctest::timeout(300)) {
  fp::FloorplanTester tester;
  
  std::vector<int> prop{1,2,3,4,5,-1,-1,-1,-1};

  REQUIRE(tester.complement_last2cutline(prop) == true);
  REQUIRE(tester.is_valid_expression(prop) == true);
  REQUIRE(prop == std::vector<int>{1,2,3,4,5,-1,-1,-2,-2});
}



TEST_CASE("testing operation M4" * doctest::timeout(300)) {
  fp::FloorplanTester tester;
  
  std::vector<int> prop{1,2,3,4,5,-1,-1,-1,-1};
  std::vector<int> curr{1,2,3,4,5,-1,-1,-1,-1};

  tester.operator_operand_swap(curr, prop);
  REQUIRE(tester.is_valid_expression(prop) == true);
  REQUIRE(curr == std::vector<int>{1,2,3,4,5,-1,-1,-1,-1});
}


TEST_CASE("testing operation M5" * doctest::timeout(300)) {
  fp::FloorplanTester tester;
  
  std::vector<int> prop{1,2,3,4,5,-1,-1,-1,-1};

  REQUIRE(tester.complement_first2cutline(prop) == true);
  REQUIRE(tester.is_valid_expression(prop) == true);
  REQUIRE(prop == std::vector<int>{1,2,3,4,5,-2,-2,-1,-1});
}


TEST_CASE("testing operation M6" * doctest::timeout(300)) {
  fp::FloorplanTester tester;
  
  std::vector<int> curr{1,2,3,4,5,-1,-1,-1,-1};
    
  tester.rotate_module(curr);
  REQUIRE(curr == std::vector<int>{1,2,3,4,5,-1,-1,-1,-1});
}



TEST_CASE("testing pack" * doctest::timeout(300)) {
  fp::FloorplanTester tester;
  
  std::vector<int> expression{0,1,2,3,-1,-1,-1};
  REQUIRE(tester.pack(expression) == 498760);

  expression = {0,1,2,3,-2,-2,-2};
  REQUIRE(tester.pack(expression) == 533628);
  
  expression = {0,1,2,3,-1,-1,-2};
  REQUIRE(tester.pack(expression) == 831760);
  
  expression = {0,1,2,3,-1,-2,-2};
  REQUIRE(tester.pack(expression) == 836160);
  
  expression = {0,1,2,3,-1,-2,-1};
  REQUIRE(tester.pack(expression) == 654368);
  
  expression = {0,1,2,3,-2,-1,-1};
  REQUIRE(tester.pack(expression) == 567056);
  
  expression = {0,1,2,3,-2,-1,-2};
  REQUIRE(tester.pack(expression) == 691488);
  
  expression = {0,1,2,3,-2,-2,-1};
  REQUIRE(tester.pack(expression) == 560404);
}


TEST_CASE("testing sorted_modules_wrt_area" * doctest::timeout(300)) {
  fp::FloorplanTester tester;
 
  tester.sort_modules_wrt_area();
   
  REQUIRE(tester.tester_sorted_modules_area[0] == 1);
  REQUIRE(tester.tester_sorted_modules_area[1] == 0);
  REQUIRE(tester.tester_sorted_modules_area[2] == 2);
  REQUIRE(tester.tester_sorted_modules_area[3] == 3);
}


// TODO: complete the following unittests
// 1. is_valid_expression?
// 2. pack
// 3. operations
