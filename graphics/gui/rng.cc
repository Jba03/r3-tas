//
//  rng.cc
//  r3-tas
//
//  Created by Jba03 on 2022-12-07.
//

static int previous_rng_index = 0;

static bool rng_display_as_decimal = false;

static bool calc_rng = false;
static ImVec2 calc_rng_pos;
static int calc_rng_value = 0;
static int calc_rng_min = 0;
static int calc_rng_max = 5;

void display_calculate_rng()
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_Modal;
    
    ImGui::Begin("Calculate RNG value", &calc_rng, flags);
    ImGui::InputInt("value", &calc_rng_value);
    ImGui::InputInt("min", &calc_rng_min);
    ImGui::InputInt("max", &calc_rng_max);
    
    const uint32_t Tmax = host_byteorder_32(rnd->tableMax);
    const int R = (calc_rng_min + ((calc_rng_max - calc_rng_min + 1) * calc_rng_value) / (Tmax + 1));
    ImGui::Text("Result: %d", R);
    
    ImGui::SetWindowSize(ImVec2(200, 125));
    
    ImGui::End();
}

void display_rng_info()
{
    const char* fmt = "RNG(%d, 0x%X, 0x%X)";
    if (rng_display_as_decimal) fmt = "RNG(%d, %d, %d)";
}

void display_rng_table()
{
    #define RNG_TABLE_DISPLAY 20
    uint32_t base_index = host_byteorder_32(rnd->tableIndices[0]);
    if (base_index == 0) previous_rng_index = 0;
    
    std::string fmt = "0x%X: 0x%X";
    if (rng_display_as_decimal) fmt = "%d: %d";
    
    for (int i = -RNG_TABLE_DISPLAY / 3; i < RNG_TABLE_DISPLAY; i++)
    {
        uint32_t value = fnRNDTableIndex(rnd, 0, i);
        
        float a = 1.0f - float(i) / float(RNG_TABLE_DISPLAY) + 0.1f;
        
        std::string marker = "";
        
        ImVec4 color = ImVec4(0.5, 0.5, 1.0, a);
        if (i < 0) color = ImVec4(1.0f, 0.2f, 0.3f, 0.4f);
        if (base_index + i == previous_rng_index || i == 0)
        {
            color.w = 0.75f;
            marker = "->";
        }
        
        if (base_index + i < host_byteorder_32(rnd->tableSize))
        {
            ImGui::TextColored(color, ("%-2s " + fmt).c_str(), marker.c_str(), base_index + i, value);
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::TextColored(color, fmt.c_str(), base_index + i, value);
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Left click to open calculator");
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Right click to view as %s", rng_display_as_decimal ? "hex" : "decimal");
                ImGui::EndTooltip();
            }

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                if (!calc_rng)
                {
                    ImVec2 pos = ImGui::GetMousePos();
                    pos.x -= 100;
                    ImGui::SetNextWindowPos(pos);
                }
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
