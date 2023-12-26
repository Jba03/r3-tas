#include "gui.hh"
#include "game.hh"

#include <type_traits>

static auto superObjectMenu(stSuperObject *spo) -> void {
  ImGui::Begin("Context menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
 
  if (ImGui::Button("View object")) {
    superObjectWindow window(spo);
    gui::spoWindows.push_back(window);
    gui::contextMenu = nullptr;
  }
  
  if (ImGui::Button("View in memory editor")) {
    gui::memoryEditor.GotoAddr = pointer<stSuperObject>(spo).pointeeAddress().effectiveAddress();
    gui::contextMenu = nullptr;
  }
  
  ImGui::End();
}

auto contextMenu::draw() -> void {
  ImGui::SetNextWindowPos(position);
  if (first) ImGui::SetNextWindowFocus();
  switch (type) {
    case superObject: superObjectMenu(target);
  }
  first = false;
}
