#include "gui.hh"
#include "run-manager.hh"

static const ImColor defaultColor = ImColor(0.2f, 1.0f, 0.3f, 0.75f);

static void drawSpecialText(ImDrawList* drawlist, std::string text, ImVec2 pos, float scale = 1.0f, ImColor textColor = ImColor(0.2f, 1.0f, 0.3f, 0.75f)) {
  ImColor highlightColor = ImColor(textColor.Value.x * 3.0f, textColor.Value.y * 3.0f, textColor.Value.z * 3.0f, 1.0f);
  ImColor shadowColor = ImColor(textColor.Value.x * 0.5f, textColor.Value.y * 0.5f, textColor.Value.z * 0.5f, 1.0f);
  drawlist->AddText(ImGui::GetFont(), ImGui::GetFontSize() * scale, ImVec2(pos.x - 0.5f, pos.y - 0.5f), highlightColor, text.c_str());
  drawlist->AddText(ImGui::GetFont(), ImGui::GetFontSize() * scale, ImVec2(pos.x + 0.5f, pos.y + 0.5f), shadowColor, text.c_str());
  drawlist->AddText(ImGui::GetFont(), ImGui::GetFontSize() * scale, ImVec2(pos.x, pos.y), textColor, text.c_str());
}

float angle = 0.0f;

void drawSpeedrunTimer() {
  ImDrawList* drawlist = ImGui::GetBackgroundDrawList();
  
  Run* currentRun = runManager.currentRun;
  if (currentRun) {
    if (currentRun->currentSection) {
      Run::Section* section = currentRun->currentSection;
      
      ImColor textColor;
      if (section->type == Run::Section::Type::Level) textColor = ImColor(0.1, 1.0f, 0.4f, 1.0f);
      if (section->type == Run::Section::Type::Load) textColor = ImColor(1.0f, 0.1f, 0.5f, 1.0f);
      
      uint32_t frame = currentRun->currentSection->framecount; //g_stEngineStructure->timer.currentFrame;
      uint32_t millisecond = (frame % 60) / 60.0f * 1000.0f;
      uint32_t second = (frame / 60) % 60;
      uint32_t minute = (frame / 60 / 60) % 60;
      uint32_t hour = (frame / 60 / 60 / 24) % 24;
      
      
      
      
      char buf[64];
      sprintf(buf, "%02d:%02d:%02d.%03d", hour, minute, second, millisecond);
      
      
      drawSpecialText(drawlist, buf, ImVec2(5, 200), 2.0f, textColor);
      drawlist->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 1.0f, ImVec2(5, 200 + ImGui::GetTextLineHeightWithSpacing() * 3.0f), textColor, section->name.c_str());
      drawlist->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 1.0f, ImVec2(5, 200 + ImGui::GetTextLineHeightWithSpacing() * 3.0f + ImGui::GetTextLineHeightWithSpacing() * 1), textColor, "LRT= 00:07:24");
      drawlist->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 1.0f, ImVec2(5, 200 + ImGui::GetTextLineHeightWithSpacing() * 3.0f + ImGui::GetTextLineHeightWithSpacing() * 2), textColor, "Level= 352");
      //drawSpecialText(drawlist, "LRT 00:07:24", ImVec2(0, 250), 1.0f, textColor);
      //drawlist->AddText(ImGui::GetFont(), ImGui::GetFontSize()*3.0f, ImVec2(0,200), ImColor(0.1f, 1.0f, 0.2f, 1.0f), "00:05:23");
      
    }
  }
}
