


#include "floorplan.hpp"


std::ostream& operator<< (std::ostream &out, const std::vector<int>& vec) {
  for(int i = 0; i < vec.size(); ++i) {
    out << vec[i] << ' ';
  }
  out << '\n';
 
  return out; 
}

// TODO
namespace fp {



// print out the read-in modules
void Floorplan::print_modules(std::ostream& os) {
  for(int i = 0; i < _modules.size(); ++i) {
    os << "module[" << _modules[i].idx
       << "] has width = " << _modules[i].w
       << " and height = "  << _modules[i].h
       << ", located at ("   << _modules[i].llx
       << ", " << _modules[i].lly
       << ")\n"; 
  }
}


// read modules from input file
void Floorplan::open(const std::string& input_file) {
  _input_file = input_file;

  std::ifstream infile(input_file, std::ios::in);
  
  if(!infile) {
    std::cerr << "File could not be opened\n";
    std::exit(EXIT_FAILURE);
  }
  
  int num_modules, width, height, index;
  infile >> num_modules;

  while(infile >> index >> width >> height) {
    module_t temp;
    temp.idx = index;
    temp.w = width;
    temp.h = height;
    temp.llx = 0;
    temp.lly = 0;
    _modules.push_back(temp);
  }
    
  for(int i = 0; i < _modules.size(); ++i)
    _mapping.push_back(i);
}


// dump floor plan to console
void Floorplan::dump(std::ostream& os) const {
  os << "0 0 " << _urx << " " << _ury << '\n';
  
  for(int i = 0; i < _modules.size(); ++i) {
    os << _modules[i].idx << " "
       << _modules[i].llx << " "
       << _modules[i].lly << " "
       << _modules[i].w   << " "
       << _modules[i].h   << '\n';
  }
}


// dump floor plan to a text file and a json file
void Floorplan::dump(const std::string& output_file) const {

  std::ofstream outfile(output_file, std::ios::out);
 
  if(!outfile) {
    std::cerr << "File could not be opened for writing\n";
    std::exit(EXIT_FAILURE); 
  }
 
  outfile << "0 0 " << _urx << " " << _ury << '\n';
  for(int i = 0; i < _modules.size(); ++i) {
    outfile << _modules[i].idx << " "
            << _modules[i].llx << " "
            << _modules[i].lly << " "
            << _modules[i].w   << " "
            << _modules[i].h   << '\n';
  }

  // generate json output 
  std::string output_file_json = output_file;
  output_file_json.replace(output_file_json.end()-3,
                           output_file_json.end(), "json");
 
  std::ofstream outfile_json(output_file_json, std::ios::out);
  outfile_json << "{\"circuit\":\"" << _input_file << "\""
               << ",\"block_number\":" << _modules.size()
               << ",\"llx\":0"
               << ",\"lly\":0"
               << ",\"urx\":" << _urx
               << ",\"ury\":" << _ury
               << ",\"area\":" << _area
               << ",\"coordinates\":"
               << "[";
  for(int i = 0; i < _modules.size(); ++i) {
    outfile_json << "{\"idx\":" << _modules[i].idx
                 << ",\"llx\":" << _modules[i].llx
                 << ",\"lly\":" << _modules[i].lly
                 << ",\"width\":" << _modules[i].w
                 << ",\"height\":" << _modules[i].h;
    if(i == _modules.size()-1)
      outfile_json << "}";
    else
      outfile_json << "},";
  }
  outfile_json << "]}";
}



// sort modules with respect to its area
void Floorplan::_sort_modules_wrt_area() {

  for(int i = 0; i < _modules.size(); ++i)
    _sorted_modules_area.push_back(std::make_pair(
    i, _modules[i].w * _modules[i].h));

  std::sort(_sorted_modules_area.begin(), 
            _sorted_modules_area.end(), 
            [](std::pair<int, int>& i, std::pair<int, int>& j){
              return i.second < j.second;
            }
  );  
}
  
 
// generate an initial expression 
void Floorplan::_generate_initial_expression() {
    
  for(size_t i = 0; i < _sorted_modules_area.size(); ++i) {
    if(i == 0) {
      _expression.push_back(_sorted_modules_area[i].first);
    }
    // -1 means a vertical cutline
    else if(i%2 == 1) {
      _expression.push_back(_sorted_modules_area[i].first);
      _expression.push_back(-1);
    }
    // -2 means a horizontal cutline
    else {
      _expression.push_back(_sorted_modules_area[i].first);
      _expression.push_back(-2);
    }
  }
}


// generate an optimized floor plan  
void Floorplan::optimize(const int& max_iterations_per_temperature, 
                             double& initial_temperature, 
                             const double& frozen_temperature) {
  
  // TODO: measure the time using std::chrono
  
  auto tbeg = std::chrono::steady_clock::now();
  _sort_modules_wrt_area();
  auto tend = std::chrono::steady_clock::now();

  std::cout << "sort modules completed: " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(tend-tbeg).count()
            << '\n';


  _generate_initial_expression();
  
  initial_temperature = _calculate_initial_temperature(); 

  _simulated_annealing(max_iterations_per_temperature,
                       initial_temperature, 
                       frozen_temperature);
   
  _modules = _modules_best;

  int area = _pack(_expression);
  std::cout << "area = " << area << '\n';
}


// check if the expression is valid
bool Floorplan::_is_valid_expression(const std::vector<int>& expression) const {
  std::vector<int> operand_count(expression.size(), 0);
  std::vector<int> operator_count(expression.size(), 0);

  if(expression.empty())
    return true;

  if(expression[0] >= 0)
    operand_count[0] = 1;
  else
    return false;

  if(expression.back() >= 0)
    return false;

  for(int i = 1; i < expression.size(); ++i) {
    if(expression[i] >= 0) {
      operand_count[i] = operand_count[i-1] + 1;
      operator_count[i] = operator_count[i-1];
    }
    else {
      operator_count[i] = operator_count[i-1] + 1;
      operand_count[i] = operand_count[i-1];
    }

    if(operand_count[i] <= operator_count[i])
      return false;
  }

  return true;
}


// calculate an initial temperature 
double Floorplan::_calculate_initial_temperature() {
  int num_moves = 0;
  double total_area_change = 0.0;
  double delta_area, avg_area_change, init_temperature;

  std::vector<int> expression_curr = _expression;
  std::vector<int> expression_prop = _expression;

  int area_curr = _pack(expression_curr);
  int area_prop;

  while(num_moves < random_moves) {
    _generate_neighbor(expression_curr, expression_prop);

    area_prop = _pack(expression_prop);
    delta_area = area_prop - area_curr;

    total_area_change += abs(delta_area);
    ++num_moves;
    expression_curr = expression_prop;
    area_curr = area_prop;
  }

  avg_area_change = total_area_change / num_moves;
  init_temperature = P < 1 ? (t0 * avg_area_change) / log(P) : avg_area_change / log(P);

  return init_temperature;
}

// perform simulated annealing
//
void Floorplan::_simulated_annealing(
  const int& max_iterations_per_temperature,  // TODO: I suggested using size_t
  const double& initial_temperature,
  const double& frozen_temperature) {
    
  double temperature = initial_temperature;

  std::vector<int> expression_prop;
  std::vector<int> expression_curr;
  std::vector<int> expression_best;   
    
  expression_curr = _expression;
  expression_best = expression_curr;
    
  int area_best = _pack(expression_best);
  int area_curr = area_best;
          
  std::random_device rd;
  std::mt19937 gen(rd());  // expensive - typically construct once
  std::uniform_real_distribution<> dis(0, 1);
  
  std::srand(std::time(nullptr)); 
  //std::cout << "postfix_curr : " << postfix_curr << '\n'; 
  //std::cout << "postfix_best : " << postfix_best << '\n'; 
  //std::cout << "area_curr : " << area_curr << '\n';
  //std::cout << "area_best : " << area_best << '\n';
  //std::cout << "--------------------\n";

  while(temperature > frozen_temperature) {
    for(int iter = 0; iter < max_iterations_per_temperature; iter++) {
      expression_prop = expression_curr;
      _generate_neighbor(expression_curr, expression_prop);
      
      // TODO: can we try using aspect ratio? we want to be as close to 1.0 as
      // possible ... a = max(H, W) / min(H, W)
      int area_prop = _pack(expression_prop);
      int cost = area_prop - area_curr;
        
      //std::cout << "postfix_prop : " << postfix_prop << '\n'; 
      //std::cout << "postfix_curr : " << postfix_curr << '\n'; 
      //std::cout << "postfix_best : " << postfix_best << '\n'; 
      //std::cout << "area_best : " << area_best << '\n';
      //std::cout << "area_curr : " << area_curr << '\n';
      //std::cout << "area_prop : " << area_prop << '\n';
      //std::cout << "@@@@@@@@@@@@@@@@@@@@@\n";

      if(cost < 0) {
        expression_curr = expression_prop;
        area_curr = area_prop;
        if(area_prop < area_best) {
          expression_best = expression_prop;
          area_best = area_prop;
          _modules_best = _modules;
        }
      }

      else {
        auto prob = std::exp(-cost / temperature); 
        if(prob > dis(gen)) {
          expression_curr = expression_prop;
          area_curr = area_prop; 
        }
      }
    }
    temperature *= 0.95;  
  }

  _expression = expression_best;
}
  
  
// pack modules
int Floorplan::_pack(const std::vector<int>& expression) {
  while(!_stack.empty())
    _stack.pop();

  int cluster_id = 0;

  for(int i = 0; i < expression.size(); ++i) {
            
    // cutline
    if((expression[i] == -1) || (expression[i] == -2)) {
       _pack_cutline(expression[i]);
    }

    // integer idx 
    else {
      cluster_t cluster;
      int idx = expression[i];
        
      _modules[idx].llx = 0;
      _modules[idx].lly = 0;
      cluster.w = _modules[idx].w;
      cluster.h = _modules[idx].h;
      cluster.beg = cluster_id;
      cluster.end = cluster_id;
      
      _mapping[cluster_id] = idx;
      ++cluster_id;
      
      _stack.push(cluster);
    }
  }

  _area = (_stack.top()).w * (_stack.top()).h;
  _urx = (_stack.top()).w;
  _ury = (_stack.top()).h;

  return _area;
}


void Floorplan::_pack_cutline(const int& cutline) {
  cluster_t cluster, cluster_r, cluster_l;
    
  cluster_r = _stack.top();
  _stack.pop();
  cluster_l = _stack.top();
  _stack.pop();

  cluster.beg = cluster_l.beg;
  cluster.end = cluster_r.end;

  // horizontal cutline
  if(cutline == -2) {
    for(int i = cluster_r.beg; i <= cluster_r.end; ++i) {
      _modules[_mapping[i]].lly += cluster_l.h;
    }

    cluster.w = (cluster_l.w > cluster_r.w) ? cluster_l.w : cluster_r.w;
    cluster.h = cluster_l.h + cluster_r.h;
  }
    
  // vertical cutline
  else {
    for(int i = cluster_r.beg; i <= cluster_r.end; ++i) {
      _modules[_mapping[i]].llx += cluster_l.w; 
    }

    cluster.w = cluster_l.w + cluster_r.w;
    cluster.h = (cluster_l.h > cluster_r.h) ? cluster_l.h : cluster_r.h;
  }

  _stack.push(cluster);
}
  
  
// M1 : swap two adjacent operands
void Floorplan::_operand_swap(std::vector<int>& prop) {

  int head, tail;

  while(1) {
    head = (std::rand()) % (prop.size()-1);
    tail = head + 1;
      
    int pph = prop[head];

    if(pph >= 0) {
      while(tail < prop.size()) {
        int ppt = prop[tail];
        if(ppt >= 0) {
          std::swap(prop[head], prop[tail]);
          return;
        }
        else {
          ++tail;
          continue;
        }
      }
    }
    else
      continue;
  }
}


// M2 : complement a cutline
void Floorplan::_complement_cutline(std::vector<int>& prop) const {

  int head;

  while(1) {
    head = (std::rand()) % (prop.size()-1);
    int pph = prop[head];

    if(pph == -2) {
      prop[head] = -1;
      return;
    }
    else if(pph == -1) {
      prop[head] = -2;
      return;
    }
    else
      continue;
  }
}
 

// M3 : complement last pair of two cutlines
bool Floorplan::_complement_last2cutline(std::vector<int>& prop) const {
  
  for(int i = prop.size()-1; i > 0; --i) {
    if((prop[i] < 0) && (prop[i-1] < 0)) {
      if(prop[i] == -1)
        prop[i] = -2;
      else
        prop[i] = -1;

      if(prop[i-1] == -1)
        prop[i-1] = -2;
      else
        prop[i-1] = -1;

      return true;
    }
    else
      continue;
  }
  return false;
}
  
  

// M4 : swap two adjacent operand and operator
void Floorplan::_operator_operand_swap(const std::vector<int>& curr,
                                           std::vector<int>& prop) const {
    
  int head, tail;

  while(1) {
    //std::cout << "trap here";
    head = (std::rand()) % (prop.size()-1);
    tail = head + 1;
     
    //std::cout << "head = " << head << '\n'; 
    int pph = prop[head];
    int ppt = prop[tail];

    if(pph < 0) {
      if(ppt >= 0) {
        std::swap(prop[head], prop[tail]);
        //std::cout << "[" << head << "]=" << postfix_prop[head] << " , tail = " << tail << '\n';
        if(_is_valid_expression(prop) == false) {
          //std::cout << "not valid\n";
          prop = curr;
          continue;
        }
        else {
          //std::cout << "valid\n";
          return;
        }
      }
      else
        continue;
    }

    if(pph >= 0) {
      if(ppt < 0) {
        std::swap(prop[head],prop[tail]);
        if(_is_valid_expression(prop) == false) {
          //std::cout << "not valid\n";
          prop = curr;
          continue;
        }
        else {
          //std::cout << "valid\n";
          return;
        }
      }
      else
        continue;
    }
  }
}
 

// M5 : complement first pair of two cutlines, consecutive cutlines must be different
bool Floorplan::_complement_first2cutline(std::vector<int>& prop) const {

  // private method uses assert!!!
  assert(prop.size() > 0);

  // TODO: what happened when prop.size == 0?
  for(int i = 0; i < prop.size()-1; ++i) {
    if((prop[i] < 0) && (prop[i+1] < 0)) {
      if(prop[i] == -1)
        prop[i] = -2;
      else
        prop[i] = -1;

      if(prop[i+1] == -1)
        prop[i+1] = -2;
      else
        prop[i+1] = -1;

      return true; 
    }
    else
      continue;
  }
  return false;
}
  
  
// M6 : randomly roate one module
void Floorplan::_rotate_module(const std::vector<int>& curr) {
  int head;

  while(1) {
    head = (std::rand()) % (curr.size()-1);
    if(curr[head] >= 0) {
      int idx = curr[head];
      std::swap(_modules[idx].w, _modules[idx].h); 

      return;
    }
    else
      continue;
  }
}


// generate different expressions from neighbor
void Floorplan::_generate_neighbor(const std::vector<int>& curr,
                                       std::vector<int>& prop) {

  bool is_exist = true;

  while(1) {
    switch(std::rand()%6) {
      case 0:
        std::cout << "rand 0 : \n";
        std::cout << curr;
        _operand_swap(prop);
        std::cout << prop;
        //++count[0];
        break;
      case 1:
        std::cout << "rand 1 : \n";
        std::cout << curr;
        _complement_cutline(prop);
        std::cout << prop;
        //++count[1];
        break;
      case 2:
        std::cout << "rand 2 : \n";
        std::cout << curr;
        is_exist = _complement_last2cutline(prop);
        //++count[2];
        if(is_exist == false)
          continue;
        std::cout << prop;
        break;
      case 3:
        std::cout << "rand 3 : \n";
        std::cout << curr;
        _operator_operand_swap(curr, prop);
        std::cout << prop;
        //++count[3];
        break;
      case 4:
        std::cout << "rand 4 : \n";
        std::cout << curr;
        is_exist = _complement_first2cutline(prop);
        //++count[4];
        if(is_exist == false)
          continue;
        std::cout << prop;
        break;
      case 5:
        std::cout << "rand 5 : \n";
        std::cout << curr;
        _rotate_module(curr);
        std::cout << prop;
        //++count[5];
        break;
    }
    break;
  }
}



// the following definitions are used for testing purposes

FloorplanTester::FloorplanTester() {
  Floorplan _tester_fp;
}

void FloorplanTester::sort_modules_wrt_area() {
  _tester_fp.open("../../circuits/circuit1.txt");
  _tester_fp._sort_modules_wrt_area();

  for(int i = 0; i < _tester_fp._sorted_modules_area.size(); ++i)
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
