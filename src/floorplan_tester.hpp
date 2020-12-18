#pragma once

#include <string>
#include <vector>
#include "floorplan.hpp"

namespace fp {


class FloorplanTester {

public:
  std::vector<int> tester_sorted_modules_area;
  
  FloorplanTester();

  bool is_valid_expression(const std::vector<int>& expression) const;

  bool operand_swap(std::vector<int>& prop);
 
  bool complement_cutline(std::vector<int>& prop);

  bool complement_last2cutline(std::vector<int>& prop);

  bool complement_first2cutline(std::vector<int>& prop);

  void rotate_module(const std::vector<int>& curr);

  void operator_operand_swap(const std::vector<int>& curr,
                             std::vector<int>& prop);
  
  int pack(const std::vector<int>& expression);

  void sort_modules_wrt_area();

private:
  Floorplan _tester_fp;
};

} // end of namespace fp
