//
//  game.h
//  r3-tas
//
//  Created by Jba03 on 2022-12-19.
//

#ifndef game_h
#define game_h

#include <cpatools/cpatools.hpp>

using namespace cpa;
using namespace cpa::structure;
using namespace cpa::global;

namespace game {
    
void initialize();
void update();
void deinitialize();
void level_read();

/** find an object by instance name*/
pointer<stSuperObject> findObject(std::string instanceName);
/** get object type color */
uint32_t objectColor(stSuperObject *object);

/** look up input structure element */
pointer<stInputEntryElement> findInputEntryElement(std::string name);

/** is the engine in a valid state? */
bool isValidGameState();
/** did the engine mode change? */
bool engineModeChangedTo(int mode, int from = engineModeInvalid);
  
}

#endif /* game_h */
