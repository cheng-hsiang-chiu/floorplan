#include <iostream>
#include <string>
#include <vector>

#include "floorplan.hpp"



int main(int argc, char* argv[]) {

  if(argc < 6) {
    std::cerr << "usage: ./floorplan input output ite itemp ftemp\n";
    std::exit(EXIT_FAILURE);
  }

  std::string input_file = argv[1];
  std::string output_file = argv[2];
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

  // dump the floor plan to console
  fp_obj.dump(std::cout);

  // dump the floor plan to output file and its json format
  fp_obj.dump(output_file);
  
  return 0;
}
