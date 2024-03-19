#include "gui.hh"

static void drawJoystick(ImDrawList *drawlist, ImVec2 pos, const float radius, int8_t joyX, int8_t joyY, ImColor joyCol, ImColor bgCol = IM_COL32(255, 255, 255, 20)) {
  drawlist->AddRectFilled(pos, ImVec2(pos.x + radius * 2, pos.y + radius * 2), IM_COL32(255, 255, 255, 5));
  drawlist->AddCircleFilled(ImVec2(pos.x + radius, pos.y + radius), radius, bgCol);
  drawlist->AddCircleFilled(ImVec2(pos.x + radius, pos.y + radius), 2.0f, bgCol);
  
  float x = float(joyX) / 100.0f * radius;
  float y = float(joyY) / 100.0f * radius;
  
  ImVec2 start = ImVec2(pos.x + radius, pos.y + radius);
  ImVec2 end = ImVec2(pos.x + radius + x, pos.y + radius + y);
  drawlist->AddLine(start, end, joyCol);
}

void InputWindow::draw() {
  ImGui::Begin("Input");
  
  try {
//    pointer<stInputStructure> ipt = game::g_stInputStructure;
//    printf("a\n");
//    float x = float(ipt->device[0].joyAxisR->analogValue);
//    float y = 0;//ipt->device[0].joyAxisY->analogValue;
//    printf("x, y: %f, %f\n", x, y);
//    printf("%X\n", ipt->device[1].joyCenterY.memoryOffset().effectiveAddress());
//
    pointer<stInputEntryElement> mX = game::findInputEntryElement("Action_Pad0_AxeX");
    pointer<stInputEntryElement> mY = game::findInputEntryElement("Action_Pad0_AxeY");
    pointer<stInputEntryElement> cX = game::findInputEntryElement("Action_Pad0_AxeV");
    pointer<stInputEntryElement> cY = game::findInputEntryElement("Action_Pad0_AxeZ");
    
    float x = float(mX->analogValue);
    float y = float(mY->analogValue);
    
    ImDrawList *drawlist = ImGui::GetWindowDrawList();
    drawJoystick(drawlist, ImGui::GetCursorScreenPos(), joystickRadius, x, y, joystickColor);
  } catch (...) {
    /* ... */
  }
    
  
  ImGui::End();
}
