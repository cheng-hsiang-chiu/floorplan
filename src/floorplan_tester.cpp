#include <algorithm>
#include "floorplan_tester.hpp"
#include "floorplan.hpp"

namespace fp {


FloorplanTester::FloorplanTester() {
  Floorplan _tester_fp;
}

void FloorplanTester::sort_modules_wrt_area() {
  _tester_fp.open("../../circuits/circuit1.txt");
  _tester_fp._sort_modules_wrt_area();

  for(size_t i = 0; i < _tester_fp._sorted_modules_area.size(); ++i)
    tester_sorted_modules_area.push_back(_tester_fp._sorted_modules_area[i].first);
}


bool FloorplanTester::is_valid_expression(
  const std::vector<int>& expression) const {
  return _tester_fp._is_valid_expression(expression);
}

bool FloorplanTester::operand_swap(std::vector<int>& prop) {
  _tester_fp._operand_swap(prop);
  return _tester_fp._is_valid_expression(prop);

}

bool FloorplanTester::complement_cutline(std::vector<int>& prop) {
  _tester_fp._complement_cutline(prop);
  return _tester_fp._is_valid_expression(prop);
}


bool FloorplanTester::complement_last2cutline(std::vector<int>& prop) {
  return _tester_fp._complement_last2cutline(prop);
}


bool FloorplanTester::complement_first2cutline(std::vector<int>& prop) {
  return _tester_fp._complement_first2cutline(prop);
}


void FloorplanTester::rotate_module(const std::vector<int>& curr) {
  _tester_fp.open("../../circuits/circuit1.txt");
  _tester_fp._rotate_module(curr);
}


void FloorplanTester::operator_operand_swap(
  const std::vector<int>& curr, std::vector<int>& prop) {
  _tester_fp._operator_operand_swap(curr, prop);
}

int FloorplanTester::pack(const std::vector<int>& expression) {
  _tester_fp.open("../../circuits/circuit1.txt");
  return _tester_fp._pack(expression);
}


}
