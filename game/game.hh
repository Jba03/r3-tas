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
    
  void initialize();
  void update();
  void deinitialize();
  void level_read();
  
  /** find an object by instance name*/
  pointer<stSuperObject> findObject(std::string instanceName);
  /** get object type color */
  uint32_t objectColor(stSuperObject *object);
  /** resolve object name */
  std::string nameResolver(eObjectType type, int *index);
  
  /** look up input structure element */
  pointer<stInputEntryElement> findInputEntryElement(std::string name);
  
  /** is the engine in a valid state? */
  bool isValidGameState();
  /** did the engine mode change? */
  bool engineModeChangedTo(eEngineMode mode, eEngineMode from = eEngineMode::Invalid);
  
}

#endif /* game_h */
