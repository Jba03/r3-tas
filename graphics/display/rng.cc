//
//  rng.c
//  r3-tas
//
//  Created by Jba03 on 2022-12-07.
//

#include "global.h"

static int previous_rng_index = 0;

static bool rng_display_as_decimal = false;

static bool calc_rng = false;
static ImVec2 calc_rng_pos;
static int calc_rng_value = 0;
static int calc_rng_min = 0;
static int calc_rng_max = 1;

void display_calculate_rng()
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_Modal;
    
    ImGui::Begin("Calculate RNG value", &calc_rng, flags);
    ImGui::InputInt("value", &calc_rng_value);
    ImGui::InputInt("min", &calc_rng_min);
    ImGui::InputInt("max", &calc_rng_max);
    
    const int R = (calc_rng_min + ((calc_rng_max - calc_rng_min + 1) * calc_rng_value) / (rnd.table_max + 1));
    ImGui::Text("Result: %d", R);
    
    ImGui::End();
}

void display_rng_info()
{
    const char* fmt = "RNG(0x%X, 0x%X)";
    if (rng_display_as_decimal) fmt = "RNG(%d, %d)";
    
    uint32_t value = rnd_table_index(rnd, 0, 0);
    ImGui::TextColored(ImVec4(1.0, 1.0, 1.0, 1.0), fmt, value, rnd.table_max);
}

void display_rng_table()
{
    #define RNG_TABLE_DISPLAY 20
    uint32_t base_index = memory.read_32(rnd.ptr_table_indices);
    if (base_index == 0) previous_rng_index = 0;
    
    const char* fmt = "0x%X: 0x%X";
    if (rng_display_as_decimal) fmt = "%d: %d";
    
    for (int i = -RNG_TABLE_DISPLAY / 3; i < RNG_TABLE_DISPLAY; i++)
    {
        uint32_t value = rnd_table_index(rnd, 0, i);
        
        float a = 1.0f - float(i) / float(RNG_TABLE_DISPLAY) + 0.1f;
        
        ImVec4 color = ImVec4(0.5, 0.5, 1.0, a);
        if (i < 0) color = ImVec4(1.0f, 0.2f, 0.3f, 0.4f);
        if (base_index + i == previous_rng_index) color.w = 0.75f;
        if (base_index + i < rnd.table_length)
        {
            ImGui::TextColored(color, fmt, base_index + i, value);
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                calc_rng_value = value;
                calc_rng = true;
            }
            
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                rng_display_as_decimal = !rng_display_as_decimal;
        }
    }
    
    if (calc_rng) display_calculate_rng();
    
    previous_rng_index = base_index;
}
