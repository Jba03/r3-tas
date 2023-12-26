//
//  rng.cc
//  r3-tas
//
//  Created by Jba03 on 2022-12-07.
//

//#include <iostream>
#include <string>
#include "gui.hh"
#include "game.hh"

static bool rng_display_as_decimal = false;

static bool calc_rng = false;
static ImVec2 calc_rng_pos;
static int calc_rng_value = 0;
static int calc_rng_min = 0;
static int calc_rng_max = 5;

using namespace game;

//
//void display_calculate_rng()
//{
//    ImGuiWindowFlags flags = ImGuiWindowFlags_Modal;
//    
//    ImGui::Begin("Calculate RNG value", &calc_rng, flags);
//    ImGui::InputInt("value", &calc_rng_value);
//    ImGui::InputInt("min", &calc_rng_min);
//    ImGui::InputInt("max", &calc_rng_max);
//    
//    const uint32_t Tmax = host_byteorder_32(rnd->tableMax);
//    const int R = (calc_rng_min + ((calc_rng_max - calc_rng_min + 1) * calc_rng_value) / (Tmax + 1));
//    ImGui::Text("Result: %d", R);
//    
//    ImGui::SetWindowSize(ImVec2(200, 125));
//    
//    ImGui::End();
//}
//
//void display_rng_info()
//{
//    const char* fmt = "RNG(%d, 0x%X, 0x%X)";
//    if (rng_display_as_decimal) fmt = "RNG(%d, %d, %d)";
//}
//

static int PreviousFrameIndex = 0;
static int PreviousRealIndex = 0;
static bool DecimalDisplay = false;

static const ImVec4 PastColor = ImVec4(1.0f, 0.2f, 0.3f, 0.4f);
static const ImVec4 FutureColor = ImVec4(0.5f, 0.5f, 1.0, 1.0f);
static const char * const MarkerSymbol = "->";

static float TimeScale = 1.0f;

static void RazoffHelper() {
    if (game::g_stEngineStructure->currentLevelName == "Swamp_51" && p_stFatherSector) {
        
    }
}

static void DrawTable()
{    
    ImGuiWindowFlags flags =
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize;
    
    bool view = true;
    
    ImGuiStyle OldStyle = ImGui::GetStyle();
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().WindowPadding = ImVec2(5.0f, 15.0f);
    ImGui::GetStyle().WindowBorderSize = 0.0f;
    ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.05f);
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
    
    
    ImGui::Begin("RNG Table", &view, flags);
    {
        const int H = ImGui::GetIO().DisplaySize.y / ImGui::GetTextLineHeightWithSpacing();
        const int NumRows = H / 2;
        
        //ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.25f), "   RNG");
        
        ImGui::SetWindowSize(ImVec2(150, 0));
        ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::GetWindowWidth(), 140));
        
        uint32 TableSize = RND_g_stRandomStructure->tableSize;
        uint32 BaseIndex = *RND_g_stRandomStructure->tableIndices;
        
        if (PreviousFrameIndex != BaseIndex)
            PreviousRealIndex = PreviousFrameIndex;
        
        std::string fmt = DecimalDisplay ? "%d: %d" : "0x%04X: 0x%04X";
        for (int i = -NumRows/2+2; i < NumRows/2; i++)
        {
            uint32 value = RND_g_stRandomStructure->indexRelative(0, i);
            float a = 1.0f - float(i) / float(NumRows) + 0.1f;
            
            bool b = (BaseIndex + i == PreviousRealIndex) || (i == 0) || (i == 2);
            ImVec4 color = i < 0 ? PastColor : FutureColor;
            
            float c = powf(((float)NumRows / 2.0f - std::abs(i)) / ((float)NumRows / 2.0f), 0.5f);
            color.w = c; //sin(((float)NumRows/2.0f + i)*0.1f);
            color.x /= c;
            color.y *= pow(cos(c),0.5);
            
            if (b) color.w *= 0.5f;
            if (i == 0) color = ImVec4(1.0f, 0.55f, 0.1f, 1.0f);
            if (i == 2) color = ImVec4(0.0f, 1.0f, 0.5f, 0.75f);
            
            if (BaseIndex + i < TableSize)
            {
                ImGui::TextColored(color, ("%-2s " + fmt).c_str(), (b ? MarkerSymbol : ""), BaseIndex + i, value);
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::TextColored(color, fmt.c_str(), BaseIndex + i, value);
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
                    DecimalDisplay = !DecimalDisplay;
            }
        }
        
        //if (calc_rng) display_calculate_rng();
        
        PreviousFrameIndex = BaseIndex;
    }
    ImGui::End();
    
    ImGui::GetStyle() = OldStyle;
    
//    ImGui::GetStyle().WindowRounding = 0.0f;
//    ImGui::GetStyle().WindowPadding = ImVec2(5.0f, 15.0f);
//    ImGui::GetStyle().WindowBorderSize = 0.0f;
//    ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.05f);
//    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
}

static void Draw() {
    DrawTable();
    RazoffHelper();
}

Window RNGWindow = {
    &Draw,
};
