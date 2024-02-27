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

#include <iostream>

#include "imgui_memory_editor.h"

struct superObjectWindow {
  bool open = true;
  pointer<stSuperObject> target;
  superObjectWindow(stSuperObject *spo) : target(spo) {}
  auto draw() -> void;
};

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
}

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

struct GameWindow {
  GameWindow();
  void draw(ImTextureID texture);
};

struct CinematicWindow {
  CinematicWindow();
  void draw();
};

struct AIWindow {
  AIWindow(pointer<stSuperObject> target);
  void draw();
private:
  pointer<stSuperObject> targetObject;
  pointer<stBehavior> targetBehavior;
  
  void drawInfo();
  void drawBehaviorLists();
  void drawScript();
};

extern Window MenuBar;
extern Window ConfigurationWindow;
extern Window RNGWindow;
extern Window HierarchyWindow;
extern Window RunCreateWindow;
//extern Window MovieInputWindow;
//extern Window ScriptWindow;
//extern Window SuperObjectWindow;
extern Window bruteforceWindow;

#endif /* gui_hh */
