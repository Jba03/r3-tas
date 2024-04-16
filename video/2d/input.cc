#include "gui.hh"

static void drawJoystick(ImDrawList *drawlist, ImVec2 pos, const float radius, int8_t joyX, int8_t joyY, ImColor joyCol, ImColor bgCol = IM_COL32(255, 255, 255, 20)) {
  drawlist->AddText(pos, IM_COL32(255, 255, 255, 128), std::string("(" + std::to_string(joyX) + "," + std::to_string(joyY) + ")").c_str());
  drawlist->AddRectFilled(pos, ImVec2(pos.x + radius * 2, pos.y + radius * 2), IM_COL32(255, 255, 255, 5));
  drawlist->AddCircleFilled(ImVec2(pos.x + radius, pos.y + radius), radius, bgCol);
  drawlist->AddCircleFilled(ImVec2(pos.x + radius, pos.y + radius), 2.0f, bgCol);
  
  float x = float(joyX) / 100.0f * radius;
  float y = float(joyY) / 100.0f * radius;
  
  ImVec2 start = ImVec2(pos.x + radius, pos.y + radius);
  ImVec2 end = ImVec2(pos.x + radius + x, pos.y + radius + y);
  drawlist->AddLine(start, end, joyCol);
  
  ImGui::SetCursorScreenPos(ImVec2(pos.x + radius * 2.0f + 4, pos.y));
}

stVector2D jostickFromCamera(stVector3D worldDirection) {
  stVector3D i, j, k;
  pointer<stTransform> cameraTransform = game::g_stEngineStructure->standardCamera->globalTransform;
  cameraTransform->getRotation(i, j, k);
  j.z = 0.0f;
  if (j.length() == 0.0f) (j = k).z = 0.0f;
  stVector3D normJ = j.normalize();
  
  
  stVector3D k2 = stVector3D(0.0f, 0.0f, 1.0f);
  stVector3D i2 = game::g_stEngineStructure->currentMainPlayers[0]->position();
  
  i = j.cross(k);
  
  stMatrix3D mat = stMatrix3D::identity();
  *(stVector3D*)&mat(0,0) = i;
  *(stVector3D*)&mat(1,0) = j;
  *(stVector3D*)&mat(2,0) = k;
  
//  stMatrix4D inverseCameraTransform = cameraTransform.inverse();
//  stVector2D joystick = (inverseCameraTransform * worldDirection).xy().normalize() * 100;
  
  //mat = mat.transpose();
  
  stVector2D joystick = (mat * worldDirection).xy().normalize() * 100.0f;
  
  return joystick;
}


std::deque<stVector3D> targets = {
//  stVector3D(-77.0f, 41.45f, -5.32f),
//  stVector3D(-36.90f, 11.17f, -5.16f),
//  stVector3D(-10.38, 22.24, -4.92f),
  
// CF1:
//  stVector3D(-8.20, 63.09, 18.36),
//  stVector3D(-2.70, 50.9, 19.43),
//  stVector3D(10.12, 56.82, 17.72),
//  stVector3D(2.56, 77.22, 18.30),
//  stVector3D(46.10, 59.44, -0.74),
//  stVector3D(62.73, 35.56, -2.52),
};

static float cameraAngle = 0.0f;
static bool running = false;

stVector3D genericDirection;
stVector3D realDirection;

stVector3D targetPos;
static bool useTarget = false;

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
    
    stInputDevice device = game::g_stInputStructure->device[0];
    stPadReadingOutput pad = *(stPadReadingOutput*)pointer<stPadReadingOutput>(0x8042F8F8); //device.padReadOutput;
    
   // printf("addr: %X\n", game::g_stInputStructure->device[0].valid.memoryOffset().effectiveAddress());
    
//    stVector3D globalVector = pad.globalVector;
//    int16 horizontalAxis = pad.horizontalAxis;
//    int16 verticalAxis;
//    float32 analogForce;
//    float32 trueAnalogForce;
//    float32 rotationAngle;
//    int32 strafeSector;
    
//    printf("pad vector: (%.2f, %.2f, %.2f)\n", float(pad.globalVector.x), float(pad.globalVector.y), float(pad.globalVector.z));
//    printf("pad x axis: %d\n", int(pad.horizontalAxis));
//    printf("pad y axis: %d\n", int(pad.verticalAxis));
//    printf("pad analog force: %f\n", float(pad.analogForce));
//    printf("pad true analog force: %f\n", float(pad.trueAnalogForce));
//    printf("pad rotation angle: %f\n", float(pad.rotationAngle));
//    printf("pad strafe sector: %d\n", int(pad.strafeSector));
//    printf("\n");
    
    ImDrawList *drawlist = ImGui::GetWindowDrawList();
    drawJoystick(drawlist, ImGui::GetCursorScreenPos(), joystickRadius, float(mX->analogValue), float(mY->analogValue), joystickColor);
    drawJoystick(drawlist, ImGui::GetCursorScreenPos(), joystickRadius, float(cX->analogValue), float(cY->analogValue), joystickColor);
    
    //drawJoystick(drawlist, ImGui::GetCursorScreenPos(), joystickRadius, float(pad.globalVector.x), float(pad.globalVector.y), ImVec4(1.0f, 0.4f, 0.1f, 1.0f));

//    game::g_stEngineStructure->inputMode = running ? stEngineStructure::inputMode::Commands : stEngineStructure::inputMode::Normal;
    stVector3D pos = game::g_stEngineStructure->currentMainPlayers[0]->position();
//
//    if (running) {
//      genericDirection = targets.size() > 0 ? -(targets.front() - pos) : stVector3D();
//      realDirection = (targets.front() - pos);
//      if ((pos - targets.front()).length() < 0.5f) targets.pop_front();
//      if (targets.size() == 0) running = false;
//
//      //genericDirection = -stVector3D(62.73f, 35.56f, -2.52f); //stVector3D(-20.0, -6.0, 0.0f);
//      genericDirection.y = -float(genericDirection.y);
//      genericDirection.z = 0.0f;
//
//
    genericDirection = -(targetPos - pos);
    genericDirection.y = -float(genericDirection.y);
    genericDirection.z = 0.0f;
    stVector2D joy = jostickFromCamera(genericDirection.normalize());
//      mX->analogValue = joy.x;
//      mY->analogValue = joy.y;
//
//      cX->analogValue = -100.0f; //joy.y; //sin(cameraAngle) * 100.0f;
//      cY->analogValue = -50.0f;//joy.x; // -100.0f; //sin(cameraAngle) * 100.0f;
//
//      cameraAngle += 0.035f;
      
      if (useTarget) drawJoystick(drawlist, ImGui::GetCursorScreenPos(), joystickRadius, float(joy.x), float(joy.y), ImVec4(1.0f, 0.4f, 0.1f, 1.0f));
   // }
      
//    //printf("running: %d, targets: %d\n", running, targets.size());
//    if (ImGui::Button("start")) running = true;
//    if (ImGui::Button("Add")) targets.push_back(pos);
    
    if (ImGui::Button("Add target here")) { useTarget = true; targetPos = pos; }
    if (ImGui::Button("Remove target")) useTarget = false;
    
  } catch (...) {
    /* ... */
  }
    
  
  ImGui::End();
}
