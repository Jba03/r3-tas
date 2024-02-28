#include "structure.hh"

namespace CPA {
  
  namespace Structure {
    
#pragma mark - stEngineStructure
    
    void stEngineStructure::loadLevel(std::string levelName) {
      nextLevelName = levelName;
      mode = eEngineMode::ChangeLevel;
    }
    
#pragma mark - stEngineObject
    
    std::string stEngineObject::name(ObjectNameResolver resolve, eObjectType type) {
      std::string name = "Invalid";
      std::array<int, 3> index { stdGame->familyType, stdGame->modelType, stdGame->instanceType };
      int t = static_cast<int>(type);
      while (name == "Invalid" && t >= 0) {
        name = resolve(static_cast<eObjectType>(t), &index[t]);
        t--;
      }
      
      return name;
    }
    
    std::string stEngineObject::familyName(ObjectNameResolver resolve) {
      return name(resolve, objectFamilyName);
    }
    
    std::string stEngineObject::modelName(ObjectNameResolver resolve) {
      return name(resolve, objectModelName);
    }
    
    std::string stEngineObject::instanceName(ObjectNameResolver resolve) {
      return name(resolve, objectInstanceName);
    }
    
    pointer<stAIModel> stEngineObject::aiModel() {
      return brain->mind->aiModel;
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
    
    std::string stSuperObject::name(ObjectNameResolver resolve, bool fullname) {
      switch (type) {
        case superObjectTypeActor: return actor->instanceName(resolve);
        case superObjectTypeIPO: return fullname ? ipo->name : ipo->name.lastPathComponent();
        case superObjectTypeSector: return fullname ? sector->name : sector->name.lastPathComponent();
        default: return typeName();
      }
    }
    
  };
  
};
