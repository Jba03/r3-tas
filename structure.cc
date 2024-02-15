#include "structure.hh"

namespace CPA {
  
  namespace Structure {
    
#pragma mark - stEngineObject
    
    std::string stEngineObject::familyName(ObjectNameResolver& resolve) {
      return resolve(objectFamilyName);
    }
    
    std::string stEngineObject::modelName(ObjectNameResolver& resolve) {
      return resolve(objectModelName);
    }
    
    std::string stEngineObject::instanceName(ObjectNameResolver& resolve) {
      return resolve(objectInstanceName);
    }
    
    
#pragma mark - stSuperObject
    
    std::string stSuperObject::name() {
      switch(type) {
//        case superObjectTypeActor:
//          return actor->
      }
      return "";
    }
    
  };
  
};
