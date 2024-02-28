#include "gui.hh"

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

#pragma mark - Object markers

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
    marker(pointer<stSuperObject>(object), Memory::readonly);
    
    ImDrawCmd cmd;
    
//          dw->AddCircleFilled(screenPosReal, 5.0f, bgColor);
//          dw->AddCircle(screenPosReal, 5.0f, color);
   // dw->AddText(screenPosReal, color, object->name(game::nameResolver).c_str());
  }
}

static void objectMarkersDrawWorld(stSuperObject *world, ImDrawList *drawlist, bool onlyActors = true) {
  if (world) {
    world->recurse([onlyActors](stSuperObject* object, ImDrawList *dw) {
      try {
        if (object->type == eSuperObjectType::superObjectTypeActor || (!onlyActors && object->type != eSuperObjectType::superObjectTypeActor))
          objectMarker(object);
      } catch (BadPointer& e) {
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
//          marker(pointer<stSuperObject>(object), Memory::readonly);
//          
//          ImDrawCmd cmd;
//          
////          dw->AddCircleFilled(screenPosReal, 5.0f, bgColor);
////          dw->AddCircle(screenPosReal, 5.0f, color);
//         // dw->AddText(screenPosReal, color, object->name(game::nameResolver).c_str());
//        }
//      }, drawlist);
//    }
//  } catch (BadPointer& e) {
//    /* ... */
//  }
}

static void drawOverlay() {
  if (interface->mode == Practice)
    text("PRACTICE MODE", ImVec2(0, 528), ImVec4(1.0f, 0.4f, 0.5f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 0.25f));
  
  ImPlot::PushStyleColor(ImPlotCol_InlayText, ImVec4(1.0f, 0.4f, 0.6f, 1.0f));
  ImPlot::PlotText("Health 40/100", 450, 65);
  ImPlot::PopStyleColor();
  
  ImPlot::PushStyleColor(ImPlotCol_InlayText, ImVec4(1.0f, 1.0f, 0.7f, 1.0f));
  ImPlot::PlotText("Bossbar", 120, 48);
  ImPlot::PopStyleColor();
  
  //overlayDrawObjects();
  ImDrawList *drawlist = ImPlot::GetPlotDrawList();
  objectMarkersDrawWorld(game::p_stDynamicWorld, drawlist);
  
}

void GameWindow::drawGame(ImTextureID texture) {
  ImVec2 avail = ImGui::GetContentRegionAvail();
  ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0.0f, 0.0f));
  if (ImPlot::BeginPlot("##game-display-plot", avail, ImPlotFlags_NoTitle | ImPlotFlags_NoFrame | ImPlotFlags_Equal)) {
    ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoTickMarks | /*ImPlotAxisFlags_NoGridLines*/ ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels);
    ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoTickMarks | /*ImPlotAxisFlags_NoGridLines*/ ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels);
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
    
    ImPlot::PlotImage("##game", texture, ImVec2(0,0), ImVec2(640,528), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImPlot::PlotImage("##game", texture, ImVec2(640,0), ImVec2(640+640,528), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    if (interface->mode != Speedrun)
      drawOverlay();
      
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
          if (ImGui::SliderFloat("Timescale", &timescale, 0.1f, 10.0f, "%.1f")) {
            game::g_stEngineStructure->timer.ticksPerMs = uint32_t(40500 * 1.0f / timescale);
          }
          ImGui::EndMenu();
        }
        
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
  /* ... */
}

void GameWindow::draw(ImTextureID texture) {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Game", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);
  ImGui::SetWindowSize(ImVec2(640,528));
  ImGui::PopStyleVar();
  
  drawMenuBar();
  
  drawGame(texture);
  
  ImGui::End();
}
