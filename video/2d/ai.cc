#include "gui.hh"

void AIWindow::drawIntelligenceBehaviorList() {
  try {
    pointer<stScriptAI> list = targetObject->actor->aiModel()->intelligenceBehaviorList;
    pointer<stBehavior> currentBehavior = targetObject->actor->brain->mind->intelligence->currentBehavior;
    if (ImGui::BeginCombo("Intelligence", currentBehavior->name.lastPathComponent().c_str())) {
      for (int i = 0; i < list->numBehaviors; i++) {
        pointer<stBehavior> behavior = list->behavior[i];
        ImGui::Selectable(behavior->name.lastPathComponent().c_str(), behavior == currentBehavior);
      }
      ImGui::EndCombo();
    }
  } catch (CPA::BadPointer& e) {
    std::cout << "Failed to draw behavior list: " << e.what() << "\n";
  }
}

void AIWindow::drawReflexBehaviorList() {
  try {
    pointer<stScriptAI> list = targetObject->actor->aiModel()->reflexBehaviorList;
    pointer<stBehavior> currentBehavior = targetObject->actor->brain->mind->reflex->currentBehavior;
    if (ImGui::BeginCombo("Reflex", currentBehavior->name.lastPathComponent().c_str())) {
      for (int i = 0; i < list->numBehaviors; i++) {
        pointer<stBehavior> behavior = list->behavior[i];
        ImGui::PushStyleColor(ImGuiCol_Text, behavior == currentBehavior ? ImVec4(0.1f, 1.0f, 0.25, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::Selectable(behavior->name.lastPathComponent().c_str(), behavior == currentBehavior);
        ImGui::PopStyleColor();
      }
      ImGui::EndCombo();
    }
  } catch (CPA::BadPointer& e) {
    std::cout << "Failed to draw behavior list: " << e.what() << "\n";
  }
}

AIWindow::AIWindow(pointer<stSuperObject> target) {
  targetObject = target;
}

void AIWindow::draw() {
  ImGui::Begin("AI");
  if (targetObject->type == eSuperObjectType::superObjectTypeActor) {
    drawIntelligenceBehaviorList();
    drawReflexBehaviorList();
  } else {
    ImGui::Text("Invalid target actor");
  }
  ImGui::End();
}
