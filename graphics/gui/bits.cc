//
//  bits.cpp
//  r3-tas-memorymap
//
//  Created by Jba03 on 2022-12-28.
//

#include <vector>
#include <bitset>

static ImVec4 bit_on_color = ImColor(0.5f, 0.5f, 0.5f);
static ImVec4 bit_off_color = ImColor(0.3f, 0.3f, 0.3f);

static void DisplayBit(bool on, int idx, std::string description = "")
{
    ImGui::TextColored(on ? bit_on_color : bit_off_color, "%1d", on ? 1 : 0);
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Bit %d", idx);
        ImGui::Text("%s", description.c_str());
        ImGui::EndTooltip();
    }
}

template <typename T>
static void DisplayBits(T data, bool all = true, std::vector<std::string> description = {})
{
    std::bitset bits = std::bitset<sizeof(T)*8>(data);
    
    size_t total = bits.count();
    if (true) total = sizeof(data) * 8;
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::TextColored(bit_off_color, "0b");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Hex: %X", data);
        ImGui::Text("Decimal: %d", data);
        ImGui::EndTooltip();
    }
    
    size_t i = total;
    while (total--)
    {
        ImGui::SameLine();
        if (total < description.size())
            DisplayBit(bits[total], total, description.at(total));
        else
            DisplayBit(bits[total], total);
    }
    
    ImGui::PopStyleVar();
    
}
