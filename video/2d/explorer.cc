#include "gui.hh"

static const std::map<std::string, pointer<>> globals {
  {"g_stEngineStructure", game::g_stEngineStructure},
  {"g_stRandomStructure", game::g_stRandomStructure},
  {"g_stInputStructure", game::g_stInputStructure},
};

StructureExplorerWindow::StructureExplorerWindow() {
  /* ... */
}

void drawElement(serializer::node root, void*) {
  try {
    //ImGui::PushID(static_cast<void*>(root));
    //ImGui::PushStyleColor(ImGuiCol_Text, game::objectColor(root));
    
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None | (root.children().size() == 0 ? ImGuiTreeNodeFlags_Leaf : 0);
    if (ImGui::TreeNodeEx(root.name().c_str(), flags, "%s: %s", root.name().c_str(), root.value().c_str())) {
      ImVec4 col = ImGui::GetStyleColorVec4(ImGuiCol_Text);
      //ImGui::PopStyleColor();
      //gui::drawPopup();
      //ImGui::PushStyleColor(ImGuiCol_Text, col);
      for (auto& child : root.children()) drawElement(child, nullptr);
      //root->forEachChild(drawElement);
      ImGui::TreePop();
    }
    
   //ImGui::PopStyleColor();
    //ImGui::PopID();
  } catch (bad_pointer& e) {
    /* ... */
    ImGui::TreePop();
  }
}

void StructureExplorerWindow::draw() {
  ImGui::Begin("Structure Explorer");
  
  if (game::isValidGameState()) {
    serializer ser;
    game::g_stRandomStructure->serialize(ser);
    
    drawElement(ser, nullptr);
  }
//  for (auto& v : globals) {
//    drawElement(v, <#void *#>)
//   // ImGui::Text("%s", v.first.c_str());
//  }
  ImGui::End();
}
