#include "gui.hh"
#include "game.hh"
#include "graphics.hh"

#include "dynamics.cc"

static auto actor(stSuperObject *spo) -> void {
  stEngineObject *obj = spo->actor;
  // Name hierarchy
  ImColor color = ImColor(game::objectColor(spo));
  ImGui::TextColored(color, "%s -", obj->familyName().c_str());
  
  color.Value.w = 0.6f;
  ImGui::SameLine();
  ImGui::TextColored(color, "%s -", obj->modelName().c_str());
  
  color.Value.w = 0.45f;
  ImGui::SameLine();
  ImGui::TextColored(color, "%s", obj->instanceName().c_str());
  
  if (ImGui::BeginTabBar("Actor")) {
    
    if (obj->dynam) {
      if (obj->dynam->dynamics) {
        if (ImGui::BeginTabItem("Dynamics")) {
          drawDynamics(obj->dynam->dynamics);
          ImGui::EndTabItem();
        }
      }
    }
    
    if (obj->brain) {
      if (ImGui::BeginTabItem("AI")) {
        ImGui::EndTabItem();
      }
    }
    
    ImGui::EndTabBar();
  }
}

static auto sector(stSuperObject *spo) -> void {
  stSector *sector = spo->sector;
  
  ImGui::Text("%s", sector->name.lastPathComponent().c_str());
  ImGui::Text("Min: [%.2f, %.2f, %.2f]", float(sector->min.x), float(sector->min.y), float(sector->min.z));
  ImGui::Text("Max: [%.2f, %.2f, %.2f]", float(sector->max.x), float(sector->max.y), float(sector->max.z));
  
  if (ImGui::Button("Highlight in 3D view")) {
    graphics::highlightList.push_back(spo);
  }
}

static auto ipo(stSuperObject *spo) -> void {
  if (ImGui::Button("Highlight in 3D view")) {
    graphics::highlightList.push_back(spo->ipo);
  }
}

static auto physicalObject() -> void {
  
}

auto superObjectWindow::draw() -> void {
  if (open) {
    ImGui::Begin(std::string("SPO - " + target->name()).c_str(), &open);
    switch (target->type) {
      case superobjectTypeActor: actor(target);
      case superobjectTypeSector: sector(target);
      case superobjectTypeIPO: ipo(target);
      case superobjectTypePhysicalObject: physicalObject();
    }
    ImGui::End();
  }
}
