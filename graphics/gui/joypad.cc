//
//  joypad.cc
//  r3-tas
//
//  Created by Jba03 on 2022-12-21.
//

static bool joypad_display = true;
static const float joystick_radius = 35.0f;

static void draw_joystick(ImDrawList* drawlist, ImVec2 pos, float radius, int joyx, int joyy)
{
    drawlist->AddRectFilled(pos, ImVec2(pos.x + joystick_radius * 2, pos.y + joystick_radius * 2), IM_COL32(255, 255, 255, 5));
    drawlist->AddCircleFilled(ImVec2(pos.x + joystick_radius, pos.y + joystick_radius), joystick_radius, IM_COL32(255, 255, 255, 20));
    drawlist->AddCircleFilled(ImVec2(pos.x + joystick_radius, pos.y + joystick_radius), 2.0f, IM_COL32(255, 255, 255, 20));
    
    float x = (float(joyx - 128) / 128.0f) * joystick_radius;
    float y = (float(joyy - 128) / 128.0f) * joystick_radius;
    
    ImVec2 linestart = ImVec2(pos.x + joystick_radius, pos.y + joystick_radius);
    ImVec2 lineend = ImVec2(pos.x + joystick_radius + x, pos.y + joystick_radius - y);
    drawlist->AddLine(linestart, lineend, IM_COL32(255, 0, 0, 196));
}

static void display_joypad()
{
    ImGuiWindowFlags flags =
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoBackground |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize;
    
    ImGui::Begin("Input display", &joypad_display, flags);
    ImDrawList* drawlist = ImGui::GetWindowDrawList();
    
    ImGui::SetWindowSize(ImVec2(joystick_radius * 2.0f * 3.0f, joystick_radius * 2.0f + 100));
    ImGui::SetWindowPos(ImVec2(0, display_size.y - joystick_radius * 2.0f - 50));
    
    ImVec2 ctrlstickpos = ImGui::GetCursorScreenPos();
    ImVec2 cstickpos = ImGui::GetCursorScreenPos();
    ctrlstickpos.y += 15;
    cstickpos.y += 15;
    cstickpos.x += joystick_radius * 2.0f + 5.0f;
    
    draw_joystick(drawlist, ctrlstickpos, joystick_radius, input.joymain_x, input.joymain_y);
    draw_joystick(drawlist, cstickpos, joystick_radius, input.joyc_x, input.joyc_y);
    
    ctrlstickpos.y -= 15;
    cstickpos.y -= 15;
    
    ImU32 col = IM_COL32(255, 255, 255, 30);
    if (joystick_radius >= 35.0f)
    {
        drawlist->AddText(ctrlstickpos, col, std::string("M(" + std::to_string(input.joymain_x) + "," + std::to_string(input.joymain_y) + ")").c_str());
        drawlist->AddText(cstickpos, col, std::string("C(" + std::to_string(input.joyc_x) + "," + std::to_string(input.joyc_y) + ")").c_str());
    }
    else
    {
        drawlist->AddText(ctrlstickpos, col, "Main");
        drawlist->AddText(cstickpos, col, "C");
    }
      
    ImU32 on = IM_COL32(255, 255, 255, 128);
    ImU32 off = col;
    
    const float yy = ctrlstickpos.y + joystick_radius * 2.0f + 20;
    drawlist->AddText(ImVec2(ctrlstickpos.x + 00, yy), input.a ? on : off, "A");
    drawlist->AddText(ImVec2(ctrlstickpos.x + 10, yy), input.b ? on : off, "B");
    drawlist->AddText(ImVec2(ctrlstickpos.x + 20, yy), input.x ? on : off, "X");
    drawlist->AddText(ImVec2(ctrlstickpos.x + 30, yy), input.y ? on : off, "Y");
    drawlist->AddText(ImVec2(ctrlstickpos.x + 40, yy), input.z ? on : off, "Z");
    drawlist->AddText(ImVec2(ctrlstickpos.x + 50, yy), input.l ? on : off, "L");
    drawlist->AddText(ImVec2(ctrlstickpos.x + 60, yy), input.r ? on : off, "R");
    drawlist->AddText(ImVec2(ctrlstickpos.x + 70, yy), input.R ? on : off, "CAM");
    drawlist->AddText(ImVec2(ctrlstickpos.x + 95, yy), input.S ? on : off, "START");
    
    ImGui::End();
}