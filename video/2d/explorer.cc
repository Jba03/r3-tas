#include "gui.hh"

static const std::map<std::string, pointer<>> globals {
  {"g_stEngineStructure", game::g_stEngineStructure},
  {"g_stRandomStructure", game::g_stRandomStructure},
  {"g_stInputStructure", game::g_stInputStructure},
};

StructureExplorerWindow::StructureExplorerWindow() {
  /* ... */
}

void drawElement(pointer<stSuperObject> root, void*) {
  try {
    ImGui::PushID(static_cast<void*>(root));
    ImGui::PushStyleColor(ImGuiCol_Text, game::objectColor(root));
//    
//    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None | (root->numChildren == 0 ? ImGuiTreeNodeFlags_Leaf : 0);
//    if (ImGui::TreeNodeEx(root->typeName().c_str(), flags, "%s", overrideName(root).c_str())) {
//      ImVec4 col = ImGui::GetStyleColorVec4(ImGuiCol_Text);
//      ImGui::PopStyleColor();
//      gui::drawPopup();
//      ImGui::PushStyleColor(ImGuiCol_Text, col);
//      //root->forEachChild(drawRecursive);
//      ImGui::TreePop();
//    }
    
    ImGui::PopStyleColor();
    ImGui::PopID();
    
    // Popup
    //ImGui::OpenPopupOnItemClick("SPO", ImGuiMouseButton_Right);
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
      gui::popupTarget = root;
      ImGui::OpenPopup("SPO");
      printf("click\n");
    }
  } catch (BadPointer& e) {
    /* ... */
  }
}

void StructureExplorerWindow::draw() {
  ImGui::Begin("Structure Explorer");
  for (auto& v : globals) {
    ImGui::Text("%s", v.first.c_str());
  }
  ImGui::End();
}
