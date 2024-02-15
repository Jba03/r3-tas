//
//  debugger.hpp
//  r3-tas
//
//  Created by Jba03 on 2024-01-12.
//

#ifndef debugger_hpp
#define debugger_hpp

#include "structure.hh"

struct Debugger {
  
  Debugger(std::string map_filename);
  
  struct Symbol {
    library::memory::address address;
    std::string name;
  };
  
  struct CallTree {
    
  };
  
  CallTree GetCallTree();
  
  void NewFrame();
  
  Symbol SymbolForAddress(library::memory::address_type address);
  
  /* current calltree */
  std::vector<Symbol> tree;
  
private:
  /* map of symbols loaded */
  std::map<library::memory::address_type, Symbol> symbol_map;
  /* debugger function */
  void Process();
};

#endif /* debugger_hpp */
