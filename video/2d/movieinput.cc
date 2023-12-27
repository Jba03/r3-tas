//
//  movieinput.cpp
//  r3-tas
//
//  Created by Jba03 on 2023-07-07.
//

#include "gui.hh"
#include "game.hh"

static void DrawInputLine()
{
    if (!hierarchy) return;
    
    ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, (rand() % 2) ? 0.5 : 1.0f);
    
    const float mx = host_byteorder_f32(input.stick.main.x->analogValue);
    const float my = host_byteorder_f32(input.stick.main.y->analogValue);
    float mA = 180.0f - degrees(atan2(mx, my));
    
    const float cx = host_byteorder_f32(input.stick.c.x->analogValue);
    const float cy = host_byteorder_f32(input.stick.c.y->analogValue);
    float cA = 180.0f - degrees(atan2(cx, cy));
    
    //ImGui::GetIO().
    
//    ImGui::TextColored(color, "A");
//    ImGui::SameLine();
//    ImGui::TextColored(color, "B");
//    ImGui::SameLine();
//    ImGui::TextColored(color, "X");
//    ImGui::SameLine();
//    ImGui::TextColored(color, "Y");
//    ImGui::SameLine();
//    ImGui::TextColored(color, "Z");
//    ImGui::SameLine();
//    ImGui::TextColored(color, "S");
//    ImGui::SameLine();
//    ImGui::TextColored(color, "%.0f°", mA);
//    ImGui::SameLine();
//    ImGui::TextColored(color, "%.0f°", cA);
}

static void Draw()
{
    ImGui::Begin("Movie input roll");
    {
        for (int i = 0; i < 10; i++)
        {
            DrawInputLine();
        }
        
        
    }
    ImGui::End();
}

const Window MovieInputWindow =
{
    &Draw
};
