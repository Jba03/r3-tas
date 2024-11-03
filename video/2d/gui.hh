//
//  gui.hh
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#ifndef gui_hh
#define gui_hh

#include <imgui/imgui.h>
#include <imgui/imgui_memory_editor.h>
#include <implot/implot.h>

#include "game.hh"
#include "interface.hh"
#include <cpatools/script.hpp>
#include "graphics.hh"

#include "viewport.hh"

#include <iostream>
#include <unordered_map>

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

class GameWindow {
  void drawMenuBar();
  void drawGame(ImTextureID texture);
  bool projectObjectPositions;
  bool projectActorChildren;
  float gameBrightness = 1.0f;
  
  std::vector<Viewport*> viewports;
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

class RNGWindow {
  int numEntriesShown = 20;
public:
  RNGWindow();
  void draw();
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

extern Window HierarchyWindow;
extern Window ConfigurationWindow;

static auto projectionMatrix() {
  float32* rawProjection = pointer<float32>(0x80273588);
  //pointer<float32>(0x80272fec);
  
  stMatrix4D projectionMatrix;
  projectionMatrix.m[0] = rawProjection[0];
  projectionMatrix.m[1] = 0.0f;
  projectionMatrix.m[2] = rawProjection[1];
  projectionMatrix.m[3] = 0.0f;
  projectionMatrix.m[4] = 0.0f;
  projectionMatrix.m[5] = rawProjection[2];
  projectionMatrix.m[6] = rawProjection[3];
  projectionMatrix.m[7] = 0.0f;
  projectionMatrix.m[8] = 0.0f;
  projectionMatrix.m[9] = 0.0f;
  projectionMatrix.m[10] = rawProjection[4];
  projectionMatrix.m[11] = rawProjection[5];
  projectionMatrix.m[12] = 0.0f;
  projectionMatrix.m[13] = 0.0f;
  projectionMatrix.m[14] = -1.0f;
  projectionMatrix.m[15] = 0.0f;

  projectionMatrix(0,0) = -projectionMatrix(0,0);
  projectionMatrix(1,1) = -projectionMatrix(1,1);
  projectionMatrix(2,2) -= 1;
  projectionMatrix(3,2) *= 0.5f;
  projectionMatrix(2,3) *= 4.0f;
  
//  pointer<stCamera> camera = g_stEngineStructure->viewportCamera[0];
//  const float fov = camera->xAlpha;
//  stMatrix4D projectionMatrix = stMatrix4D::make_perspective(fov, 640.f/528.0f, camera->near, camera->far);
  
  return projectionMatrix;
}

extern std::deque<stMatrix4D> viewMatrixStack;

static auto viewMatrix() {
  
  pointer<stCameraGLI> camera = g_stEngineStructure->viewportCamera[0];
  stMatrix4D view = camera->transform.matrix;
  view(0,1) = -(float)view(0,1); view(1,1) = -(float)view(1,1);
  view(2,1) = -(float)view(2,1); view(2,2) = -(float)view(2,2);
  view(3,1) = -(float)view(3,1); view(3,2) = -(float)view(3,2);
  view(0,2) = -(float)view(0,2); view(1,2) = -(float)view(1,2);
  
//  auto v = viewMatrixStack.front();
//  viewMatrixStack.pop_front();
//  viewMatrixStack.push_back(view);
  return view;
  //return v;
}



#pragma mark - Gui

namespace gui {

static inline void drawVector3(stVector3D v) {
  ImGui::Text("[%.2f, %.2f, %.2f]", float(v.x()), float(v.y()), float(v.z()));
}

static inline void drawMatrix3(stMatrix3D mat) {
  ImGui::Text("[%.2f  %.2f  %.2f]", float(mat.m[0]), float(mat.m[1]), float(mat.m[2]));
  ImGui::Text("[%.2f  %.2f  %.2f]", float(mat.m[3]), float(mat.m[4]), float(mat.m[5]));
  ImGui::Text("[%.2f  %.2f  %.2f]", float(mat.m[6]), float(mat.m[7]), float(mat.m[8]));
}

static inline void drawMatrix4(stMatrix4D mat) {
  ImGui::Text("[%.2f  %.2f  %.2f  %.2f]", float(mat.m[0]), float(mat.m[1]), float(mat.m[2]), float(mat.m[3]));
  ImGui::Text("[%.2f  %.2f  %.2f  %.2f]", float(mat.m[4]), float(mat.m[5]), float(mat.m[6]), float(mat.m[7]));
  ImGui::Text("[%.2f  %.2f  %.2f  %.2f]", float(mat.m[8]), float(mat.m[9]), float(mat.m[10]), float(mat.m[11]));
  ImGui::Text("[%.2f  %.2f  %.2f  %.2f]", float(mat.m[12]), float(mat.m[13]), float(mat.m[14]), float(mat.m[15]));
}

static inline void drawTransform(pointer<stTransform> T) {
  ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.3f, 1.0f), "TRANSFORM(%s)", T->typeName().c_str());
  drawMatrix4(T->matrix);
}


extern std::vector<stVector3D> xrayPoints;

extern ImTextureID gameTexture;

void initialize();
void draw(void *c, void *texture, bool *windowed);

auto saveLayout(const std::string& filename) -> void;
auto loadLayout(const std::string& filename) -> void;

  extern MemoryEditor memoryEditor;
  auto memoryEditorHighlight(const ImU8 *address, size_t offset) -> bool;
  
  extern std::vector<superObjectWindow> spoWindows;
  
  /// Popup
  auto drawPopup() -> void;
  extern pointer<> popupTarget;
  
  /// Project a world coordinate to the screen
  ImVec4 projectWorldCoordinate(stVector3D p);
  
void addTemporaryMessage(std::string msg);

  extern std::vector<std::pair<std::string, int>> temporaryMessages;
  extern AIWindow *aiWindow;
}


#pragma mark - Marker

#define MARKER_HOVERED  (1 << 0)
#define MARKER_CLICKED  (1 << 1)

/// A marker which references an object. Can be assigned to
/// other marker references compatible with the type of object `T`.
template <typename T>
int marker(pointer<T> reference, bool readonly = false, std::string customText = "", bool quiet = false);
void marker(cpa::script::TranslationToken tok, bool readonly = false, std::string customText = "", bool quiet = false);
void clearMarkers();

void drawSpeedrunTimer();
void drawRunManager();

#endif /* gui_hh */
