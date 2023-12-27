//
//  hierarchy.cpp
//  r3-tas
//
//  Created by Jba03 on 2023-07-16.
//


#include "gui.hh"
#include "game.hh"

using namespace game;

static bool Search = false;
static char SearchString[64];

static auto overrideName(stSuperObject *root) -> std::string  {
  if (!root) return "Invalid name";
  if (root == p_stActualWorld) return "Actual world";
  else if (root == p_stDynamicWorld) return "Dynamic world (" + std::to_string(root->numChildren) + ")";
  else if (root == p_stInactiveDynamicWorld) return "Inactive dynamic world (" + std::to_string(root->numChildren) + ")";
  else if (root == p_stFatherSector) return "Father sector (" + std::to_string(root->numChildren) + ")";
  return root->name();
}

static auto drawRecursive(stSuperObject *root, void*) -> void {
  if (!root) return;
  ImGui::PushID(root);
  ImGui::PushStyleColor(ImGuiCol_Text, game::objectColor(root));
  
  if (root->type == superobjectTypeSector) {
    if (root == g_stEngineStructure->currentMainPlayers[0]->currentSector(p_stFatherSector)) {
      ImGui::PopStyleColor();
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.1f, 1.0f));
    }
  }
  
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None | (root->numChildren == 0 ? ImGuiTreeNodeFlags_Leaf : 0);
  if (ImGui::TreeNodeEx(root->typeName().c_str(), flags, "%s", overrideName(root).c_str())) {
    ImVec4 col = ImGui::GetStyleColorVec4(ImGuiCol_Text);
    ImGui::PopStyleColor();
    gui::drawPopup();
    ImGui::PushStyleColor(ImGuiCol_Text, col);
    root->forEachChild(drawRecursive);
    ImGui::TreePop();
  }
  
  ImGui::PopStyleColor();
  ImGui::PopID();
  
  // Popup
  //ImGui::OpenPopupOnItemClick("SPO", ImGuiMouseButton_Right);
  if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
    gui::popupTarget = root;
    ImGui::OpenPopup("SPO");
    printf("click\n");
  }
}

static auto drawHierarchy() -> void {
  ImGuiStyle PreviousStyle = ImGui::GetStyle();
  ImGui::GetStyle().WindowRounding = 0.0f;
  ImGui::GetStyle().WindowBorderSize = 1.0f;
  ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.05f);
  ImGui::GetStyle().ScrollbarRounding = 0.0f;
    
  ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(-1, ImGui::GetIO().DisplaySize.y - 200));
  ImGui::SetNextWindowSize(ImVec2(-1, -1));
  ImGui::Begin("Actual world", &HierarchyWindow.Open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
  ImGui::SetWindowPos(ImVec2(0, 100));
  
  drawRecursive(p_stDynamicWorld, nullptr);
  drawRecursive(p_stInactiveDynamicWorld, nullptr);
  drawRecursive(p_stFatherSector, nullptr);
  
  gui::drawPopup();
    
  ImGui::End();
  ImGui::GetStyle() = PreviousStyle;
}

static void DrawSearchWindow() {
  if (Search) {
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(500, 400));
        
    ImGui::Begin("Search object");
    if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
      ImGui::SetKeyboardFocusHere(0);
        
    ImGui::InputText("", SearchString, 64);
        
    ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - ImGui::GetWindowSize().x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f - ImGui::GetWindowSize().y / 2.0f));
    ImGui::End();
  }
}

static void Draw() {
    if (isValidGameState()) {
      drawHierarchy();
      //DrawSearchWindow();
    }
}

Window HierarchyWindow = { Draw };
