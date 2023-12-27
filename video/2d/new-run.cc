//
//  new-run.cpp
//  r3-tas
//
//  Created by Jba03 on 2023-07-18.
//

#include "gui.hh"

static int StartMode = 0;

static void Draw() {
    if (RunCreateWindow.Open) {
        ImGui::Begin("New run");
        
        ImGui::Text("Start...");
        {
            ImGui::RadioButton("From level load", &StartMode, 0);
            ImGui::SameLine();
            //ImGui::ListBox
        }
        ImGui::RadioButton("From player control gained", &StartMode, 1);
        ImGui::RadioButton("From start of next loadstate", &StartMode, 2);
        
        ImGui::End();
    }
}

Window RunCreateWindow = { &Draw, NULL, false };
