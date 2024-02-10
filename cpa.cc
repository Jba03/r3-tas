#include "cpa.hh"

namespace CPA {

  #if defined(htonl) && defined(htons)
  # define OPTIMIZED_BYTESWAP
  #endif
  
  /* swap integral byteorder */
  static auto memory_swap(std::integral auto v) {
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
  
  template <typename T>
  address<T>::operator T() const {
    return addr;
  }
  
  template <typename T>
  T address<T>::physicalAddress() {
#if platform == GCN
    return swap(addr);
#else
    /* ... */
#endif
  }
  
  template <typename T>
  T address<T>::effectiveAddress() {
#if platform == GCN
    return memory_swap(addr) & 0x7FFFFFFF;
#else
    /* ... */
#endif
  }
  
  template <typename T>
  bool address<T>::valid() {
    return effectiveAddress() != 0;
  }
  
#pragma mark - Type
  
  template <typename T, typename OpT>
  template <typename S>
  type<T, OpT>::type(const S v) {
    if constexpr (std::is_floating_point<S>::value) {
      /* copy float type to memory */
      data = swap(*(T*)&v);
    } else if constexpr (std::is_integral<S>::value) {
      /* copy integral type to memory */
      
    }
  }
  
  template <typename T, typename OpT>
  template <typename S>
  type<T, OpT>& type<T, OpT>::operator =(const S v) {
    if (memoryBound()) {
      
    }
  }
  
  template <typename T, typename OpT>
  bool type<T, OpT>::memoryBound() {
    
  }
  
#pragma mark - Structure -
  
  /** ``STRUCTURE BEGIN ``**/
  
  template <typename T, unsigned N> const T stVector<T, N>::dot(const stVector<T, N> vector) { T s=0.0f; for(int i=0;i<N;i++)s+=v[i]*v[i]; return s;}
  template <typename T, unsigned N> const T stVector<T, N>::square() { return dot(*this); }
  template <typename T, unsigned N> const T stVector<T, N>::length() { return std::sqrt(square()); }
  template <typename T, unsigned N> const T stVector<T, N>::x() { return v[0]; }
  template <typename T, unsigned N> const T stVector<T, N>::y() { return v[1]; }
  template <typename T, unsigned N> const T stVector<T, N>::z() { return v[2]; }
  template <typename T, unsigned N> const T stVector<T, N>::w() { return v[3]; }
  template <typename T, unsigned N> const stVector<T, N> stVector<T, N>::operator +(const Vec a) { for (int i = 0; i < N; i++) v[i] += a.v[i]; }
  template <typename T, unsigned N> const stVector<T, N> stVector<T, N>::operator -(const Vec a) { for (int i = 0; i < N; i++) v[i] -= a.v[i]; }
  template <typename T, unsigned N> const stVector<T, N> stVector<T, N>::operator *(const Vec a) { for (int i = 0; i < N; i++) v[i] *= a.v[i]; }
  template <typename T, unsigned N> const stVector<T, N> stVector<T, N>::operator /(const Vec a) { for (int i = 0; i < N; i++) v[i] /= a.v[i]; }
  template <typename T, unsigned N> const stVector<T, N> stVector<T, N>::operator *(const T   a) { for (int i = 0; i < N; i++) v[i] *= a;      }
  template <typename T, unsigned N> const stVector<T, N> stVector<T, N>::operator /(const T   a) { for (int i = 0; i < N; i++) v[i] /= a;      }
  
  template <typename T, unsigned N> const stMatrix<T, N> stMatrix<T, N>::operator *(const stMatrix<T, N> a) {
    stMatrix<T, N> result;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k++) {
          result.m[i][j] += m[i][k] * a.m[k][j];
        }
      }
    }
    return result;
  }
  
  template <typename T, unsigned N> const stVector4D stMatrix<T, N>::operator *(const stVector4D a) {
    
  }
  
  void stObjectTypeElement::serialize(serializer& s) {
    s.add(next);
    s.add(prev);
    s.add(father);
    s.add(name);
    s.add(priority);
    s.add(identifier);
  }
  
  void stObjectType::serialize(serializer& s) {
    s.add(family);
    s.add(model);
    s.add(instance);
  }
  
#pragma mark -
  
  void stEngineTimer::serialize(serializer& s) {
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
  
  void stEngineStructure::serialize(serializer& s) {
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
  
}
