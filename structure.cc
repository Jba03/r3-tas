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
    
    std::string stSuperObject::typeName() {
      eSuperObjectType t = static_cast<eSuperObjectType>(uint32_t(type));
      if (stSuperObjectTypeNames.find(t) != stSuperObjectTypeNames.end()) {
        return stSuperObjectTypeNames[t];
      } else {
        return "";
      }
    }
    
    std::string stSuperObject::name(ObjectNameResolver& resolve, bool fullname) {
      switch (type) {
        case superObjectTypeActor: return actor->instanceName(resolve);
        case superObjectTypeIPO: return fullname ? ipo->name : ipo->name.lastPathComponent();
        default: return typeName();
      }
    }
    
  };
  
};
