//
//  gui.hh
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#ifndef gui_hh
#define gui_hh

#include "imgui.h"
#include "game.hh"

#include "imgui_memory_editor.h"

struct superObjectWindow {
  bool open = true;
  pointer<stSuperObject> target;
  superObjectWindow(stSuperObject *spo) : target(spo) {}
  auto draw() -> void;
};

namespace gui {
  
  void initialize();
  void draw(void *c, void *texture, bool *windowed);
  
  extern MemoryEditor memoryEditor;
  auto memoryEditorHighlight(const ImU8 *address, size_t offset) -> bool;
  
  extern std::vector<superObjectWindow> spoWindows;
  
  /// Popup
  auto drawPopup() -> void;
  extern pointer<> popupTarget;
  
  struct video_payload {
    void* texture;
    void* context;
    bool windowed;
  };
}

struct Window
{
    void (*Draw)(void);
    void (*SaveSettings)(void);
    bool Open = false;
};

extern Window MenuBar;
extern Window ConfigurationWindow;
extern Window RNGWindow;
extern Window HierarchyWindow;
extern Window RunCreateWindow;
extern Window CinematicWindow;
//extern Window MovieInputWindow;
//extern Window ScriptWindow;
//extern Window SuperObjectWindow;
extern Window bruteforceWindow;

#endif /* gui_hh */
