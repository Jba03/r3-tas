//
//  cinematic.cpp
//  r3-tas
//
//  Created by Jba03 on 2023-08-06.
//

#include "gui.hh"
#include "game.hh"

static void draw() {
  if (game::g_stEngineStructure->cineManager) {
    ImGui::Begin("Cinemanager");
    game::g_stEngineStructure->cineManager->cineList.forEach([&](stCine *cine, void*) {
      ImColor color = cine->playing ? ImColor(1.0f, 0.0f, 0.0f, 1.0f) : ImColor(1.0f, 1.0f, 1.0f, 0.5f);
      ImGui::TextColored(color, "%s", cine->name);
    });
    
    ImGui::End();
  }
}

Window CinematicWindow = { &draw, nullptr, false };
