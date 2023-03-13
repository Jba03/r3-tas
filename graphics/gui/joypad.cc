//
//  joypad.cc
//  r3-tas
//
//  Created by Jba03 on 2022-12-21.
//

static bool joypad_display = true;
static const float joystick_radius = 35.0f;
static const ImColor joystick_color = IM_COL32(255, 0, 0, 196);

static void draw_joystick(ImDrawList* drawlist, ImVec2 pos, float radius, int joyx, int joyy, ImColor joy = joystick_color, ImColor bg = IM_COL32(255, 255, 255, 20))
{
    drawlist->AddRectFilled(pos, ImVec2(pos.x + radius * 2, pos.y + radius * 2), IM_COL32(255, 255, 255, 5));
    drawlist->AddCircleFilled(ImVec2(pos.x + radius, pos.y + radius), radius, bg);
    drawlist->AddCircleFilled(ImVec2(pos.x + radius, pos.y + radius), 2.0f, bg);
    
    float x = (float(joyx) / 100.0f) * radius;
    float y = -(float(joyy) / 100.0f) * radius;
    
    ImVec2 linestart = ImVec2(pos.x + radius, pos.y + radius);
    ImVec2 lineend = ImVec2(pos.x + radius + x, pos.y + radius - y);
    drawlist->AddLine(linestart, lineend, joy);
}

static void display_joypad(bool *display)
{
    if (*display)
    {
        ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize;
        
        ImGui::Begin("Input display", display, flags);
        ImDrawList* drawlist = ImGui::GetWindowDrawList();
        
        ImGui::SetWindowSize(ImVec2(joystick_radius * 2.0f * 3.0f, joystick_radius * 2.0f + 100));
        ImGui::SetWindowPos(ImVec2(0, display_size.y - joystick_radius * 2.0f - 50));
        
        ImVec2 ctrlstickpos = ImGui::GetCursorScreenPos();
        ImVec2 cstickpos = ImGui::GetCursorScreenPos();
        ctrlstickpos.y += 15;
        cstickpos.y += 15;
        cstickpos.x += joystick_radius * 2.0f + 5.0f;
        
        const float mjoy_x = input.stick.main.x ? host_byteorder_f32(*(uint32_t*)&input.stick.main.x->analogvalue) : 0.0f;
        const float mjoy_y = input.stick.main.y ? host_byteorder_f32(*(uint32_t*)&input.stick.main.y->analogvalue) : 0.0f;
        const float cjoy_x = input.stick.c.x ? host_byteorder_f32(*(uint32_t*)&input.stick.c.x->analogvalue) : 0.0f;
        const float cjoy_y = input.stick.c.y ? host_byteorder_f32(*(uint32_t*)&input.stick.c.y->analogvalue) : 0.0f;
        
        draw_joystick(drawlist, ctrlstickpos, joystick_radius, mjoy_x, mjoy_y);
        draw_joystick(drawlist, cstickpos, joystick_radius, cjoy_x, cjoy_y);
        
        ctrlstickpos.y -= 15;
        cstickpos.y -= 15;
        
        ImU32 col = IM_COL32(255, 255, 255, 30);
        if (joystick_radius >= 35.0f)
        {
            drawlist->AddText(ctrlstickpos, col, std::string("M(" + std::to_string((int)mjoy_x) + "," + std::to_string((int)mjoy_y) + ")").c_str());
            drawlist->AddText(cstickpos, col, std::string("C(" + std::to_string((int)cjoy_x) + "," + std::to_string((int)cjoy_y) + ")").c_str());
        }
        else
        {
            drawlist->AddText(ctrlstickpos, col, "Main");
            drawlist->AddText(cstickpos, col, "C");
        }
        
        ImU32 on = IM_COL32(255, 255, 255, 128);
        ImU32 off = col;
        
        const bool a = input.button.a ? !(host_byteorder_32(input.button.a->state) & 0xFF000000) : false;
        const bool b = input.button.b ? !(host_byteorder_32(input.button.b->state) & 0xFF000000) : false;
        const bool x = input.button.x ? !(host_byteorder_32(input.button.x->state) & 0xFF000000) : false;
        const bool y = input.button.y ? !(host_byteorder_32(input.button.y->state) & 0xFF000000) : false;
        const bool S = input.button.S ? !(host_byteorder_32(input.button.S->state) & 0xFF000000) : false;
        const bool L = input.button.L ? !(host_byteorder_32(input.button.L->state) & 0xFF000000) : false;
        const bool R = input.button.R ? !(host_byteorder_32(input.button.R->state) & 0xFF000000) : false;
        const bool l = input.button.l ? !(host_byteorder_32(input.button.l->state) & 0xFF000000) : false;
        
        const float yy = ctrlstickpos.y + joystick_radius * 2.0f + 20;
        drawlist->AddText(ImVec2(ctrlstickpos.x + 00, yy), a ? on : off, "A");
        drawlist->AddText(ImVec2(ctrlstickpos.x + 10, yy), b ? on : off, "B");
        drawlist->AddText(ImVec2(ctrlstickpos.x + 20, yy), x ? on : off, "X");
        drawlist->AddText(ImVec2(ctrlstickpos.x + 30, yy), y ? on : off, "Y");
        //    drawlist->AddText(ImVec2(ctrlstickpos.x + 40, yy), input.z ? on : off, "Z");
        drawlist->AddText(ImVec2(ctrlstickpos.x + 50, yy), L ? on : off, "L");
        drawlist->AddText(ImVec2(ctrlstickpos.x + 60, yy), R ? on : off, "R");
        drawlist->AddText(ImVec2(ctrlstickpos.x + 70, yy), l ? on : off, "CAM");
        drawlist->AddText(ImVec2(ctrlstickpos.x + 95, yy), S ? on : off, "START");
        
        ImGui::End();
    }
}
