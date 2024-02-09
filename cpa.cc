#include "cpa.hh"
  
/* swap integral byteorder */
auto cpa::memory_swap(std::integral auto v) {
  if constexpr (endianness != std::endian::native) {
#ifdef OPTIMIZED_BYTESWAP
    if constexpr (sizeof(v) == 1) return v;
    if constexpr (sizeof(v) == 2) return htons(v);
    if constexpr (sizeof(v) == 4) return htonl(v);
#else
    if constexpr (sizeof(v) == 1) return v;
    if constexpr (sizeof(v) == 2) return ((v>>8)&0x00FF)|((v<<8)&0xFF00);
    if constexpr (sizeof(v) == 4) return ((v>>24)&0xFF)|((v>>8)&0xFF00)|((v<<8)&0xFF0000)|((v<<24)&0xFF000000);
#endif
  }
}
  
#pragma mark - Address
  
template <typename T> cpa::address<T>::operator T() const {
  return addr;
}

template <typename T> T cpa::address<T>::physicalAddress() {
#if platform == GCN
  return swap(addr);
#else
  /* ... */
#endif
}

template <typename T> T cpa::address<T>::effectiveAddress() {
#if platform == GCN
  return swap(addr) & 0x7FFFFFFF;
#else
  /* ... */
#endif
}

template <typename T> bool cpa::address<T>::valid() {
  return effectiveAddress() != 0;
}

#pragma mark - Type

template <typename T, typename OpT>
template <typename S> cpa::type<T, OpT>::type(const S v) {
  if constexpr (std::is_floating_point<S>::value) {
    /* copy float type to memory */
    data = swap(*(T*)&v);
  } else if constexpr (std::is_integral<S>::value) {
    /* copy integral type to memory */
    
  }
}

template <typename T, typename OpT>
template <typename S> cpa::type<T, OpT>& cpa::type<T, OpT>::operator =(const S v) {
  if (memoryBound()) {
    
  }
}

template <typename T, typename OpT> bool cpa::type<T, OpT>::memoryBound() {
  
}

#pragma mark - Structure -

/** ``STRUCTURE BEGIN ``**/

void cpa::stObjectTypeElement::serialize(serializer& s) {
  s.add(next);
  s.add(prev);
  s.add(father);
  s.add(name);
  s.add(priority);
  s.add(identifier);
}

void cpa::stObjectType::serialize(serializer& s) {
  s.add(family);
  s.add(model);
  s.add(instance);
}

#pragma mark -

void cpa::stEngineTimer::serialize(serializer& s) {
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

void cpa::stEngineStructure::serialize(serializer& s) {
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
