//
//  debugger.cpp
//  r3-tas
//
//  Created by Jba03 on 2024-01-12.
//

#include "debugger.hh"
#include "hook.hh"

#include <fstream>

void Debugger::Process() {
//  Symbol s;
//  s.name = "hi!";
//  s.address = PC;
//  tree.insert(s);
}


static Debugger *CONTEXT = NULL;

static void process() {
//  Debugger::Symbol s;
//  s.name = CONTEXT->SymbolForAddress(PC).name;
//  s.address = PC;
//  
//  if (s.name.length() > 5)
//    CONTEXT->tree.push_back(s);
}

Debugger::Debugger(std::string map_filename) {
  std::ifstream s(map_filename);
  if (!s.is_open())
    return;
  
  CONTEXT = this;
  
  std::string line;
  while (std::getline(s, line)) {
    unsigned addr, ext, addr2, align;
    char buf[256];
    sscanf(line.c_str(), "  %x %x %x  %i %s\n", &addr, &ext, &addr2, &align, buf);
    
    if (!strncmp(buf, "fn_", 3)) {
      
      Symbol s;
      s.address = addr;
      s.name = buf;
      symbol_map[addr] = s;
      
      ///printf("loading symbol: %s\n",buf);
      
      hookDynamic(addr, &process);
    }
  }
  
  s.close();
}

Debugger::Symbol Debugger::SymbolForAddress(library::memory::address_type address) {
  return symbol_map[address];
}

void Debugger::NewFrame() {
  tree.clear();
}
