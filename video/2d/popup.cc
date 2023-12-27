#include "gui.hh"
#include "game.hh"

namespace gui {
  pointer<> popupTarget = nullptr;
  
  auto superObjectPopup() -> void {
    if (ImGui::BeginPopup("SPO")) {
      if (ImGui::Button("View object")) {
        superObjectWindow window(popupTarget);
        gui::spoWindows.push_back(window);
      }

      if (ImGui::Button("View in memory editor")) {
        memoryEditor.GotoAddr = popupTarget.pointeeAddress().effectiveAddress();
      }
      
      ImGui::EndPopup();
    }
  }
  
  auto drawPopup() -> void {
    //ImGui::OpenPopup(name);
    
    ImGuiStyle style = ImGui::GetStyle();
    
    superObjectPopup();
    
    ImGui::GetStyle() = style;
  }
}
