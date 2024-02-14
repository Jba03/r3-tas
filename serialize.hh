#ifndef serialize_hh
#define serialize_hh

namespace CPA {
  
  struct serializer {
    template<typename T>void add(T v) {
      
    }
  };
  
  struct serializable {
    //virtual void serialize(serializer& s);
  };
  
};


#endif /* serialize_hh */
