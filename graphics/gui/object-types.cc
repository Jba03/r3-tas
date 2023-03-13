static void display_object_types(bool *display)
{
    if (*display && lvl.section_a)
    {
        doubly_linked_list family_list = lvl.section_a->object_type.family;
        doubly_linked_list model_list = lvl.section_a->object_type.model;
        doubly_linked_list instance_list = lvl.section_a->object_type.instance;
        
        if (!pointer(family_list.first) || !pointer(model_list.first) || !pointer(instance_list.first)) return;
        if ((int32)host_byteorder_32(family_list.n_entries) < 0) return;
        if ((int32)host_byteorder_32(model_list.n_entries) < 0) return;
        if ((int32)host_byteorder_32(instance_list.n_entries) < 0) return;
        
        ImGui::Begin("Object types", display);
        
        ImGui::BeginChild("Family types", ImVec2(ImGui::GetContentRegionAvail().x / 3, ImGui::GetContentRegionAvail().y), true);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Family types");
        for (int i = 0; i < (int32)host_byteorder_32(family_list.n_entries); i++)
        {
            const struct object_type* type = ((const struct object_type*)pointer(family_list.first) + i);
            const char* name = (const char*)pointer(type->name);
            
            ImGui::TextColored(ImColor(color_table_index(2 * i + 1)), "%s", name);
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Model types", ImVec2(ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y), true);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Model types");
        for (int i = 0; i < (int32)host_byteorder_32(model_list.n_entries); i++)
        {
            const struct object_type* type = ((const struct object_type*)pointer(model_list.first) + i);
            const char* name = (const char*)pointer(type->name);

            ImVec4 color = (i % 2) == 0 ? ImVec4(1.0f, 1.0f, 1.0f, 0.5f) : ImVec4(1.0f, 1.0f, 1.0f, 0.35f);
            ImGui::TextColored(color, "%s", name);
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Instance types", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Instance types");
        for (int i = 0; i < (int32)host_byteorder_32(instance_list.n_entries); i++)
        {
            const struct object_type* type = ((const struct object_type*)pointer(instance_list.first) + i);
            const char* name = (const char*)pointer(type->name);

            ImVec4 color = (i % 2) == 0 ? ImVec4(1.0f, 1.0f, 1.0f, 0.5f) : ImVec4(1.0f, 1.0f, 1.0f, 0.35f);
            ImGui::TextColored(color, "%s", name);
        }
        ImGui::EndChild();
        
        ImGui::End();
    }
}
