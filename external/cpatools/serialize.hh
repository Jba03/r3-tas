#ifndef serialize_hh
#define serialize_hh

#include "types.hh"

#include <vector>
#include <cxxabi.h>

namespace CPA {
  
  struct serializer_node {
    serializer_node() {
      /* ... */
    }
    
    serializer_node(std::string name, std::string value) : name(name), value(value) {
      /* ... */
    }
    
    std::string name;
    std::string value;
    std::vector<serializer_node*> children;
    
    void add(serializer_node *nd) {
      children.push_back(nd);
    }
    
    template <typename Functor>
    static void forEach(Functor f, serializer_node *nd) {
      if (nd) {
        for (auto c : nd->children) {
          f(c);
          forEach(f, c);
        }
      }
    }
  };
  
  struct serializer {
    
  };
  
};


#endif /* serialize_hh */
