#ifndef stStandardGameInfo_h
#define stStandardGameInfo_h

#include "structure.h"

typedef struct stStandardGameInfo tdstStandardGameInfo;

struct stStandardGameInfo
{
    readonly int32 familyType;
    readonly int32 modelType;
    readonly int32 instanceType;
    readonly pointer superObject;
    readonly uint8 initialFlag;
    readonly uint8 flag1;
    readonly uint8 flag2;
    readonly padding(2)
    readonly uint32 lastFrame;
    readonly uint32 capabilities;
    readonly uint8 tractionFactor;
    readonly uint8 hitPoints;
    readonly uint8 maxHitPoints;
    readonly uint8 maxHitPointsMax;
    readonly uint32 customBits;
    readonly uint32 aiCustomBits;
    readonly uint8 platformType;
    readonly uint8 miscFlags;
    readonly uint8 transparencyZoneMin;
    readonly uint8 transparencyZoneMax;
    readonly uint32 initialCustomBits;
    readonly uint32 aiInitialCustomBits;
    readonly uint8 initialHitPoints;
    readonly uint8 maxInitialHitPoints;
    readonly uint8 initialMiscFlags;
    readonly uint8 tooFarLimit;
    readonly uint8 importance;
    readonly uint8 optional;
    
    readonly padding(2)
    
    /* :: custom values :: */
};

/**
 * stdgame_get_superobject:
 *  Get the superobject whose actor holds this info.
 */
#define stdgame_get_superobject(stdgame) (stdgame ? ((tdstSuperObject*)pointer(stdgame->superObject)) : NULL)

#define stdgame_family_type(stdgame) host_byteorder_32(stdgame->familyType)
#define stdgame_model_type(stdgame) host_byteorder_32(stdgame->modelType)
#define stdgame_instance_type(stdgame) host_byteorder_32(stdgame->instanceType)

#endif /* stStandardGameInfo_h */
