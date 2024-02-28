#include "gui.hh"
#include "imgui_internal.h"

static const ImVec2 defaultPadding = ImVec2(3.0f, 2.5f);
static const float defaultRounding = 5.0f;

static bool line = true;

struct Marker {
  bool hovered;
};

static std::vector<Marker> activeMarkers;
static Marker* selectedMarker = nullptr;

static bool markerElement(std::string text, bool quiet, ImVec4 color, ImVec2 pad, float round, bool *clicked = nullptr) {
  ImDrawList *drawlist = ImGui::GetWindowDrawList();
  
  
  
  ImVec2 itemPos = ImGui::GetCursorScreenPos();
  ImVec2 itemSize = ImGui::CalcTextSize(text.c_str());
  ImVec2 mousePos = ImGui::GetMousePos();
  
  ImVec2 min = ImVec2(itemPos.x, itemPos.y);
  ImVec2 max = ImVec2(itemPos.x + itemSize.x + pad.x * 2.0f, itemPos.y + itemSize.y + pad.y * 2.0f);
  
  ImColor bgCol = color;
  ImColor txtCol = bgCol;
  bgCol.Value.w = 0.15f;
  
  bool hovered = false;
  if (mousePos.x >= min.x && mousePos.y >= min.y && mousePos.x <= max.x && mousePos.y <= max.y && selectedMarker == nullptr) {
    txtCol.Value.x *= 1.5f;
    txtCol.Value.y *= 1.5f;
    txtCol.Value.z *= 1.5f;
    
    bgCol.Value.x *= 1.25f;
    bgCol.Value.y *= 1.25f;
    bgCol.Value.z *= 1.25f;
    bgCol.Value.w = 0.25f;
    
    hovered = true;
    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    clicked ? (*clicked = ImGui::IsMouseDown(ImGuiMouseButton_Left)) : 0;
  }
  
  if (!quiet || (quiet && hovered)) drawlist->AddRectFilled(min, max, bgCol, round);
  drawlist->AddText(ImVec2(itemPos.x + pad.x, itemPos.y + pad.y), txtCol, text.c_str());
  
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + itemSize.x + pad.x * 2.0f);
  //ImGui::SetCursorPosY(ImGui::GetCursorPosY() + itemSize.y + pad.y * 2.0f);
  
//  ImDrawList *fgDrawlist = ImGui::GetForegroundDrawList();
//  fgDrawlist->AddCircleFilled(itemPos, 2.5f, ImColor(0.3f, 0.6f, 1.0f, 1.0f));
//  fgDrawlist->AddLine(itemPos, ImGui::GetMousePos(), ImColor(0.3f, 0.6f, 1.0f, 0.75f), 2.0f);
//  fgDrawlist->AddCircleFilled(ImGui::GetMousePos(), 2.5f, ImColor(0.3f, 0.6f, 1.0f, 1.0f));
  
  if (hovered) {
    Marker m;
    m.hovered = hovered;
    activeMarkers.push_back(m);
    selectedMarker = &activeMarkers.back();
  }
    
  return hovered;
}

template <typename T>
static std::pair<std::string, ImVec4> markerFormat(pointer<T> reference) {
  if constexpr (std::is_same<T, stSuperObject>::value) {
    std::string objectName = reference->name(game::nameResolver);
    ImVec4 bgCol = ImGui::ColorConvertU32ToFloat4(game::objectColor(reference));
    return { objectName, bgCol };
  } else if constexpr (std::is_same<T, stBrain>::value) {
    std::string objectName = "Brain";
    ImVec4 bgCol = ImGui::ColorConvertU32ToFloat4(game::objectColor(reference));
    return { objectName, bgCol };
  }
}

template <>
void marker<stSuperObject>(pointer<stSuperObject> reference, bool readonly, std::string customText, bool quiet) {
  std::pair<std::string, ImVec4> data = markerFormat(reference);
  markerElement(customText.length() != 0 ? customText : data.first, quiet, data.second, defaultPadding, defaultRounding);
}

template <>
void marker<stBrain>(pointer<stBrain> reference, bool readonly, std::string customText, bool quiet) {
  std::pair<std::string, ImVec4> data = markerFormat(reference);
  markerElement(customText.length() != 0 ? customText : data.first, quiet, data.second, defaultPadding, defaultRounding);
}

void marker(CPA::Script::TranslationToken tok, bool readonly, std::string customText, bool quiet) {
  switch (tok.originalNode->type) {
    case Script::ScriptNodeType::ActorRef:
      markerElement(customText, quiet, ImVec4(229.0f / 255.0f, 193.0f / 255.0f, 124.0f / 255.0f, 1.0f), ImVec2(0.0f, 0.0f), defaultRounding);
      break;
      
    case Script::ScriptNodeType::DsgVarRef:
    case Script::ScriptNodeType::DsgVarRef2:
      markerElement(customText, quiet, ImVec4(0.9, 0.4, 0.45, 1.0f), ImVec2(0.0f, 0.0f), defaultRounding);
      break;
      
    case Script::ScriptNodeType::Subroutine:
      if (markerElement(customText, quiet, ImVec4(84.0f / 255.0f, 222.0f / 255.0f, 101.0f / 255.0f, 1.0f), ImVec2(0.0f, 0.0f), defaultRounding)) {
        AIWindow w(nullptr);
        ImGui::SetNextWindowPos(ImGui::GetMousePos());
        ImGui::SetNextWindowSize(ImVec2(700,500));
        ImGui::Begin(customText.c_str());
        w.setTargetMacro(pointer<stMacro>(uint32_t(tok.originalNode->param)));
        w.drawScript();
        ImGui::End();
      }
      break;
      
    default:
      break;
  }
}

void clearMarkers() {
  activeMarkers.clear();
  selectedMarker = nullptr;
}
