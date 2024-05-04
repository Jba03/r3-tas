#include "gui.hh"
#include "game.hh"

namespace gui {
  pointer<> popupTarget = nullptr;
  
  auto superObjectPopup() -> void {
//    if (game::isValidGameState()) {
//      pointer<stSuperObject> spo = pointer<stSuperObject>(0x810B0CD8);
//      spo->ipo->physicalObject->physicalCollideset = nullptr;
//    }
    if (ImGui::BeginPopup("SPO")) {
      if (ImGui::Button("View object")) {
        superObjectWindow window(popupTarget);
        gui::spoWindows.push_back(window);
      }

      if (ImGui::Button("View in memory editor")) {
        //memoryEditor.GotoAddr = popupTarget.pointeeAddress().effectiveAddress();
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
