#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>
#include <cassert>
#include <vector>
#include <string>
#include <stack>



#define random_moves 60
#define P 0.99
#define t0 -1

#define FP_RANDOM_MOVES 60
#define FP_FROZEN_TEMPERATURE 0.1
#define FP_MAX_ITERATIONS_PER_TEMPERATURE 1000
//#define FP_SA_RATIO 0

std::ostream& operator<< (std::ostream &out, const std::vector<int>& vec);


namespace fp {

typedef struct MODULE {
  size_t idx;
  int llx, lly;
  int w, h;
}module_t;

typedef struct CLUSTER {
  size_t beg, end;
  int w, h;
}cluster_t;


class Floorplan {
  
  friend class FloorplanTester;

  public:
  
    Floorplan() = default;
    
    // TODO: POD data types don't need reference
    void optimize();
    
    // TODO: constant method 
    // rename to dump_modules
    void dump_modules(std::ostream& os) const;
  
    void open(const std::string& input_file);
  
    void dump(std::ostream& os) const;
    
    // TODO: a single dump is enough => just dumps it to json or
    //auto ofs = std::ofstream("myoutput");
    //fp.dump(ifs);
  
    void dump_json(std::string& output_file) const;
  
  private:
    std::vector<int> _expression;
    
    std::vector<module_t> _modules;
    
    std::vector<module_t> _modules_best;
    
    std::vector<std::pair<int, int> > _sorted_modules_area;
    
    std::stack<cluster_t> _stack;
    
    std::vector<int> _mapping;

    std::vector<int> _cost;
  
    std::string _input_file;
  
    int _area, _urx, _ury;
    
  
  
    void _sort_modules_wrt_area();
  
    void _generate_initial_expression();
  
    double _calculate_initial_temperature();
    
    bool _is_valid_expression(const std::vector<int>& expression) const;
  
    int _pack(const std::vector<int>& postfix);
    
    // TODO: pod doesn't need reference
    void _pack_cutline(const int& cutline);
    
    void _operand_swap(std::vector<int>& prop);
    
    void _complement_cutline(std::vector<int>& prop) const;
    
    bool _complement_last2cutline(std::vector<int>& prop) const;
  
    bool _complement_first2cutline(std::vector<int>& prop) const;
    
    void _rotate_module(const std::vector<int>& curr);
  
    void _operator_operand_swap(const std::vector<int>& curr,
                                std::vector<int>& prop) const;
    
    void _generate_neighbor(const std::vector<int>& curr,
                            std::vector<int>& prop);
    
    void _simulated_annealing(const double initial_temperature);
    
};

} // end of namespace fp
