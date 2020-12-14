// ...
//
// 
// class Floorplan {
//
//   friend class FloorplanTest;
//   friend class YourCompnayTester;
//  
//   public:
//     Floorplan();
//     void run();
//     void dump(std::ostream& os) const;
//   
//   private:
//
//     bool _is_expression_valid(const std::vector<int>& exp) const
// }
//
// 
// class FloorplanTester {
//
//    public:
//
//    bool is_expression_valid(const std::vector<int>& exp) const {
//      return _floorplanner._is_expression_valid(exp);
//    }
//
//    private:
//
//       Floorplan _floorplanner;
//
// }
//
#include <string>
#include <stack>
#include <vector>

#define random_moves 60
#define P 0.99
#define t0 -1


std::ostream& operator<< (std::ostream &out, const std::vector<int>& vec);


namespace fp {

typedef struct MODULE {
  int idx;
  int llx, lly;
  int w, h;
}module_t;

typedef struct CLUSTER {
  int beg, end;
  int w, h;
}cluster_t;


class Floorplan {

public:
  friend class FloorplanTester;

  Floorplan();
  
  void optimize(const int& max_iterations_per_temperature,
                double& initial_temperature,
                const double& frozen_temperature);

  void print_modules(std::ostream& os);

  void open(const std::string& input_file);

  void dump(std::ostream& os) const;

  void dump(const std::string& output_file) const;

private:
  std::vector<int> _expression;
  
  std::vector<module_t> _modules;
  
  std::vector<module_t> _modules_best;
  
  std::vector<std::pair<int, int>> _sorted_modules_area;
  
  std::stack<cluster_t> _stack;
  
  std::vector<int> _mapping;

  std::string _input_file;

  int _area, _urx, _ury;
  
  void _sort_modules_wrt_area();

  void _generate_initial_expression();



  double _calculate_initial_temperature();
  
  bool _is_valid_expression(const std::vector<int>& expression) const;

  int _pack(const std::vector<int>& postfix);

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
  
  void _simulated_annealing(const int& max_iterations_per_temperature,
                            const double& initial_temperature,
                            const double& frozen_temperature);
};

class FloorplanTester {

public:
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

private:
  Floorplan _fp_obj;
};

} // end of namespace fp
