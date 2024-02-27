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

static void drawOverlay() {
  if (interface->mode == Practice)
    text("PRACTICE MODE", ImVec2(0, 528), ImVec4(1.0f, 0.4f, 0.5f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 0.25f));
  
  ImPlot::PushStyleColor(ImPlotCol_InlayText, ImVec4(1.0f, 0.4f, 0.6f, 1.0f));
  ImPlot::PlotText("Health 40/100", 450, 65);
  ImPlot::PopStyleColor();
  
  ImPlot::PushStyleColor(ImPlotCol_InlayText, ImVec4(1.0f, 1.0f, 0.7f, 1.0f));
  ImPlot::PlotText("Bossbar", 120, 48);
  ImPlot::PopStyleColor();
  
}

static void drawGame(ImTextureID texture, ImVec2 sz) {
  if (ImPlot::BeginPlot("##game-display-plot", sz, ImPlotFlags_NoTitle | ImPlotFlags_NoFrame | ImPlotFlags_NoInputs)) {
    ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoDecorations);
    ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoDecorations);
    
    ImPlot::PlotImage("##game", texture, ImPlotPoint(0, 0), ImPlotPoint(640,528), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    if (interface->mode != Speedrun)
      drawOverlay();
      
    ImPlot::EndPlot();
  }
}

static void drawMenuBar() {
  if (ImGui::BeginMenuBar()) {
    if (interface->mode != Speedrun) {
      ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "%d", int(game::g_stEngineStructure->mode));
      if (ImGui::BeginMenu("Engine")) {
        if (ImGui::BeginMenu("Change level")) {
          for (int n = 0; n < game::g_stEngineStructure->levelCount; n++) {
            std::string name = game::g_stEngineStructure->levelNames[n];
            bool b = game::g_stEngineStructure->currentLevelName == name;
            if (b) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.65f, 0.0f, 1.0f));
            if (ImGui::MenuItem(name.c_str())) game::g_stEngineStructure->loadLevel(name);
            if (b) ImGui::PopStyleColor();
          }
          ImGui::EndMenu();
        }
        
        //if (ImGui::BeginMenu("Adjust timescale")) {
        if (ImGui::SliderFloat("Timescale", &timescale, 0.1f, 10.0f, "%.1f")) {
          game::g_stEngineStructure->timer.ticksPerMs = uint32_t(40500 * 1.0f / timescale);
        }
        //*(uint32*)(memory::baseAddress + 0x005D8510) = timescale * ;
        //}
        
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
  ImGui::Begin("Game", nullptr, ImGuiWindowFlags_MenuBar);
  ImGui::SetWindowSize(ImVec2(640,528));
  
  drawMenuBar();
  
  ImVec2 p = ImGui::GetCursorPos();
  ImVec2 av = ImGui::GetContentRegionAvail();
  ImVec2 m = ImVec2(av.x / 640.0f, av.y / 528.0f);
  ImVec2 sz = av;
  if (m.y < m.x) {
    sz.x = av.y / 528.0f * 640.0f;
    ImGui::SetCursorPosX(p.x + av.x / 2 - sz.x / 2);
  } else if (m.x < m.y) {
    sz.y = av.x / 640.0f * 528.0f;
    ImGui::SetCursorPosY(p.y + av.y / 2 - sz.y / 2);
  }
  
  drawGame(texture, sz);
  
  ImGui::End();
}
