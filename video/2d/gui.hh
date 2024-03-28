//
//  gui.hh
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#ifndef gui_hh
#define gui_hh

#include "imgui.h"
#include "implot.h"
#include "game.hh"
#include "interface.hh"
#include "script.hh"

#include <iostream>
#include <unordered_map>

#include "imgui_memory_editor.h"

struct superObjectWindow {
  bool open = true;
  pointer<stSuperObject> target;
  superObjectWindow(stSuperObject *spo) : target(spo) {}
  auto draw() -> void;
};

struct Window
{
    void (*Draw)(void);
    void (*SaveSettings)(void);
    bool Open = false;
};

class CommonWindow {
public:
  CommonWindow();
  void draw();
};

class RNGWindow {
  int numEntriesShown = 20;
public:
  RNGWindow();
  void draw();
};

class GameWindow {
  void drawMenuBar();
  void drawGame(ImTextureID texture);
  bool projectObjectPositions;
public:
  GameWindow();
  void draw(ImTextureID texture);
};

class CinematicWindow {
public:
  CinematicWindow();
  void draw();
};

class AIWindow {
  pointer<stSuperObject> targetObject = nullptr;
  pointer<stBehavior> targetBehavior = nullptr;
  pointer<stMacro> targetMacro = nullptr;
  
  bool displayActive = true;
  bool drawLineNumbers = true;
  bool quietReferences = false;
  int debuggerStep = 1;
  
  void drawMenuBar();
  void drawInfo();
  void drawBehaviorLists();
  void drawDebugPanel();
public:
  AIWindow(pointer<stSuperObject> targetObject);
  void setTargetObject(pointer<stSuperObject> targetObject);
  void setTargetMacro(pointer<stMacro> macro);
  void setTargetBehavior(pointer<stBehavior> behavior);
  void drawScript();
  void drawDsgVars();
  void draw();
  
  //std::unordered_map<pointer<stNodeInterpret>, pointer<stNodeInterpret>> debugMap;
};

class StructureExplorerWindow {
public:
  StructureExplorerWindow();
  void draw();
};

class InputWindow {
public:
  void draw();
private:
  bool displayJoyPad = true;
  float joystickRadius = 35.0f;
  ImColor joystickColor = IM_COL32(255, 0, 0, 196);
};

extern Window ConfigurationWindow;
//extern Window RNGWindow;
extern Window HierarchyWindow;
extern Window RunCreateWindow;
//extern Window MovieInputWindow;
//extern Window ScriptWindow;
//extern Window SuperObjectWindow;
extern Window bruteforceWindow;

namespace gui {
  
  extern ImGuiID dockspaceID;
  
  void initialize();
  void draw(void *c, void *texture, bool *windowed);
  
  extern MemoryEditor memoryEditor;
  auto memoryEditorHighlight(const ImU8 *address, size_t offset) -> bool;
  
  extern std::vector<superObjectWindow> spoWindows;
  
  /// Popup
  auto drawPopup() -> void;
  extern pointer<> popupTarget;
  
  /// Project a world coordinate to the screen
  ImVec4 projectWorldCoordinate(stVector3D p);
  
  extern AIWindow *aiWindow;
}


#pragma mark - Marker

/// A marker which references an object. Can be assigned to
/// other marker references compatible with the type of object `T`.
template <typename T>
void marker(pointer<T> reference, bool readonly = false, std::string customText = "", bool quiet = false);
void marker(cpa::script::TranslationToken tok, bool readonly = false, std::string customText = "", bool quiet = false);
void clearMarkers();

#endif /* gui_hh */
