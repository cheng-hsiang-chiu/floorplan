#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>



typedef struct MODULE {
  int idx;
  int llx, lly;
  int w, h;
}module_t;



typedef struct CLUSTER {
  int beg, end;
  int w, h;
}cluster_t;



std::vector<module_t> read_modules(std::string);

class floorplan {
public:
  floorplan(std::string input, std::string output) 
    : _input_file(input), _output_file(output) {
    _modules = read_modules(_input_file);
  }  


  // execute and generate an output file and its json
  void run() {
    packing();
    
    std::ofstream outfile(_output_file, std::ios::out);

    if(!outfile)
      std::cerr << "File could not be opened for writing\n";

    for(int i = 0; i < _modules.size(); ++i) {
      outfile << _modules[i].idx << " " 
              << _modules[i].llx << " "
              << _modules[i].lly << " "
              << _modules[i].w   << " "
              << _modules[i].h   << '\n';
    }

    // generate json output 
    std::string output_file_json = _output_file;
    output_file_json.replace(output_file_json.end()-3, 
                             output_file_json.end(), "json");

    std::ofstream outfile_json(output_file_json, std::ios::out);
    outfile_json << "{\"circuit\":\"" << _input_file << "\""
                 << ",\"block_number\":" << _modules.size()
                 << ",\"llx\":0"
                 << ",\"lly\":0"
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


  // check if postfix is valid
  bool is_valid_postfix() {
    std::stack<char> stk;
    
    for(int i = 0; i < _postfix.size(); ++i) {
      if((_postfix[i] == 'V') || (_postfix[i] == 'H')) {
        if(stk.size() >= 2) { 
          stk.pop();

          if(i == _postfix.size()-1)
            stk.pop();
        }
      }

      else {
        stk.push(_postfix[i]);
      }
    }

    if(stk.size() == 0)
      return true;
    else
      return false;
  }


  // check if candidate_postfix is valid
  bool is_valid_postfix(const std::string& str) {
    std::stack<char> stk;
    
    for(int i = 0; i < str.size(); ++i) {
      if((str[i] == 'V') || (str[i] == 'H')) {
        if(stk.size() >= 2) { 
          stk.pop();

          if(i == str.size()-1)
            stk.pop();
        }
      }

      else {
        stk.push(str[i]);
      }
    }

    if(stk.size() == 0)
      return true;
    else
      return false;
  }


  // swap two adjacent operands
  void operand_swap() {
    int head = 0, tail = 1;
    
    while(tail < _postfix.size()) {
      std::string candidate_postfix = _postfix;

      char cph = candidate_postfix[head];
      char cpt = candidate_postfix[tail];

      if((cph != 'H') && (cph != 'V') && (cpt != 'H') && (cpt != 'V')) {
        std::swap(candidate_postfix[head], candidate_postfix[tail]);
        head = tail;
        ++tail;
        std::cout << candidate_postfix << '\n';
        continue;
      }
      ++tail;
    }
  }


  // complement chains, V->H, H->V
  void chain_invert() {
    for(int i = 0; i < _postfix.size(); ++i) {
      std::string candidate_postfix = _postfix;
      if(candidate_postfix[i] == 'H') {
        candidate_postfix[i] = 'V';
        std::cout << candidate_postfix << '\n';
      }

      else if(candidate_postfix[i] == 'V') {
        candidate_postfix[i] = 'H';
        std::cout << candidate_postfix << '\n'; 
      }
    }
  }
 
 
  // swap two adjacent operand and operator
  void operator_operand_swap() {
    int head = 0, tail = 1;

    while(tail < _postfix.size()) {
      std::string candidate_postfix = _postfix;
    
      char cph = candidate_postfix[head];
      char cpt = candidate_postfix[tail];

      // cph is an idx and cpt is a cutline
      if((cph != 'H') && (cph != 'V')) {
        if((cpt == 'H') || (cpt == 'V')) {
          std::swap(candidate_postfix[head], candidate_postfix[tail]);
          head = tail;
          ++tail;
          if(is_valid_postfix(candidate_postfix))
            std::cout << candidate_postfix << " is valid.\n";
          else
            std::cout << candidate_postfix << " is not valid.\n";
          continue;
        }
        else {
          ++head;
          ++tail;
        }
      }

      else if((cph == 'H') || (cph == 'V')) {
        if((cpt != 'H') && (cpt != 'V')) {
          std::swap(candidate_postfix[head], candidate_postfix[tail]);
          head = tail;
          ++tail;
          if(is_valid_postfix(candidate_postfix))
            std::cout << candidate_postfix << " is valid.\n";
          else
            std::cout << candidate_postfix << " is not valid.\n";
          continue;
        }
        else {
          ++head;
          ++tail;
        }
      }
      else {
        ++head;
        ++tail;
      }
    }
  }
  

  // update modules' positions
  void packing() {
    int cluster_id = 0;

    for(int i = 0; i < _postfix.size(); ++i) {
        
      // Horizontal cutline
      if((_postfix[i] == 'H') || (_postfix[i] == 'V')) {
        packing_cutline(_postfix[i]);
      }

      // integer idx 
      else {
        cluster_t cluster;
        int idx = _postfix[i] - '0';

        _modules[idx].llx = 0;
        _modules[idx].lly = 0;
        cluster.w = _modules[idx].w;
        cluster.h = _modules[idx].h;
        cluster.beg = cluster_id;
        cluster.end = cluster_id;

        ++cluster_id;

        _stack.push(cluster);

        //std::cout << cluster.w << " " << cluster.h << " " << cluster.beg << " " << cluster.end << '\n';
      }
    }

    _area = (_stack.top()).w * (_stack.top()).h;
    std::cout << "area  = " << _area << '\n';
  }


  void packing_cutline(char& cutline) {
    cluster_t cluster, cluster_r, cluster_l;
    
    cluster_r = _stack.top();
    _stack.pop();
    cluster_l = _stack.top();
    _stack.pop();

    cluster.beg = cluster_l.beg;
    cluster.end = cluster_r.end;

    // horizontal cutline
    if(cutline == 'H') {
      for(int i = cluster_r.beg; i <= cluster_r.end; ++i) {
        _modules[i].lly += cluster_l.h;
      }

      cluster.w = (cluster_l.w > cluster_r.w) ? cluster_l.w : cluster_r.w;
      cluster.h = cluster_l.h + cluster_r.h;
    }
    
    // vertical cutline
    else {
      for(int i = cluster_r.beg; i <= cluster_r.end; ++i) {
        _modules[i].llx += cluster_l.w; 
      }

      cluster.w = cluster_l.w + cluster_r.w;
      cluster.h = (cluster_l.h > cluster_r.h) ? cluster_l.h : cluster_r.h;
    }

    _stack.push(cluster);
  }


  // print out the read in modules
  void print_modules() {
    for(int i = 0; i < _modules.size(); ++i) {
      std::cout << "module[" << _modules[i].idx
                << "] has weight = " << _modules[i].w
                << " and height = "  << _modules[i].h
                << ", located at ("   << _modules[i].llx
                << ", " << _modules[i].lly
                << ")\n"; 
    }
  }

private:
  std::string _postfix = "31V20HH";
  std::vector<module_t> _modules;
  std::string _input_file;
  std::string _output_file;
  std::stack<cluster_t> _stack;
  int _area;
};




std::vector<module_t> read_modules(const std::string circuit_name) {
  std::ifstream infile(circuit_name, std::ios::in);
  
  if(!infile)
    std::cerr << "File could not be opened\n";
  
  int num_modules, weight, height, index;
  infile >> num_modules;

  std::vector<module_t> modules;

  while(infile >> index >> weight >> height) {
    module_t temp;
    temp.idx = index;
    temp.w = weight;
    temp.h = height;
    temp.llx = 0;
    temp.lly = 0;
    modules.push_back(temp);
  }
   
  return modules;  
}



int main(int argc, char* argv[]) {

  floorplan fp("./circuits/circuit1.txt", "./circuit1_sol.txt");
  fp.run();
  fp.print_modules();
  //std::cout << fp.is_valid_postfix();
  //fp.operand_swap();
  //fp.chain_invert();
  //fp.operator_operand_swap();
  return 0;
}
