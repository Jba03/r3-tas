#include "stMatrix4D.h"

static void display_matrix4(tdstMatrix4D matrix, ImVec4 color)
{
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::Text("%5.2f  %5.2f  %5.2f  %5.2f", matrix.m00, matrix.m01, matrix.m02, matrix.m03);
    ImGui::Text("%5.2f  %5.2f  %5.2f  %5.2f", matrix.m10, matrix.m11, matrix.m12, matrix.m13);
    ImGui::Text("%5.2f  %5.2f  %5.2f  %5.2f", matrix.m20, matrix.m21, matrix.m22, matrix.m23);
    ImGui::Text("%5.2f  %5.2f  %5.2f  %5.2f", matrix.m30, matrix.m31, matrix.m32, matrix.m33);
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

