#ifndef stEngineStructure_h
#define stEngineStructure_h

#include "library.hh"
#include "stEngineTimer.hh"
#include "stCineManager.hh"

struct stEngineStructure {
  
  auto reloadLevel() -> void {
    mode = engineMode::changeLevel;
  }
  
  auto loadLevel(std::string levelName) -> void {
    nextLevelName = levelName;
    mode = engineMode::changeLevel;
  }
  
  enum engineMode : uint8_t {
    invalid = 0,
    initialize = 1,
    deinitialize = 2,
    initializeGameplay = 3,
    deinitializeGameplay = 4,
    enterLevel = 5,
    changeLevel = 6,
    gameplay = 9,
  };
  
  enum inputMode : uint8_t {
    normal = 0,
    commands = 1,
  };
  
  readonly engineMode mode;
  readonly string<30> currentLevelName;
  readonly string<30> nextLevelName;
  readonly string<30> firstLevelName;
  readonly inputMode inputMode;
  readonly uint8 displayFixMode;
  readonly padding(3)
  readonly uint32 displayMode;
  readonly stEngineTimer timer;
  readonly uint8 multimodePlayerCount;
  readonly uint8 multimodeColumnCount;
  readonly uint8 multimodeMiniScreenRatio;
  readonly padding(1)
  readonly pointer<> currentMainPlayers[4]; /* ::superobject */
  readonly int16 gldDevice;
  readonly int16 gldViewport[5];
  readonly padding(5 * 28 * 4) /* viewport attributes */
  readonly pointer<> viewportCamera[5];
  readonly int16 gldFixViewport[5];
  readonly padding(2)
  readonly padding(5 * 28 * 4) /* fix viewport attributes */
  readonly padding(5 * 2 * 4) /* fix 3d attributes */
  readonly pointer<> fixCamera[5];
  readonly padding(5 * 2 * 4) /* game 3d attributes */
  readonly pointer<> viewportArray;
  readonly stDoublyLinkedList<> cameraList;
  readonly pointer<> drawSem;
  readonly stDoublyLinkedList<> familyList;
  readonly stDoublyLinkedList<> alwaysList;
  readonly stDoublyLinkedList<stSuperObject> mainCharacterList;
  readonly pointer<> standardCamera;
  readonly pointer<> debugCamera;
  readonly pointer<> languageStructure;
  readonly pointer<> levelFilenameList;
  readonly stTransform mainActorTransform;
  readonly stTransform mainCameraTransform;
  readonly int32 submapNumber;
  
  readonly uint8 paused;
  readonly uint8 paused2;
  readonly uint8 doGameSave;
  
  readonly string<30> levelNames[150];
  readonly string<12> demoNames[30];
  readonly string<12> demoLevelNames[30];
  
  readonly uint8 demoCount;
  readonly uint8 levelCount;
  readonly uint8 currentLevel;
  readonly uint8 previousLevel;
  readonly uint8 previousLevelExitID;
  readonly uint8 globalLevelCounter;
  readonly uint8 demoMode;
  readonly uint8 currentLanguage;
  readonly uint8 languageCount;
  readonly uint8 engineFrozen;
  readonly uint8 resurrection;
  readonly int8  cameraMode;
  readonly uint8 currentImportance;
  readonly uint8 numSuperObjectsAllocated;
  readonly uint8 numSuperObjectsLoaded;
  readonly uint8 numNonPersistentSOLinks;
  
  // 9 padding
  readonly padding(9);
  
  readonly doublepointer<> SOLinks;
  readonly pointer<> graphChainedList;
  readonly pointer<stCineManager> cineManager;
};

#ifdef USE_FUNCTIONS

/** fnEngineLoadLevel: load level with specified name */
void fnEngineLoadLevel(stEngineStructure* engine, const char* level_name);

#endif

#endif /* stEngineStructure_h */
