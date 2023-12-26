//
//  hierarchy.cpp
//  r3-tas
//
//  Created by Jba03 on 2023-07-16.
//


#include "gui.hh"
#include "stSuperObject.hh"
#include "game.hh"

using namespace game;

static bool Search = false;
static char SearchString[64];

static const std::string NameOverride(stSuperObject *root) {
    if (!root) return "Invalid name";
    if (root == p_stActualWorld) return "Actual world";
    else if (root == p_stDynamicWorld) return "Dynamic world (" + std::to_string(root->numChildren) + ")";
    else if (root == p_stInactiveDynamicWorld) return "Inactive dynamic world (" + std::to_string(root->numChildren) + ")";
    else if (root == p_stFatherSector) return "Father sector (" + std::to_string(root->numChildren) + ")";
    return root->getName(g_stObjectTypes);
}

static void RecursiveDraw(stSuperObject *root) {
    ImGui::PushID(root);
    ImGui::PushStyleColor(ImGuiCol_Text, game::objectColor(root));
    
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
    if (root->numChildren == 0) flags |= ImGuiTreeNodeFlags_Leaf;
    
    if (ImGui::TreeNodeEx(root->getTypeName().c_str(), flags, "%s %s", NameOverride(root).c_str(), "")) {
        root->forEachChild([&](stSuperObject *child) {
            if (child) RecursiveDraw(child);
        });
        ImGui::TreePop();
    }
    ImGui::PopStyleColor();
    ImGui::PopID();
}

static void DrawHierarchyWindow() {
    ImGuiStyle PreviousStyle = ImGui::GetStyle();
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().WindowBorderSize = 1.0f;
    ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.05f);
    ImGui::GetStyle().ScrollbarRounding = 0.0f;
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(-1, ImGui::GetIO().DisplaySize.y - 200));
    ImGui::SetNextWindowSize(ImVec2(-1, -1));
    ImGui::Begin("Actual world", &HierarchyWindow.Open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
    ImGui::SetWindowPos(ImVec2(0, 100));
    
    ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 0.05f);
    ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.1f);
    ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
    if (ImGui::SmallButton(" Search...")) Search = true;
    ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    
    RecursiveDraw(p_stDynamicWorld);
    RecursiveDraw(p_stInactiveDynamicWorld);
    RecursiveDraw(p_stFatherSector);
    
    ImGui::End();
    ImGui::GetStyle() = PreviousStyle;
}

static void DrawSearchWindow() {
    if (Search) {
        
        ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(500, 400));
        
        ImGui::Begin("Search object");
        if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
           ImGui::SetKeyboardFocusHere(0);
        
        ImGui::InputText("", SearchString, 64);
        
        ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - ImGui::GetWindowSize().x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f - ImGui::GetWindowSize().y / 2.0f));
        ImGui::End();
    }
}

static void Draw() {
    if (isValidGameState()) {
        DrawHierarchyWindow();
        DrawSearchWindow();
    }
}

Window HierarchyWindow = {
    Draw
};
