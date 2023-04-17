//
//  bits.cpp
//  r3-tas-memorymap
//
//  Created by Jba03 on 2022-12-28.
//

#include <vector>
#include <bitset>

#include "log.h"
#include "configuration.h"

static ImVec4 bit_on_color = ImColor(0.5f, 0.5f, 0.5f);
static ImVec4 bit_off_color = ImColor(0.3f, 0.3f, 0.3f);

template <typename T>
static void DisplayBit(T* data, bool on, int idx, std::string description = "")
{
    ImGui::TextColored(on ? bit_on_color : bit_off_color, "%1d", on ? 1 : 0);
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Bit %d", idx);
        ImGui::Text("%s", description.c_str());
        ImGui::EndTooltip();
    }
    
    if (ImGui::IsItemClicked() && configuration.cheats.enabled)
    {
        info("Toggle bit %d\n", idx);
        *(uint32_t*)data ^= game_byteorder_32((1 << idx));
    }
}

template <typename T>
static void DisplayBits(T* data, bool all = true, long descriptionLength = 0, const char * const * description = nullptr)
{
    std::bitset bits = std::bitset<sizeof(T)*8>((uint32_t)host_byteorder_32((*(T*)data)));
    
    size_t total = bits.count();
    if (true) total = sizeof((*(T*)data)) * 8;
    
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
        if (total < descriptionLength)
            DisplayBit(data, bits[total], total, description[total]);
        else
            DisplayBit(data, bits[total], total);
    }
    
    ImGui::PopStyleVar();
    
}
