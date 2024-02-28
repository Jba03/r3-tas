//
//  game.h
//  r3-tas
//
//  Created by Jba03 on 2022-12-19.
//

#ifndef game_h
#define game_h

#include "cpa.hh"
using namespace CPA;
using namespace CPA::Structure;

namespace game {
  
  /* Global structures */
  extern pointer<stAlways> g_stAlways;
  extern pointer<stEngineStructure> g_stEngineStructure;
  extern pointer<stObjectType> g_stObjectTypes;
  extern pointer<stInputStructure> g_stInputStructure;
  extern pointer<stRandom> g_stRandomStructure;
  
  /* World */
  extern stSuperObject *p_stActualWorld;
  extern stSuperObject *p_stDynamicWorld;
  extern stSuperObject *p_stInactiveDynamicWorld;
  extern stSuperObject *p_stFatherSector;
    
  /* Global variables */
  extern CPA::uint8 *g_bGhostMode;
    
    
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
  
  std::string nameResolver(eObjectType type, int *index);
    
  bool isValidGameState();
  
  bool engineModeChangedTo(eEngineMode mode, eEngineMode from = eEngineMode::Invalid);
  
  void setEngineLoopIterations(int i);
}

#endif /* game_h */
