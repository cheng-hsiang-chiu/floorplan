#include <iostream>
#include <string>
#include <vector>

#include "floorplan.hpp"


std::ostream& operator<< (std::ostream &out, const std::vector<int>& vec) {
  for(int i = 0; i < vec.size(); ++i) {
    out << vec[i] << ' ';
  }
  out << '\n';
 
  return out; 
}



int main(int argc, char* argv[]) {

  if(argc < 6) {
    std::cerr << "usage: ./floorplan input output ite itemp ftemp\n";
    std::exit(EXIT_FAILURE);
  }

  std::string input_file = argv[1];
  std::string outpu_tfile = argv[2];
  int max_iterations_per_temperature = std::stoi(argv[3]);
  double initial_temperature = std::stod(argv[4]);
  double frozen_temperature = std::stod(argv[5]);

  // declare a fp object 
  fp::Floorplan fp_obj;
  
  // read in circuit configurations
  fp_obj.open(input_file);

  // start to generate optimized floorplan
  fp_obj.optimize(
    max_iterations_per_temperature, initial_temperature, frozen_temperature 
  );  

  //fp.dump(std::cout);


  //fp.sort_modules_size();
  //fp.run();
  //std::cout << fp.calculate_initial_temperature() << '\n';
  //fp.print_modules();
  //std::cout << fp.is_valid_expression();
  //std::cout << fp.operand_swap() << '\n';
  //std::cout << fp.complement_cutline() << '\n';
  //std::cout << fp.complement_first2cutline() << '\n';
  //fp.chain_invert();
  //std::cout << fp.operator_operand_swap() << '\n';
  //std::cout << count;
  //std::cout << accept << '\n';
  return 0;
}
