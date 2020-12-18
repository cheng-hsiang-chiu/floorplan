#include <iostream>
#include <string>
#include <vector>

#include <floorplan.hpp>

int main(int argc, char* argv[]) {

  if(argc < 3) {
    std::cerr << "usage: ./floorplan input output ite itemp\n";
    std::exit(EXIT_FAILURE);
  }

  std::string input_file = argv[1];
  std::string output_file = argv[2];

  // declare a fp object 
  fp::Floorplan fp_obj;
  
  // read in circuit configurations
  fp_obj.open(input_file);

  // start to generate optimized floorplan
  fp_obj.optimize();  

  // dump the floor plan to console
  fp_obj.dump(std::cout);

  // dump the floor plan to output file and its json format
  fp_obj.dump_json(output_file);
  
  return 0;
}
