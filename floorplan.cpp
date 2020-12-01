#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>



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

    //std::string _output_file_json = _output_file;
    //_output_file_json.replace(_output_file_json.end()-3, 
    //                          _output_file_json.end(), "json");


  }

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
  std::string _postfix = "01V23HH";
  std::vector<module_t> _modules;
  std::string _input_file;
  std::string _output_file;
  std::stack<cluster_t> _stack;
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
  //fp.run();
  //fp.print_modules();
  std::cout << fp.is_valid_postfix();
  return 0;
}
