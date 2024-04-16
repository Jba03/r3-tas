#include "gui.hh"
#include "collision.hh"

#define PLOT_SNAP_X (640.0f / 13.0f)
#define PLOT_SNAP_Y (528.0f / 13.0f)

template<typename T>
static inline void plotGridSnap(T& x, T& y) {
  x = PLOT_SNAP_X * round(x / PLOT_SNAP_X);
  y = PLOT_SNAP_Y * round(y / PLOT_SNAP_Y);
}


#pragma mark - Overlay

static void text(std::string text, ImPlotPoint position, ImVec4 textColor, ImVec4 backgroundColor = ImVec4(0,0,0,0), float padding = 2.5f) {
  ImVec2 sz = ImGui::CalcTextSize(text.c_str());
  ImVec2 pos = ImPlot::PlotToPixels(position);
  
  ImDrawList *drawlist = ImPlot::GetPlotDrawList();
  drawlist->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + sz.x + padding * 2, pos.y + sz.y + padding * 2), ImGui::GetColorU32(backgroundColor));
  drawlist->AddText(ImVec2(pos.x + padding, pos.y + padding), ImGui::GetColorU32(textColor), text.c_str());
}

static void objectMarker(pointer<stSuperObject> object) {
  if (object == game::g_stEngineStructure->standardCamera)
    return;
  
  stVector3D pos = object->globalTransform->translation();
  ImVec4 screenPos = gui::projectWorldCoordinate(pos);
  if (screenPos.w > 0.0f) {
    ImVec2 screenPosReal = ImPlot::PlotToPixels(ImPlotPoint(640.0f - screenPos.x * 640.0f, 528.0f - screenPos.y * 528.0f));
    
    
    ImColor color = game::objectColor(object);
    ImColor bgColor = color;
    bgColor.Value.w = 0.5f;
    
    ImGui::SetCursorScreenPos(screenPosReal);
    
    if (object->type == stSuperObject::type::Actor) {
      if (marker(pointer<stSuperObject>(object), memory::readonly) & MARKER_CLICKED) {
        gui::aiWindow->setTargetObject(object);
      }
    } else {
      
      ImDrawCmd cmd;
      
      ImDrawList *dw = ImPlot::GetPlotDrawList();
      dw->AddCircleFilled(screenPosReal, 5.0f, bgColor);
      dw->AddCircle(screenPosReal, 5.0f, color);
      dw->AddText(screenPosReal, color, object->name().c_str());
    }
  }
}

static void objectMarkersDrawWorld(stSuperObject *world, ImDrawList *drawlist, bool onlyActors = true) {
  if (world) {
    world->recurse([onlyActors](stSuperObject* object, ImDrawList *dw) {
      try {
        if (object->type == stSuperObject::type::Actor || (!onlyActors && object->type != stSuperObject::type::Actor))
          objectMarker(object);
      } catch (bad_pointer& e) {
        /* ... */
      }
    }, drawlist);
  }
}

static void overlayDrawObjects() {
  ImDrawList *drawlist = ImPlot::GetPlotDrawList();
  try {
    if (game::p_stActualWorld) {
      game::p_stActualWorld->recurse([](stSuperObject* object, ImDrawList *dw) {
        stVector3D pos = object->globalTransform->translation();
        ImVec4 screenPos = gui::projectWorldCoordinate(pos);
        if (screenPos.w > 0.0f) {
          ImVec2 screenPosReal = ImPlot::PlotToPixels(ImPlotPoint(640.0f - screenPos.x * 640.0f, 528.0f - screenPos.y * 528.0f));

          ImColor color = game::objectColor(object);
          ImColor bgColor = color;
          bgColor.Value.w = 0.5f;

          ImGui::SetCursorScreenPos(screenPosReal);
          marker(pointer<stSuperObject>(object), memory::readonly);

          ImDrawCmd cmd;

//          dw->AddCircleFilled(screenPosReal, 5.0f, bgColor);
//          dw->AddCircle(screenPosReal, 5.0f, color);
         // dw->AddText(screenPosReal, color, object->name(game::nameResolver).c_str());
        }
      }, drawlist);
    }
  } catch (bad_pointer& e) {
    /* ... */
  }
}

static auto drawLine(ImDrawList *drawlist, stVector3D a, stVector3D b, ImColor color = ImColor(1.0f, 0.0f, 0.0f, 1.0f), float thickness = 2.5f) -> void {
  ImVec4 p1 = gui::projectWorldCoordinate(a);
  ImVec4 p2 = gui::projectWorldCoordinate(b);
  ImVec2 s1 = ImPlot::PlotToPixels(ImPlotPoint(640.0f - p1.x * 640.0f, 528.0f - p1.y * 528.0f));
  ImVec2 s2 = ImPlot::PlotToPixels(ImPlotPoint(640.0f - p2.x * 640.0f, 528.0f - p2.y * 528.0f));
  drawlist->AddLine(s1, s2, color, thickness);
}

static void drawVector22() {
  try {
    pointer<stSuperObject> mainchar = game::g_stEngineStructure->currentMainPlayers[0];
    stVector3D pos = mainchar->globalTransform->translation();
    stVector3D dsg22 = *(stVector3D*)mainchar->actor->dsgVar(22);
    stVector3D t = pos + dsg22;
    
    ImVec4 p1 = gui::projectWorldCoordinate(pos);
    ImVec4 p2 = gui::projectWorldCoordinate(t);
    ImVec2 s1 = ImPlot::PlotToPixels(ImPlotPoint(640.0f - p1.x * 640.0f, 528.0f - p1.y * 528.0f));
    ImVec2 s2 = ImPlot::PlotToPixels(ImPlotPoint(640.0f - p2.x * 640.0f, 528.0f - p2.y * 528.0f));
    
    ImDrawList *drawlist = ImPlot::GetPlotDrawList();
    drawlist->AddLine(s1, s2, ImColor(0.0f, 1.0f, 1.0f, 1.0f), 2.5f);
    
//    for (auto& point : gui::xrayPoints) {
//      p1 = gui::projectWorldCoordinate(point);
//      if (p1.w > 0.0f) {
//        s1 = ImPlot::PlotToPixels(ImPlotPoint(640.0f - p1.x * 640.0f, 528.0f - p1.y * 528.0f));
//        drawlist->AddCircleFilled(s1, 1.0, ImColor(1.0f, 0.0f, 1.0f, 1.0f));
//      }
//    }
    
//    for (int i = 0; i < gui::xrayPoints.size() / 2; i += 2) {
//      stVector3D a = gui::xrayPoints[i+0];
//      stVector3D b = gui::xrayPoints[i+1];
//      p1 = gui::projectWorldCoordinate(a);
//      p2 = gui::projectWorldCoordinate(b);
//      if (p2.w > 0.0f && p2.w > 0.0f) {
//        s1 = ImPlot::PlotToPixels(ImPlotPoint(640.0f - p1.x * 640.0f, 528.0f - p1.y * 528.0f));
//        s2 = ImPlot::PlotToPixels(ImPlotPoint(640.0f - p2.x * 640.0f, 528.0f - p2.y * 528.0f));
//        drawlist->AddLine(s1, s2, ImColor(1.0f, 0.0f, 1.0f, 1.0f), 1.0);
//      }
//    }
    
  } catch (bad_pointer& e) {
    /* ... */
  }
}

static auto drawCollisionVectors() -> void {
  text(std::to_string(collisionTableEntries.size()) + " entries", ImPlotPoint(0.0f, 0.0f), ImVec4(1.0f, 0.0f, 1.0f, 1.0f));
  for (auto& entry : collisionTableEntries) {
    if (entry.radius == 0.8f)
    drawLine(ImPlot::GetPlotDrawList(), entry.tempHit, entry.tempHit + entry.tempNormal, ImColor(1.0f, 0.0f, 1.0f, 0.75f));
  }
  collisionTableEntries.clear();
}

static void drawOverlayCommon() {
  try {
    pointer<stEngineObject> global = game::findObject("global")->actor;
    
    int currentHealth = *(int32*)global->dsgVar(60);
    int maxHealth = *(int32*)global->dsgVar(61);
    std::string health = std::to_string(currentHealth) + "/" + std::to_string(maxHealth);
    text(health, ImPlotPoint(450, 72), ImVec4(1.0f, 0.525f, 0.75f, 1.0f), ImVec4(0.5f, 0.1f, 0.1f, 0.5f), 0.1f);
    
  } catch (...) {
    /* ... */
  }
}

static void drawOverlay(bool actorsOnly = true) {
  if (interface->mode == Practice) {
    text("PRACTICE MODE", ImVec2(0, 528), ImVec4(1.0f, 0.4f, 0.5f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 0.25f));
  } else if (interface->mode == Advanced) {
    text("ADVANCED MODE", ImVec2(0, 528), ImVec4(1.0f, 0.8f, 0.4f, 1.0f), ImVec4(1.0f, 0.75f, 0.0f, 0.25f));
  }
  
  drawOverlayCommon();
  
  
  
  ImDrawList *drawlist = ImPlot::GetPlotDrawList();
  objectMarkersDrawWorld(game::p_stDynamicWorld, drawlist, actorsOnly);
  
  drawCollisionVectors();
  drawVector22();
}


#pragma mark - World

static auto drawWorld(GraphicsContext* ctx, pointer<stSuperObject> root, stMatrix4D T) -> void {
  if (!root) return;
  
  try {
    T = root->globalTransform->matrix * T;
    if (root->type == stSuperObject::type::IPO) {
      //root->drawFlags = 0; // for collision/game blend
      ctx->drawIPO(root->data, T);
    }
    
    root->forEachChild([&](pointer<stSuperObject> object, void*) {
      drawWorld(ctx, object, T);
    });
  } catch (...) {}
}

static auto drawZDX(GraphicsContext* ctx, pointer<stSuperObject> object) -> void {
    auto draw = [&](pointer<stZdxList> list, stVector4D color) {
      try {
        assert(list->numZdx == list->list.numEntries);
        for (auto zdx : list->list) {
          pointer<stCollideObject> colobj = zdx->data;
          for (int i = 0; i < int(colobj->numElements); i++) {
            int16_t elementType = *(int16*)colobj->elementTypes[i];
            if (elementType == stCollideObject::IndexedSpheres) {
              pointer<stCollideElementSpheres> sphereSet = colobj->elements[i];
              for (int j = 0; j < sphereSet->numSpheres; j++) {
                pointer<stCollideElementIndexedSphere> sphere = sphereSet->spheres[i];
                int idx = int(sphere->indexOfCenterPoint);
                stVector3D pos = (object->globalTransform->matrix * *(stVector3D*)colobj->vertices[idx]).xyz();
                ctx->setColor(color);
                ctx->setUseCheckerTexture(true);
                ctx->drawSphere(pos, sphere->radius);
              }
            }
          }
        }
      } catch (...) {
        /* ... */
      }
    };
    
  pointer<stEngineObject> actor = object->actor;
  
  
  try { draw(actor->collSet->zdmList, stVector4D(1.0f, 1.0f, 0.0f, 0.5f)); } catch (...) { /* ... */ }
  try { draw(actor->collSet->zddList, stVector4D(0.0f, 1.0f, 1.0f, 0.5f)); } catch (...) { /* ... */ }
  try { draw(actor->collSet->zdrList, stVector4D(0.0f, 1.0f, 0.0f, 0.5f)); } catch (...) { /* ... */ }
  try { draw(actor->collSet->zdeList, stVector4D(1.0f, 0.0f, 0.0f, 0.5f)); } catch (...) { /* ... */ }
}

Viewport::Viewport() {
  /* ... */
}

auto Viewport::hasMouse() -> bool {
  ImPlotPoint mouse = ImPlot::GetPlotMousePos();
  ImPlotPoint mouse2 = ImPlot::GetPlotMousePos();
  ImPlotPoint min = position;
  ImPlotPoint max = ImPlotPoint(position.x + size.x, position.y + size.y);
  ImPlotPoint plotMin = ImPlot::GetPlotPos();
  ImPlotPoint plotMax = ImPlotPoint(plotMin.x + ImPlot::GetPlotSize().x, plotMin.y + ImPlot::GetPlotSize().y);
  return
  (mouse.x >= min.x && mouse.x <= max.x && mouse.y >= min.y && mouse.y <= max.y) && ImPlot::IsPlotHovered();
  
 // && (mouse2.x >= plotMin.x && mouse2.x <= plotMax.x && mouse2.y >= plotMin.y && mouse2.y <= plotMax.y);
}

#pragma mark - Game viewport

GameViewport::GameViewport() {
  /* ... */
}

static double x = 0, y = 0;

void GameViewport::draw() {
  
  
  ImGui::BeginChild("hi!");
  ImGui::EndChild();

  bool grab = ImPlot::DragPoint(0, &position.x, &position.y, ImVec4(0.1f, 0.4f, 1.0f, 0.75));
  ImPlot::PlotImage("##Game viewport", gui::gameTexture, position, ImVec2(position.x + size.x, position.y + size.y), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f, 1.0f, 1.0f, grab ? 0.5f : alpha));
  
//  ImGui::SetCursorScreenPos(ImPlot::PlotToPixels(position));
//  ImGui::SliderFloat("##GameAlpha", &alpha, 0.0f, 1.0f);
  
  drawOverlay();
  
  plotGridSnap(position.x, position.y);
}

#pragma mark - Collision viewport

CollisionViewport::CollisionViewport() {
  /* ... */
}

extern stVector3D genericDirection;
extern stVector3D realDirection;

void CollisionViewport::draw() {
  if (!game::isValidGameState()) return;
  
  graphics->beginFrame(gui::gameTexture);
  graphics->setViewMatrix(viewMatrix());
  graphics->setProjectionMatrix(projectionMatrix());
  graphics->setUseCheckerTexture(true);
  graphics->setDisableShading(false);
  drawWorld(graphics, game::p_stFatherSector, stMatrix4D());
  
  pointer<stSuperObject> main = game::g_stEngineStructure->currentMainPlayers[0];
  stVector3D pos = main->position();
 // graphics->setUseCheckerTexture(true);
//  graphics->drawLine(pos, pos + stVector3D(1.0f, 0.0f, 0.0f), stVector4D(1.0f, 0.0f, 0.0f, 1.0f));
//  graphics->drawLine(pos, pos + stVector3D(0.0f, 1.0f, 0.0f), stVector4D(0.0f, 1.0f, 0.0f, 1.0f));
//  graphics->drawLine(pos, pos + stVector3D(0.0f, 0.0f, 1.0f), stVector4D(0.0f, 0.0f, 1.0f, 1.0f));
  graphics->setUseCheckerTexture(true);
  
//  for (int i = 0; i < gui::xrayPoints.size() / 2; i += 2) {
//    stVector3D a = gui::xrayPoints[i+0];
//    stVector3D b = gui::xrayPoints[i+1];
//    graphics->drawLine(a, b, stVector4D(1.0f, 0.0f, 0.0f, 1.0f));
//  }
    
  graphics->setUseCheckerTexture(false);
  graphics->setColor(stVector4D(1.0f, 0.0f, 1.0f, 1.0f));
  graphics->draw(GraphicsContext::Points, gui::xrayPoints.size(), gui::xrayPoints.data());
  
  graphics->setColor(stVector4D(1.0f, 0.0f, 0.0f, 1.0f));
  graphics->draw(GraphicsContext::Lines, gui::xrayPoints.size(), gui::xrayPoints.data());
  
//  stPadReadingOutput pad = *(stPadReadingOutput*)pointer<stPadReadingOutput>(0x8042F8F8);
//  stVector3D lines[2] = { pos, pos + stVector3D(pad.globalVector.x, pad.globalVector.y, 0.0f) };
//  graphics->setColor(stVector4D(0.0f, 1.0f, 1.0f, 1.0f));
//  graphics->draw(GraphicsContext::Lines, 2, lines);
  
  
//  graphics->setDisableShading(true);
//  for (auto& entry : collisionTableEntries) {
//    stVector3D lines[2] = { entry.tempHit, entry.tempHit + entry.tempNormal };
//    graphics->setColor(stVector4D(1.0f, 0.0f, 1.0f, 1.0f));
//    graphics->draw(GraphicsContext::Lines, 2, lines);
//  }
//  collisionTableEntries.clear();
//  graphics->setDisableShading(false);
  
  stPadReadingOutput pad = *(stPadReadingOutput*)pointer<stPadReadingOutput>(0x8042F8F8);
  stVector3D lines[2] = { pos, pos - genericDirection };
  graphics->setColor(stVector4D(0.0f, 1.0f, 0.0f, 1.0f));
  graphics->draw(GraphicsContext::Lines, 2, lines);
  
  stVector3D lines2[2] = { pos, pos + realDirection };
  graphics->setColor(stVector4D(1.0f, 1.0f, 0.0f, 1.0f));
  graphics->draw(GraphicsContext::Lines, 2, lines2);
  
  //graphics->drawSphere(pos, 0.9f);
  
  //zdx
  graphics->setDisableShading(false);
  graphics->setUseCheckerTexture(true);
  game::p_stDynamicWorld->forEachChild([&](pointer<stSuperObject> object, void*) {
    drawZDX(graphics, object);
  }, nullptr);
  
  graphics->endFrame();
  
  bool grab = ImPlot::DragPoint(1, &position.x, &position.y, ImVec4(0.1f, 0.4f, 1.0f, 0.75));
  ImPlot::PlotImage("##Collision viewport", graphics->texture(), position, ImVec2(position.x + size.x, position.y + size.y), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f, 1.0f, 1.0f, grab ? 0.5f : alpha));
//
//  ImGui::SetCursorScreenPos(ImPlot::PlotToPixels(position));
//  ImGui::SliderFloat("##CollisionAlpha", &alpha, 0.0f, 1.0f);
  
  
  plotGridSnap(position.x, position.y);
}

#pragma mark - Collision viewport (topdown)

TopdownCollisionViewport::TopdownCollisionViewport() {
  /* ... */
}

void TopdownCollisionViewport::draw() {
  if (!game::isValidGameState()) return;
  
  pointer<stCamera> camera = game::g_stEngineStructure->viewportCamera[0];
  
  graphics->beginFrame(gui::gameTexture);
  graphics->setViewMatrix(stMatrix4D::make_lookat(stVector3D(0.0f, 0.0f, 10.0f), stVector3D(0.0f, 0.0f, 0.0f), stVector3D(0.0f, 0.0f, -1.0f)));
  graphics->setProjectionMatrix(projectionMatrix());
  graphics->setUseCheckerTexture(true);
  graphics->setDisableShading(true);
  drawWorld(graphics, game::p_stFatherSector, stMatrix4D());
  
  printf("draw top down world\n");
  
  pointer<stSuperObject> main = game::g_stEngineStructure->currentMainPlayers[0];
  stVector3D pos = main->position();
 // graphics->setUseCheckerTexture(true);
//  graphics->drawLine(pos, pos + stVector3D(1.0f, 0.0f, 0.0f), stVector4D(1.0f, 0.0f, 0.0f, 1.0f));
//  graphics->drawLine(pos, pos + stVector3D(0.0f, 1.0f, 0.0f), stVector4D(0.0f, 1.0f, 0.0f, 1.0f));
//  graphics->drawLine(pos, pos + stVector3D(0.0f, 0.0f, 1.0f), stVector4D(0.0f, 0.0f, 1.0f, 1.0f));
  graphics->setUseCheckerTexture(true);
  
//  for (int i = 0; i < gui::xrayPoints.size() / 2; i += 2) {
//    stVector3D a = gui::xrayPoints[i+0];
//    stVector3D b = gui::xrayPoints[i+1];
//    graphics->drawLine(a, b, stVector4D(1.0f, 0.0f, 0.0f, 1.0f));
//  }
  
//  stPadReadingOutput pad = *(stPadReadingOutput*)pointer<stPadReadingOutput>(0x8042F8F8);
//  stVector3D lines[2] = { pos, pos + stVector3D(pad.globalVector.x, pad.globalVector.y, 0.0f) };
//  graphics->setColor(stVector4D(0.0f, 1.0f, 1.0f, 1.0f));
//  graphics->draw(GraphicsContext::Lines, 2, lines);
  
  stPadReadingOutput pad = *(stPadReadingOutput*)pointer<stPadReadingOutput>(0x8042F8F8);
  stVector3D lines[2] = { pos, pos - genericDirection };
  graphics->setColor(stVector4D(0.0f, 1.0f, 0.0f, 1.0f));
  graphics->draw(GraphicsContext::Lines, 2, lines);
  
  stVector3D lines2[2] = { pos, pos + realDirection };
  graphics->setColor(stVector4D(1.0f, 1.0f, 0.0f, 1.0f));
  graphics->draw(GraphicsContext::Lines, 2, lines2);
  
  //graphics->drawSphere(pos, 0.9f);
  
  //zdx
  graphics->setDisableShading(false);
  graphics->setUseCheckerTexture(true);
  game::p_stDynamicWorld->forEachChild([&](pointer<stSuperObject> object, void*) {
    drawZDX(graphics, object);
  }, nullptr);
  
  graphics->endFrame();
  
  bool grab = ImPlot::DragPoint(1, &position.x, &position.y, ImVec4(0.1f, 0.4f, 1.0f, 0.75));
  ImPlot::PlotImage("##Topdown Collision viewport", graphics->texture(), position, ImVec2(position.x + size.x, position.y + size.y), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f, 1.0f, 1.0f, grab ? 0.5f : 1.0f));
  
  plotGridSnap(position.x, position.y);
}
