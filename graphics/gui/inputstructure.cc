#include "stInputStructure.h"

static void display_input_structure(bool *display)
{
    if (input_struct && *display)
    {
        ImGui::Begin("Input entry viewer", display);
        if (ImGui::BeginTable("Input structure", 5, ImGuiTableRowFlags_Headers | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, ImVec4(0.6, 0, 0, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(1, 1, 1, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4(1, 1, 1, 0.1f));
            
            /* Table header */
            ImGui::TableSetupColumn("Action name");
            ImGui::TableSetupColumn("Entry name");
            ImGui::TableSetupColumn("Active");
            ImGui::TableSetupColumn("State");
            ImGui::TableSetupColumn("Analog state");
            ImGui::TableHeadersRow();
            
            tdstInputEntryElement* entry = (tdstInputEntryElement*)pointer(input_struct->entries);
            int c = host_byteorder_32(input_struct->numEntries);
            while (c-- && entry)
            {
                const char* action_name = (const char*)pointer(entry->actionName);
                const char* entry_name = (const char*)pointer(entry->entryName);
                const int32 state = host_byteorder_32(entry->state);
                const float analog = host_byteorder_f32(*(float32*)&entry->analogValue);
                
                ImGui::TableNextColumn();
                ImGui::Text("%s", action_name);
                ImGui::TableNextColumn();
                
                /* Unlike the action name, the entry */
                /* name is not guaranteed to exist. */
                if (entry_name) ImGui::Text("%s", entry_name);
                
                ImGui::TableNextColumn();
                ImGui::Text("%s", entry->active ? "Yes" : "No");
                ImGui::TableNextColumn();
                ImGui::Text("%X", state);
                ImGui::TableNextColumn();
                ImGui::Text("%f", analog);
                ImGui::TableNextRow();
                
                entry++;
            }
            
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::EndTable();
        }
        ImGui::End();
    }
}
