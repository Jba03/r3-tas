#include "gui.hh"

CinematicWindow::CinematicWindow() {
  /* ... */
}

void CinematicWindow::draw() {
  if (game::g_stEngineStructure->cineManager) {
    ImGui::Begin("Cinemanager");
    game::g_stEngineStructure->cineManager->cineList.forEach([&](stCine *cine, void*) {
      ImColor color = cine->playing ? ImColor(1.0f, 0.0f, 0.0f, 1.0f) : ImColor(1.0f, 1.0f, 1.0f, 0.5f);
      ImGui::TextColored(color, "%s, next = %X", cine->name, cine->next.pointeeAddress().effectiveAddress());
    });
    
    ImGui::End();
  }
}
