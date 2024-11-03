//
//  game.c
//  r3-tas
//
//  Created by Jba03 on 2022-12-19.
//

#include <iostream>
#include <map>
#include <string>
#include <fstream>

#include "interface.hh"
#include "constants.hh"
#include "game.hh"
#include "log.hh"
#include "tools.hh"

#include "mecsync.hh"

#include "gui.hh"
#include "run-manager.hh"

#define CPATOOLS_IMPLEMENTATION
#include <cpatools/cpatools.hpp>

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b
#define UNIQUE_NAME(base) CONCAT(base, __LINE__)
#define padding(S) uint8_t UNIQUE_NAME(padding) [S];

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
  std::map<std::string, stSuperObject*> objectLookupCache;
  std::map<std::string, pointer<stInputEntryElement>> inputEntryElementCache;
  
  struct namecache {
    std::vector<std::string> familyNames;
    std::vector<std::string> modelNames;
    std::vector<std::string> instanceNames;
  };
  
  std::map<std::string, namecache> objectNameCache;
    
  static uint8 lastEngineMode = engineModeInvalid;
static std::string lastLevelName = "";
  
  static void cacheInputEntries() { // b24460
    //printf("entries: %X\n", g_stInputStructure->entries.memoryOffset().effectiveAddress());
    try {
      for (int i = 0; i < g_stInputStructure->numEntries; i++) {
        pointer<stInputEntryElement> element = &g_stInputStructure->entries[i];
        if (element->actionName) inputEntryElementCache[std::string(element->actionName)] = element;
      }
    } catch (...) {
      
    }
  }
  
  pointer<stInputEntryElement> findInputEntryElement(std::string name) {
    if (inputEntryElementCache.find(name) != inputEntryElementCache.end())
      return inputEntryElementCache[name];
    return nullptr;
  }
    
  void update() {
    cpa::global::load();
    
    if (isValidGameState()) {
      cacheInputEntries();
      
//      serializer s(serializer::Mode::Save);
//      serializer::node nd(&s);
//      nd.pointer("root", p_stDynamicWorld);
//      std::string data = nd.data.dump(1);
//      //printf("%s\n", nd.data.dump(2).c_str());
//      
//      FILE* fp = fopen("/Users/jba03/Desktop/Music/YouTube/out.json", "wb");
//      fwrite(data.c_str(), data.length(), 1, fp);
//      fclose(fp);
      
      
     // dynamics.serialize(nd);
      
      //printf("adr: %X\n", g_stEngineStructure->currentMainPlayers[0]->actor->brain->mind->runIntelligence.memoryOffset().effectiveAddress());
      
//      *(uint8_t*)g_stEngineStructure->currentMainPlayers[0]->actor->dsgVar(0) = 4;
//      *(uint8_t*)g_stEngineStructure->currentMainPlayers[0]->actor->dsgVar(1) = 1;
//      *(uint8_t*)g_stEngineStructure->currentMainPlayers[0]->actor->dsgVar(4) = 1;
      //g_stEngineStructure->standardCamera->actor->brain->mind->intelligence = nullptr;
      
//      serializer s;
//      serializer::node nd(&s, "root", "");
//      p_stActualWorld->serialize(nd);
//
//      std::string ser = serializer::node::serialize(nd);
//
//      FILE* fp = fopen("out.serialize", "wb");
//      fwrite(ser.c_str(), ser.length(), 1, fp);
//      fclose(fp);
      
//      serializer s(serializer::Load);
//      serializer::node nd(&s, "root", "");
//
//      FILE* fp = fopen("/Users/jba03/Desktop/Music/YouTube/out.serialize", "rb");
//
//      fseek(fp, 0, SEEK_END);
//      size_t sz = ftell(fp);
//      fseek(fp, 0, SEEK_SET);
//      char* buf = (char*)malloc(sizeof(char) * sz);
//      fread(buf, sz, 1, fp);
//
//      fclose(fp);
//
//      nd.unserialize(buf);
      //p_stActualWorld->serialize(nd);

      

//            std::string ser = serializer::node::serialize(nd);
//      printf("a: %s\n", ser.c_str());
      
      //printf("string:\n%s\n\n", );
      
     // g_stEngineStructure->inputMode = engineInputModeCommands;
//      g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeX].analogValue = float((drand48() - 0.5f) * 200.0f);
//      g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeY].analogValue = float((drand48() - 0.5f) * 200.0f);
      //g_stInputStructure->device[0].joyAxisR->analogValue = drand48();
    }
      
    
  
    if (g_stEngineStructure->mode != lastEngineMode) {
      event("EngineModeChanged").fire({{"from", lastEngineMode}, {"to", g_stEngineStructure->mode}});
    }
    
    if (std::string(g_stEngineStructure->currentLevelName) != lastLevelName) {
      //event("LevelChanged").subscribe("GUI", [&](Event::Param& p) {
      std::string currentLevel = g_stEngineStructure->currentLevelName; //std::any_cast<std::string>(p["currentLevel"]);
      std::string previousLevel = lastLevelName; //std::any_cast<std::string>(p["previousLevel"]);
        printf("level change: %s -> %s\n", previousLevel.c_str(), currentLevel.c_str());
//      if (currentLevel.length() && previousLevel.length()) {
//        gui::saveLayout(previousLevel);
//        gui::loadLayout(currentLevel);
//      }
      //});
      event("LevelChanged").fire({{"current", std::string(g_stEngineStructure->currentLevelName)}, {"previousLevel", lastLevelName}});
    }
    
    
    //printf("last: %d\n", lastEngineMode);
    
    R3::autoSplitter.update();
    
    runManager.Update();
    
    lastLevelName = std::string(g_stEngineStructure->currentLevelName);
    lastEngineMode = g_stEngineStructure->mode;
  }
  
  void initialize() {
   // library::external::nameLookup = nameLookup;
   // mecsync_start();
  }
    
  uint32_t color_table_index(unsigned idx) {
    return color_table[idx % 9];
  }
    
  uint32_t objectColor(stSuperObject *object) {
    if (!object) return 0xAA808080;
    if (object->type == superobjectTypeIPO) return 0xFF00AAFF;
    if (object->type == superobjectTypeIPOMirror) return 0xFF00DDFF;
    if (object->type != superobjectTypeActor) return 0xAAFFFFFF;
    stEngineObject *actor = object->data;
    if (!actor->stdGame) return 0x80808080;
    return color_table_index(2 * actor->stdGame->familyType + 1);
    return 0;
  }
    
  void game_unload() {
//    fix.header = NULL;
//    fix.trailer = NULL;
//    lvl.header = NULL;
//    lvl.section_a = NULL;
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
  
  pointer<stSuperObject> findObject(std::string instanceName) {
    if (!isValidGameState()) return nullptr;
    pointer<stSuperObject> target = nullptr;
    try {
      p_stActualWorld->recurse([&](stSuperObject *obj, void *data) {
        if (obj->name() == instanceName) {
          target = obj;
        }
      }, nullptr);
    } catch (...) {
      /* ... */
    }
    return target;
  }
    
  bool isValidGameState() {
    return cpa::global::isValidState();
  }
  
  bool engineModeChangedTo(int mode, int from) {
    //printf("check change: %d %d %d\n", mode, g_stEngineStructure->mode, lastEngineMode);
    //return (g_stEngineStructure->mode == mode) && (mode != lastEngineMode);
    return (from == engineModeInvalid) ?
    (g_stEngineStructure->mode == mode) && (mode != lastEngineMode) :
    (g_stEngineStructure->mode == mode) && (mode != lastEngineMode) && (lastEngineMode == from);
  }
    
}
