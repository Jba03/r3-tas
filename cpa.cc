#include "cpa.hh"

void CPA::stObjectTypeElement::serialize(serializer& s) {
  s.add(next);
  s.add(prev);
  s.add(father);
  s.add(name);
  s.add(priority);
  s.add(identifier);
}

void CPA::stObjectType::serialize(serializer& s) {
  s.add(family);
  s.add(model);
  s.add(instance);
}

#pragma mark -

void CPA::stEngineTimer::serialize(serializer& s) {
  s.add(currentFrame);
  s.add(timerHandle);
  s.add(currentCount);
  s.add(deltaCount);
  s.add(counter);
  s.add(usefulDeltaTime);
  s.add(pauseTime);
  s.add(frameLength);
  s.add(totalRealTimeLow);
  s.add(totalRealTimeHigh);
  s.add(totalPauseTimeLow);
  s.add(totalPauseTimeHigh);
  s.add(ticksPerMs);
}

#pragma mark -

void CPA::stEngineStructure::loadLevel(std::string levelName) {
  currentLevelName = levelName;
  mode = ChangeLevel;
}

void CPA::stEngineStructure::serialize(serializer& s) {
  s.add(mode);
  s.add(currentLevelName);
  s.add(nextLevelName);
  s.add(firstLevelName);
  s.add(inputMode);
  s.add(displayFixMode);
  s.add(displayMode);
  s.add(timer);
  s.add(multimodePlayerCount);
  s.add(multimodeColumnCount);
  s.add(multimodeMiniScreenRatio);
  s.add(gldDevice);
  s.add(viewportArray);
  s.add(cameraList);
  s.add(drawSem);
  s.add(familyList);
  s.add(alwaysList);
  s.add(mainCharacterList);
  s.add(standardCamera);
  s.add(debugCamera);
  s.add(languageStructure);
  s.add(levelFilenameList);
  s.add(mainActorTransform);
  s.add(mainCameraTransform);
  s.add(submapNumber);
  s.add(paused);
  s.add(paused2);
  s.add(doGameSave);
  for (pointer<stSuperObject>& so : currentMainPlayers) s.add(so);
  for (int16& i : gldViewport) s.add(i);
  for (int16& i : gldFixViewport) s.add(i);
  for (pointer<stCamera>& cam : viewportCamera) s.add(cam);
  for (pointer<stCamera>& cam : fixCamera) s.add(cam);
  for (string<30>& str : levelNames) s.add(str);
  for (string<12>& str : demoNames) s.add(str);
  for (string<12>& str : demoLevelNames) s.add(str);
  s.add(demoCount);
  s.add(levelCount);
  s.add(currentLevel);
  s.add(previousLevel);
  s.add(previousLevelExitID);
  s.add(globalLevelCounter);
  s.add(demoMode);
  s.add(currentLanguage);
  s.add(languageCount);
  s.add(engineFrozen);
  s.add(resurrection);
  s.add(cameraMode);
  s.add(currentImportance);
  s.add(numSuperObjectsAllocated);
  s.add(numSuperObjectsLoaded);
  s.add(numNonPersistentSOLinks);
  s.add(superObjectLinks);
  s.add(graphList);
  s.add(cineManager);
}

/** ``STRUCTURE END ``**/

#pragma mark - PointerFile

//CPA::PointerFile::PointerFile(std::fstream &file) {
//  uint32 numPointers = read<uint32>(file).swap();
//  while (numPointers--) {
//    uint32 targetFile = read<uint32>(file).swap();
//  }
//}
