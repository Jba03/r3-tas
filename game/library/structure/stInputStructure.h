#ifndef stInputStructure_h
#define stInputStructure_h

#include "structure.h"
#include "stVector3D.h"

typedef struct stPadReadingOutput tdstPadReadingOutput;
typedef struct stInputDevice tdstInputDevice;
typedef struct stInputStructure tdstInputStructure;
typedef struct stInputEntryElement tdstInputEntryElement;
typedef struct stInputStructure tdstInputStructure;
typedef struct stInputEntryElement tdstInputEntryElement;

struct stPadReadingOutput
{
    readonly tdstVector3D vector;
    readonly int16 horizontalAxis;
    readonly int16 verticalAxis;
    readonly float32 analogForce;
    readonly float32 trueAnalogForce;
    readonly float32 angle;
    readonly int32 sector;
};

struct stInputDevice
{
    readonly uint8 valid;
    readonly padding(3)
    readonly pointer handle;
    readonly uint8 joypadCounter[44];
    readonly int8 joyMinX;
    readonly int8 joyMaxX;
    readonly int8 joyMinY;
    readonly int8 joyMaxY;
    readonly int8 joyCenterX;
    readonly int8 joyCenterY;
    readonly padding(2)
    readonly pointer joyAxisX;
    readonly pointer joyAxisY;
    readonly pointer joyAxisZ;
    readonly pointer joyAxisR;
    readonly pointer keyUp;
    readonly pointer keyDown;
    readonly pointer keyLeft;
    readonly pointer keyRight;
    readonly pointer keySpeed;
    readonly uint8 noKeyboardInertia;
    readonly padding(3)
    readonly pointer joyButton[16];
    readonly pointer keyButton[16];
    readonly tdstPadReadingOutput pad;
};

struct stInputStructure
{
    readonly uint8 onePadActivate;
    readonly padding(3)
    readonly tdstInputDevice device[18];
    readonly uint8 keyboardCounter[256];
    readonly uint8 keyboardType;
    readonly uint8 mouseButtonCounter[9];
    readonly padding(2)
    readonly uint32 numEntries;
    readonly pointer entries;
    readonly uint32 numCommands;
    readonly pointer commands;
    readonly int16 eventSize;
    readonly padding(2)
    readonly pointer historicEvent;
    readonly uint8 oneActionValidated;
    readonly char8 line[78];
    readonly char8 lineInternal[78];
    readonly char8 lineSearch[78];
    readonly char8 lineLast[10][78];
    readonly padding(1)
    readonly int32 historicIndex;
    readonly pointer searchedCommand;
    readonly pointer commandModeEntrySwap;
    readonly pointer commandModeEntryClear;
};

struct stInputEntryElement
{
    readonly padding(6 * 4) /* ? */
    readonly uint32 numKeywords;
    readonly pointer keywordArray;
    readonly pointer actionName;
    readonly pointer entryName;
    readonly int32 state;
    readonly float32 analogValue;
    readonly int8 active;
    readonly padding(3)
};

#ifdef USE_FUNCTIONS

/** fnInputEntryFind: find an input entry by its action name */
tdstInputEntryElement* fnInputEntryFind(const tdstInputStructure* s, const char* action_name);

#endif

#endif /* stInputStructure_h */
