#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>
#include <ctime>
#include <cstdlib>


#define FROZEN 0.1



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

    for(int i = 0; i < _modules.size(); ++i)
      _mapping.push_back(i);
    /*
    // an initial normalized polish expression
    for(int i = 0; i < _modules.size(); ++i) {
      if(i == 0) { 
        _postfix.push_back(_modules[i].idx);
        continue;
      }
      if(i%2 == 1) {
        _postfix.push_back(_modules[i].idx);
        _postfix.push_back('V');
        continue;
      }
      if(i%2 == 0) {
        _postfix.push_back(_modules[i].idx);
        _postfix.push_back('H');
        continue;
      }
    }*/
  }  


  // execute and generate an output file and its json
  void run() {
    int area = packing(_postfix);
   
    optimize();
    area = packing(_postfix);
    
    std::ofstream outfile(_output_file, std::ios::out);

    if(!outfile)
      std::cerr << "File could not be opened for writing\n";

    outfile << "0 0 " << _urx << " " << _ury << '\n';
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
                 << ",\"urx\":" << _urx
                 << ",\"ury\":" << _ury
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


  // M1: swap two adjacent operands
  std::string operand_swap(const std::string& postfix_curr) {
    //std::srand(std::time(nullptr));
    int head, tail;
    std::string postfix_prop = postfix_curr;


    while(1) {
      head = (std::rand()) % (postfix_prop.size()-1);
      tail = head + 1;
      
      char pph = postfix_prop[head];

      if((pph != 'H') && (pph != 'V')) {
        while(tail < postfix_prop.size()) {
          char ppt = postfix_prop[tail];
          if((ppt != 'H') && (ppt != 'V')) {
            std::swap(postfix_prop[head], postfix_prop[tail]);
            return postfix_prop;
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
  std::string complement_cutline(const std::string& postfix_curr) {
    //std::srand(std::time(nullptr));
    int head;
    std::string postfix_prop = postfix_curr;

    while(1) {
      head = (std::rand()) % (postfix_prop.size()-1);
      char pph = postfix_prop[head];

      if(pph == 'H') {
        postfix_prop[head] = 'V';
        return postfix_prop;
      }

      else if(pph == 'V') {
        postfix_prop[head] = 'H';
        return postfix_prop;
      }

      else
        continue;
    }
  }
 

  // M3 : complement last pair of two cutline
  std::string complement_last2cutline(const std::string& postfix_curr) {
    std::string postfix_prop = postfix_curr;
  
    for(int i = postfix_prop.size()-1; i > 0; --i) {
      if((postfix_prop[i] == 'H') && postfix_prop[i-1] == 'V') {
        postfix_prop[i] = 'V';
        postfix_prop[i-1] = 'H';
        return postfix_prop; 
      }

      if((postfix_prop[i] == 'V') && (postfix_prop[i-1] == 'H')) {
        postfix_prop[i] = 'H';
        postfix_prop[i-1] = 'V';
        return postfix_prop;
      }
    }
      return {};
  }


  // M4 : swap two adjacent operand and operator
  std::string operator_operand_swap(const std::string& postfix_curr) {
    //std::srand(std::time(nullptr));
    int head, tail;
    std::string postfix_prop = postfix_curr;

    while(1) {
      head = (std::rand()) % (postfix_prop.size()-2);
      tail = head + 1;
      
      char pph = postfix_prop[head];
      char ppt = postfix_prop[tail];

      if((pph == 'H') || (pph == 'V')) {
        if((ppt != 'H') && (ppt != 'V')) {
          std::swap(postfix_prop[head], postfix_prop[tail]);
          if(is_valid_postfix(postfix_prop) == false)
            continue;
          else
            return postfix_prop;
        }
        else
          continue;
      }

      if((pph != 'H') && (pph != 'V')) {
        if((ppt == 'H') || (ppt == 'V')) {
          std::swap(postfix_prop[head], postfix_prop[tail]);
          if(is_valid_postfix(postfix_prop) == false)
            continue;
          else
            return postfix_prop;
        }
        else
          continue;
      }
    }
  }
  
  // M5 : complement first pair of two cutline
  std::string complement_first2cutline(const std::string& postfix_curr) {
    std::string postfix_prop = postfix_curr;
  
    for(int i = 0; i < postfix_prop.size()-1; ++i) {
      if((postfix_prop[i] == 'H') && postfix_prop[i+1] == 'V') {
        postfix_prop[i] = 'V';
        postfix_prop[i+1] = 'H';
        return postfix_prop; 
      }

      if((postfix_prop[i] == 'V') && (postfix_prop[i+1] == 'H')) {
        postfix_prop[i] = 'H';
        postfix_prop[i+1] = 'V';
        return postfix_prop;
      }
    }
      return {};
  }


  // update modules' positions
  int packing(const std::string& postfix) {
    while(!_stack.empty())
      _stack.pop();

    int cluster_id = 0;

    for(int i = 0; i < postfix.size(); ++i) {
        
      // Horizontal cutline
      if((postfix[i] == 'H') || (postfix[i] == 'V')) {
        packing_cutline(postfix[i]);
      }

      // integer idx 
      else {
        cluster_t cluster;
        int idx = postfix[i] - '0';
        
        _modules[idx].llx = 0;
        _modules[idx].lly = 0;
        cluster.w = _modules[idx].w;
        cluster.h = _modules[idx].h;
        cluster.beg = cluster_id;
        cluster.end = cluster_id;

        _mapping[cluster_id] = idx;
        ++cluster_id;

        _stack.push(cluster);

        //std::cout << cluster.w << " " << cluster.h << " " << cluster.beg << " " << cluster.end << '\n';
      }
    }

    _area = (_stack.top()).w * (_stack.top()).h;
    _urx = (_stack.top()).w;
    _ury = (_stack.top()).h;

    return _area;
  }


  void packing_cutline(const char& cutline) {
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


  std::string generate_neighbor(const std::string& postfix_curr) {
    std::string postfix_prop;
    //std::srand(std::time(nullptr));

    std::cout << "generate neighbor\n";

    while(1) {
      switch(std::rand()%5) {
        case 0:
          std::cout << "random 0\n";
          postfix_prop = operand_swap(postfix_curr);
          break;
        case 1:
          std::cout << "random 1\n";
          postfix_prop = complement_cutline(postfix_curr);
          break;
        case 2:
          std::cout << "random 2\n";
          postfix_prop = complement_last2cutline(postfix_curr);
          if(postfix_prop.empty())
            continue;
          break;
        case 3:
          std::cout << "random 3\n";
          postfix_prop = operator_operand_swap(postfix_curr);
          break;
        case 4:
          std::cout << "random 4\n";
          postfix_prop = complement_first2cutline(postfix_curr);
          if(postfix_prop.empty())
            continue;
          break;
      }
      break;
    }
    std::cout << postfix_prop << '\n';
    return postfix_prop;
  }


  // perform optimization
  void optimize() {
    double temperature = 100.0;

    std::string postfix_prop;
    std::string postfix_curr;
    std::string postfix_best;   
    
    //curr = generate_initial_solution(); // 12V3H4V5H6V...
    
    postfix_curr = _postfix;
    postfix_best = postfix_curr;
    
    int area_best = packing(postfix_best);
    int area_curr = area_best;
          
    std::random_device rd;
    std::mt19937 gen(rd());  // expensive - typically construct once
    std::uniform_real_distribution<> dis(0, 1);
    
    while(temperature > FROZEN) {
      
      for(int iter = 0; iter < 10; iter++) {
        
        postfix_prop = generate_neighbor(postfix_curr);
        
        int area_prop = packing(postfix_prop);
        int cost = area_prop - area_curr;
        
        if(cost < 0) {
          postfix_curr = postfix_prop;
          if(area_prop < area_best) {
            postfix_best = postfix_prop;
            area_best = area_prop;
          }
        }

        else {
          auto prob = std::exp(-cost / temperature); 
          if(prob > dis(gen)) {
            postfix_curr = postfix_prop; 
          }
        }
      }
      temperature *= 0.95;  
    }

    _postfix = postfix_best;
  }
  


private:
  std::string _postfix = "01V2H3V4H5V";
  //std::string _postfix = "3145V02VVHV";
  std::vector<module_t> _modules;
  std::string _input_file;
  std::string _output_file;
  std::stack<cluster_t> _stack;
  int _area, _urx, _ury;
  std::vector<int> _mapping;
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

  floorplan fp("./circuits/circuit2.txt", "./circuit2_sol.txt");
  fp.run();
  //fp.print_modules();
  //std::cout << fp.is_valid_postfix();
  //std::cout << fp.operand_swap() << '\n';
  //std::cout << fp.complement_cutline() << '\n';
  //std::cout << fp.complement_first2cutline() << '\n';
  //fp.chain_invert();
  //std::cout << fp.operator_operand_swap() << '\n';
  return 0;
}
