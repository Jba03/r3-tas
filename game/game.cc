//
//  game.c
//  r3-tas
//
//  Created by Jba03 on 2022-12-19.
//

#include <iostream>
#include <map>
#include <string>

#include "constants.hh"
#include "game.hh"
#include "log.hh"
#include "tools.hh"

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b
#define UNIQUE_NAME(base) CONCAT(base, __LINE__)
#define padding(S) uint8_t UNIQUE_NAME(padding) [S];

const uint8_t * memoryBase = NULL;

namespace library::memory {
  const uint8_t *baseAddress = nullptr;
}

namespace library::external {
  std::function<std::string(int, int)> nameLookup = nullptr;
};

static const std::vector<uint32_t> color_table = {
  0xc020ff20,
  0xc0ff5050,
  0xc020a0ff,
  0xc0ff20ff,
  0xc010af10,
  0xc020f0ff,
  0xc0ff00a0,
  0xc04020b0,
  0xc0d0ff20,
};

namespace game
{
  /* Globals */
  pointer<stAlways> g_stAlways = nullptr;
  pointer<stEngineStructure> g_stEngineStructure = nullptr;
  pointer<stObjectType> g_stObjectTypes = nullptr;
  pointer<stInputStructure> g_stInputStructure = nullptr;
  pointer<stRandom> g_stRandomStructure = nullptr;
  
  /* Global variables */
  uint8 *g_bGhostMode = nullptr;
  
  /* World */
  stSuperObject *p_stActualWorld;
  stSuperObject *p_stDynamicWorld;
  stSuperObject *p_stInactiveDynamicWorld;
  stSuperObject *p_stFatherSector;
  
  std::map<std::string, stSuperObject*> objectLookupCache;
  
  struct namecache {
    std::vector<std::string> familyNames;
    std::vector<std::string> modelNames;
    std::vector<std::string> instanceNames;
  };
  
  std::map<std::string, namecache> objectNameCache;
    
  static eEngineMode lastEngineMode = Invalid;
  
#pragma mark - FIX
    
    struct fix_header {
      padding(32)
      pointer<> identity_matrix;
      pointer<> localization_structure;
      uint32 level_name_count;
      uint32 demo_name_count;
    };
    
    struct fix_trailer {
      char8 first_level[30];
      padding(2)
      uint32 language_count;
      uint32 language_offset;
      uint32 texture_count;
    };
    
    struct fix {
      struct fix_header* header;
      struct fix_trailer* trailer;
    };
    
#pragma mark - LVL
    
    struct lvl_header {
      padding(4 * 4) /* ? */
      char8 text[24];
      padding(4 * 60) /* ? */
      uint32 texture_count;
    };
    
    struct lvl_section_a {
      pointer<> actual_world;
      pointer<> dynamic_world;
      pointer<> inactive_dynamic_world;
      pointer<> father_sector;
      pointer<> first_submap_position;
      stAlways always_structure;
      stObjectType object_type;
    };
    
    struct lvl {
      struct lvl_header* header;
      struct lvl_section_a* section_a;
    };
    
    /* FIX: fixed memory */
    struct fix fix;
    /* LVL: level memory */
    struct lvl lvl;
    
#pragma mark - Engine
    
    /* */
    uint8_t previous_engine_mode = 0;
    char previous_level_name[30];
    unsigned transition_frame = 0;
    
    void readLevel() {
      doublepointer<> fixptr(GCN_POINTER_FIX);
      doublepointer<> lvlptr(GCN_POINTER_LVL);
      
      if (!fixptr || !lvlptr) return;
      
      #pragma mark FIX
      {
            //info(BOLD COLOR_GREEN "FIX @ [0x%X : %p]\n", fixptr.offset(), fixptr.realAddress());
            fix.header = fixptr;
        
            const unsigned char* offset = (const unsigned char*)(fix.header + 1);
            /* Skip demo save names */
            offset += 12 * fix.header->demo_name_count;
            /* Skip demo level names */
            offset += 12 * fix.header->demo_name_count;
            /* Skip level names (these are derived from the engine struct) */
            offset += 30 * fix.header->level_name_count;
            /* Trailer */
            fix.trailer = (fix_trailer*)offset;
        }
        
#pragma mark LVL
        {
            //info(BOLD COLOR_GREEN "LVL @ [0x%X : %p]\n", lvlptr->physicalAddress(), lvlptr->hostAddress());
            
            lvl.header = (struct lvl_header*)lvlptr;
            
            const unsigned char* offset = (const unsigned char*)(lvl.header + 1);
            /* Calculate total texture count: this is the number of textures in the level aside (duplicates?) in fixed memory */
            const uint32_t n_textures = lvl.header->texture_count - fix.trailer->texture_count;
            /* Skip textures */
            offset += n_textures * 4 * 2;
            /* Read dynamically aligned section 1 */
            lvl.section_a = (lvl_section_a*)offset;
          
            g_stObjectTypes = (stObjectType*)&lvl.section_a->object_type;
            g_stAlways = (stAlways*)&lvl.section_a->always_structure;
        }
    }
  
  static auto cache() -> void {
    if (objectNameCache.find(g_stEngineStructure->currentLevelName) == objectNameCache.end()) {
      namecache& cache = objectNameCache[g_stEngineStructure->currentLevelName];
      
      g_stObjectTypes->family.forEach([&](stObjectTypeElement* e, void*) { cache.familyNames.push_back(std::string(e->name)); });
      g_stObjectTypes->model.forEach([&](stObjectTypeElement* e, void*) { cache.modelNames.push_back(std::string(e->name)); });
      g_stObjectTypes->instance.forEach([&](stObjectTypeElement* e, void*) { cache.instanceNames.push_back(std::string(e->name)); });
    }
  }
  
  static auto nameLookup(int type, int idx) -> std::string {
    if (objectNameCache.find(g_stEngineStructure->currentLevelName) != objectNameCache.end()) {
      namecache& cache = objectNameCache[g_stEngineStructure->currentLevelName];
      try {
        if (type == objectFamilyName) return cache.familyNames.at(idx);
        if (type == objectModelName) return cache.modelNames.at(idx);
        if (type == objectInstanceName) return cache.instanceNames.at(idx);
      } catch (std::out_of_range& e) {
        //std::cout << "could not locate name (idx=" << idx  << ") out of range\n";
        return "Invalid name";
      }
    }
    return "Invalid name";
  }
  
  std::string nameResolver(eObjectType type, int *index) {
    return nameLookup(type, *index);
  }
    
  void update() {
    g_stEngineStructure = pointer<stSuperObject>    (GCN_POINTER_ENGINE_STRUCTURE);
    g_stInputStructure  = pointer<stInputStructure> (GCN_POINTER_INPUT_STRUCTURE);
    g_stRandomStructure = pointer<stRandom>         (GCN_POINTER_RND);
    
    p_stActualWorld          = doublepointer<stSuperObject>(GCN_POINTER_ACTUAL_WORLD);
    p_stDynamicWorld         = doublepointer<stSuperObject>(GCN_POINTER_DYNAMIC_WORLD);
    p_stInactiveDynamicWorld = doublepointer<stSuperObject>(GCN_POINTER_INACTIVE_DYNAMIC_WORLD);
    p_stFatherSector         = doublepointer<stSuperObject>(GCN_POINTER_FATHER_SECTOR);
    
    g_bGhostMode = pointer<uint8>(GCN_POINTER_GHOST_MODE);
    
    if (isValidGameState()) {
      readLevel();
      cache();
    }
    
    R3::autoSplitter.update();
    
    lastEngineMode = g_stEngineStructure->mode;
  }
  
  void initialize() {
    library::external::nameLookup = nameLookup;
  }
    
  uint32_t color_table_index(unsigned idx) {
    return color_table[idx % 9];
  }
    
  uint32_t objectColor(stSuperObject *object) {
    if (!object) return 0xAA808080;
    if (object->type == superObjectTypeIPO) return 0xFF00AAFF;
    if (object->type == superObjectTypeIPOMirror) return 0xFF00DDFF;
    if (object->type != superObjectTypeActor) return 0xAAFFFFFF;
    stEngineObject *actor = object->data;
    if (!actor->stdGame) return 0x80808080;
    return color_table_index(2 * actor->stdGame->familyType + 1);
    return 0;
  }
    
  void game_unload() {
    fix.header = NULL;
    fix.trailer = NULL;
    lvl.header = NULL;
    lvl.section_a = NULL;
    g_stAlways = NULL;
    g_stObjectTypes = NULL;
  }
    
//    stSuperObject *findActor(std::string instanceName) {
//        if (actorLookupCache.contains(instanceName))
//            if (actorLookupCache.at(instanceName))
//                actorLookupCache.at(instanceName);
//        
//        return p_stActualWorld->find(instanceName, g_stObjectTypes);
//    }
    
  bool isValidGameState() {
    if (!g_stEngineStructure) return false;
    return !g_stEngineStructure->engineFrozen
    && g_stEngineStructure->mode != 5
    && g_stEngineStructure->mode != 6
    && p_stActualWorld
    && p_stDynamicWorld
    && p_stInactiveDynamicWorld
    && p_stFatherSector;
  }
  
  bool engineModeChangedTo(eEngineMode mode, eEngineMode from) {
    return (from == eEngineMode::Invalid) ?
    (g_stEngineStructure->mode == mode) && (mode != lastEngineMode) :
    (g_stEngineStructure->mode == mode) && (mode != lastEngineMode) && (lastEngineMode == from);
  }
    
}
