#ifndef serialize_hh
#define serialize_hh

#include "types.hh"

namespace CPA {
  
  struct serializer {
    template<typename T>
    void add(T v) {
      if constexpr (is_game_type<T>::value) {
        
      }
    }
  };
  
  struct serializable {
    //virtual void serialize(serializer& s);
  };
  
};


#endif /* serialize_hh */
