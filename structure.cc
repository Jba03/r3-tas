#include "structure.hh"

namespace CPA {
  
  namespace Structure {
    
    std::string stEngineObject::familyName(ObjectNameResolver& resolve) {
      return resolve(objectFamilyName);
    }
    
    std::string stEngineObject::modelName(ObjectNameResolver& resolve) {
      return resolve(objectModelName);
    }
    
    std::string stEngineObject::instanceName(ObjectNameResolver& resolve) {
      return resolve(objectInstanceName);
    }
    
  };
  
};
