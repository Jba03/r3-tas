//
//  game.h
//  r3-tas
//
//  Created by Jba03 on 2022-12-19.
//

#ifndef game_h
#define game_h

#include <vector>

#include "structure.hh"
using namespace library;

namespace game {
  
  /* Global structures */
  extern stAlways *g_stAlways;
  extern stEngineStructure *g_stEngineStructure;
  extern stObjectType *g_stObjectTypes;
  extern stInputStructure *g_stInputStructure;
  extern stRandom *g_stRandomStructure;
    
  /* World */
  extern stSuperObject *p_stActualWorld;
  extern stSuperObject *p_stDynamicWorld;
  extern stSuperObject *p_stInactiveDynamicWorld;
  extern stSuperObject *p_stFatherSector;
    
  /* Global variables */
  extern uint8 *g_bGhostMode;
    
    
  /* */
  extern uint8_t previous_engine_mode;
  extern char previous_level_name[30];
  extern unsigned transition_frame;
  /* just_entered_mode: true if engine mode differs from previous frame */
  #define just_entered_mode(m) ((engine->mode == m) && (previous_engine_mode != m))
    
  void level_read();
    
  void initialize();
  void update();
  void deinitialize();
    
  stSuperObject *findObject(std::string instanceName);
  uint32_t objectColor(stSuperObject *object);
    
  bool isValidGameState();
  
  
  void setEngineLoopIterations(int i);
}

#endif /* game_h */
