#include "gui.hh"
#include "graphics.hh"
#include "geometry.cc"
#include "xray.hpp"

#include "implot_internal.h"

#pragma mark - Object markers

static bool overlay = true;
static bool skippingIntro = false;
static float timescale = 1.0f;

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
//  ImDrawList *drawlist = ImPlot::GetPlotDrawList();
//  try {
//    if (game::p_stActualWorld) {
//      game::p_stActualWorld->recurse([](stSuperObject* object, ImDrawList *dw) {
//        stVector3D pos = object->globalTransform->translation();
//        ImVec4 screenPos = gui::projectWorldCoordinate(pos);
//        if (screenPos.w > 0.0f) {
//          ImVec2 screenPosReal = ImPlot::PlotToPixels(ImPlotPoint(640.0f - screenPos.x * 640.0f, 528.0f - screenPos.y * 528.0f));
//          
//          ImColor color = game::objectColor(object);
//          ImColor bgColor = color;
//          bgColor.Value.w = 0.5f;
//          
//          ImGui::SetCursorScreenPos(screenPosReal);
//          marker(pointer<stSuperObject>(object), memory::readonly);
//          
//          ImDrawCmd cmd;
//          
////          dw->AddCircleFilled(screenPosReal, 5.0f, bgColor);
////          dw->AddCircle(screenPosReal, 5.0f, color);
//         // dw->AddText(screenPosReal, color, object->name(game::nameResolver).c_str());
//        }
//      }, drawlist);
//    }
//  } catch (bad_pointer& e) {
//    /* ... */
//  }
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
    drawlist->AddLine(s1, s2, ImColor(0.0f, 1.0f, 0.0f, 1.0f), 1.5f);
    
  } catch (bad_pointer& e) {
    /* ... */
  }
}

static void drawSpeedVector() {
  try {
    pointer<stSuperObject> mainchar = game::g_stEngineStructure->currentMainPlayers[0];
    stVector3D pos = mainchar->position();
    stVector3D speed = mainchar->actor->speed();
    stVector3D t = pos + speed;
    
    ImVec4 p1 = gui::projectWorldCoordinate(pos);
    ImVec4 p2 = gui::projectWorldCoordinate(t);
    ImVec2 s1 = ImPlot::PlotToPixels(ImPlotPoint(640.0f - p1.x * 640.0f, 528.0f - p1.y * 528.0f));
    ImVec2 s2 = ImPlot::PlotToPixels(ImPlotPoint(640.0f - p2.x * 640.0f, 528.0f - p2.y * 528.0f));
    
    ImDrawList *drawlist = ImPlot::GetPlotDrawList();
    drawlist->AddLine(s1, s2, ImColor(1.0f, 0.0f, 0.0f, 1.0f), 1.5f);
    
  } catch (bad_pointer& e) {
    /* ... */
  }
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
  //objectMarkersDrawWorld(game::p_stDynamicWorld, drawlist, actorsOnly);
  
  drawSpeedVector();
  drawVector22();
}

void GameWindow::drawGame(ImTextureID texture) {
  ImVec2 avail = ImGui::GetContentRegionAvail();
  ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0.0f, 0.0f));
  if (ImPlot::BeginPlot("##game-display-plot", avail, ImPlotFlags_NoTitle | ImPlotFlags_NoFrame | ImPlotFlags_Equal)) {
    
    
    ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels);
    ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels);
    ImPlot::SetupAxisTicks(ImAxis_X1, -640.0f, 640.0f * 2.0f, 40);
    ImPlot::SetupAxisTicks(ImAxis_Y1, -528.0f, 528.0f * 2.0f, 33*(640.0f/528.0f));
    
//    ImVec2 p = ImVec2(0,0);//ImGui::GetCursorPos();
//    ImVec2 av = avail;
//    ImVec2 m = ImVec2(av.x / 640.0f, av.y / 528.0f);
//    ImPlotPoint sz = av, pos = p;
//
//    if (m.y < m.x) {
//      sz.x = av.y / 528.0f * 640.0f;
//      pos.x = p.x + av.x / 2 - sz.x / 2;
//    } else if (m.x < m.y) {
//      sz.y = av.x / 640.0f * 528.0f;
//      pos.y = p.y + av.y / 2 - sz.y / 2;
//    }
//
//    ImVec2 p = ImGui::GetCursorPos();
//    ImVec2 av = ImGui::GetContentRegionAvail();
//    ImVec2 m = ImVec2(av.x / 640.0f, av.y / 528.0f);
//    ImVec2 sz = av;
//    if (m.y < m.x) {
//      sz.x = av.y / 528.0f * 640.0f;
//      //ImGui::SetCursorPosX(p.x + av.x / 2 - sz.x / 2);
//    } else if (m.x < m.y) {
//      sz.y = av.x / 640.0f * 528.0f;
//      //ImGui::SetCursorPosY(p.y + av.y / 2 - sz.y / 2);
//    }
    
    //ImPlot::PlotImage("##game", texture, ImVec2(0,0), ImVec2(640,528), ImVec2(0,0), ImVec2(1,1), ImVec4(gameBrightness, gameBrightness, gameBrightness, 1.0f));
    
    //ImPlot::PlotImage("##collision", mainContext->texture(), ImVec2(640,0), ImVec2(640+640,528), ImVec2(0,0), ImVec2(1,1), ImVec4(gameBrightness, gameBrightness, gameBrightness, 1.0f));
    
      ImPlot::GetCurrentContext()->CurrentPlot->Flags &= ~ImPlotFlags_NoMenus;
      for (auto& viewport : viewports) {
        viewport->draw();
      }
    
   // ImPlot::PlotImage("##game", texture, ImVec2(640,0), ImVec2(640+640,528), ImVec2(0,0), ImVec2(1,1), ImVec4(gameBrightness, gameBrightness, gameBrightness, 1.0f));

    
//    int y = 0;
//    for (auto& v : gui::temporaryMessages) {
//      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 0.75f));
//      ImPlot::PlotText(v.first.c_str(), 0.0f, y -= ImGui::GetTextLineHeightWithSpacing());
//      ImGui::PopStyleColor();
//    }
//
//    try {
//      pointer<stSuperObject> drawSector = sectorSearch(game::p_stFatherSector, game::g_stEngineStructure->currentMainPlayers[0]->globalTransform->translation());
//      ImDrawList *dw = ImGui::GetWindowDrawList();
//
////      dw->PushClipRect(ImPlot::PlotToPixels(ImVec2(0,528)), ImPlot::PlotToPixels(ImVec2(640,0)), true);
////      drawGeometryRecursive(drawSector, stMatrix4D::identity(), dw, ImVec2(0,0), ImVec2(640,528), ImVec4(1.0f, 0.75f, 0.0f, 1.0f));
////      dw->PopClipRect();
////
////      std::vector<stVector3D> vertices;
////      getIPOVertices(drawSector, stMatrix4D::identity(), vertices);
////
////      int i = 0;
////      for (auto& v : vertices) {
////        ImVec4 proj = gui::projectWorldCoordinate(v);
////        if (proj.w > 0.0f) {
////          ImVec2 screenPosReal = ImPlot::PlotToPixels(ImPlotPoint(640.0f - proj.x * 640.0f, 528.0f - proj.y * 528.0f));
////
////          //ImDrawList *dw = ImPlot::GetPlotDrawList();
////         // dw->AddTriangleFilled(screenPosReal, ImVec2(screenPosReal.x+1,screenPosReal.y+1), ImVec2(screenPosReal.x-1,screenPosReal.y+1), ImColor(1.0f, 0.0f, 0.7f, 0.5f));
////          //dw->AddCircleFilled(screenPosReal, 5.0f, ImColor(1.0f, 0.0f, 0.7f, 0.5f));
////          ///dw->AddCircle(screenPosReal, 5.0f, color);
////          //dw->AddText(screenPosReal, color, object->name().c_str());
////        }
////      }
////
//
//      //drawGeometryTopdown(game::p_stFatherSector, stMatrix4D::identity(), dw, ImVec2(0,0), ImVec2(640,528), ImVec4(0.0f, 1.0f, 0.25f, 1.0f));
//
//    } catch (...) {
//      //printf(<#const char *, ...#>)
//    }

    if (interface->mode != Speedrun)
      drawOverlay(!projectActorChildren);

    ImPlot::EndPlot();
  }
  ImPlot::PopStyleVar(1);
}

void GameWindow::drawMenuBar() {
  if (ImGui::BeginMenuBar()) {
    if (interface->mode != Speedrun) {
      ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "%d", int(game::g_stEngineStructure->mode));
      if (ImGui::BeginMenu("Engine")) {
        if (ImGui::BeginMenu("Change level")) {
          for (int n = 0; n < game::g_stEngineStructure->levelCount; n++) {
            std::string name = game::g_stEngineStructure->levelNames[n];
            bool b = std::string(game::g_stEngineStructure->currentLevelName) == name;
            if (b) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.65f, 0.0f, 1.0f));
            if (ImGui::MenuItem(name.c_str(), nullptr, b)) game::g_stEngineStructure->loadLevel(name);
            if (b) ImGui::PopStyleColor();
          }
          ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Change timescale")) {
          ImGui::SliderFloat("##timescale-param", &timescale, 0.1f, 10.0f, "%.1f");
          ImGui::EndMenu();
        }
        
        ImGui::EndMenu();
      }
      
      if (ImGui::BeginMenu("Projections")) {
        if (ImGui::BeginMenu("Objects")) {
          ImGui::MenuItem("Dynamic world", nullptr, true);
          ImGui::MenuItem("Inactive dynamic world", nullptr, true);
          ImGui::MenuItem("Father sector", nullptr, true);
          ImGui::Separator();
          ImGui::MenuItem("Actor children", nullptr, &projectActorChildren);
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Collision")) {
          ImGui::MenuItem("Enable", nullptr, true);
          ImGui::EndMenu();
        }
        ImGui::SliderFloat("Game brightness", &gameBrightness, 0.0f, 1.0f);
        ImGui::EndMenu();
      }
      
      if (ImGui::BeginMenu("Tools")) {
       // ImGui::
        ImGui::EndMenu();
      }
      
    } else {
      if (game::g_stEngineStructure->currentLevelName == "intro_10" && game::isValidGameState()) {
        if (ImGui::Button("Skip intro")) {
          skippingIntro = true;
          game::g_stEngineStructure->loadLevel("intro_15");
        }
      } else if (game::g_stEngineStructure->currentLevelName == "intro_15") {
        if (skippingIntro) {
          skippingIntro = false;
        }
      }
    }
    
    
    ImGui::EndMenuBar();
  }
}

GameWindow::GameWindow() {
  Viewport *v = new GameViewport();
  viewports.push_back(v);
  viewports.push_back(new CollisionViewport());
  //viewports.push_back(new TopdownCollisionViewport());
}

void GameWindow::draw(ImTextureID texture) {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Game", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
  ImGui::SetWindowSize(ImVec2(640,528));
  ImGui::PopStyleVar();
  
  if (ImGui::IsKeyPressed(ImGuiKey_W)) {
    printf("W\n");
  }
  
  drawMenuBar();
  drawGame(texture);
  
  ImGui::End();
  
  // Update timescale
  //game::g_stEngineStructure->timer.ticksPerMs = uint32_t(40500 * 1.0f / timescale);
}
