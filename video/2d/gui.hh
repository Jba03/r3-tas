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

struct CommonWindow {
  CommonWindow();
  void draw();
};

struct RNGWindow {
  RNGWindow();
  void draw();
private:
  int numEntriesShown = 20;
};

struct GameWindow {
  GameWindow();
  void draw(ImTextureID texture);
private:
  void drawMenuBar();
  void drawGame(ImTextureID texture);
  
  bool projectObjectPositions;
};

struct CinematicWindow {
  CinematicWindow();
  void draw();
};

struct AIWindow {
  AIWindow(pointer<stSuperObject> targetObject);
  void setTargetObject(pointer<stSuperObject> targetObject);
  void setTargetMacro(pointer<stMacro> macro);
  void setTargetBehavior(pointer<stBehavior> behavior);
  void drawScript();
  void drawDsgVars();
  void draw();
private:
  pointer<stSuperObject> targetObject = nullptr;
  pointer<stBehavior> targetBehavior = nullptr;
  pointer<stMacro> targetMacro = nullptr;
  
  bool displayActive = true;
  bool drawLineNumbers = true;
  bool quietReferences = false;
  
  void drawMenuBar();
  void drawInfo();
  void drawBehaviorLists();
};

struct StructureExplorerWindow {
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

extern Window MenuBar;
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
void marker(CPA::Script::TranslationToken tok, bool readonly = false, std::string customText = "", bool quiet = false);
void clearMarkers();

#endif /* gui_hh */
