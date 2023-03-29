static void display_object_types(bool *display)
{
    if (*display && objectType)
    {
        tdstDoublyLinkedList family_list = objectType->family;
        tdstDoublyLinkedList model_list =  objectType->model;
        tdstDoublyLinkedList instance_list = objectType->instance;
        
        if (!pointer(family_list.first) || !pointer(model_list.first) || !pointer(instance_list.first)) return;
        if ((int32)host_byteorder_32(family_list.numEntries) < 0) return;
        if ((int32)host_byteorder_32(model_list.numEntries) < 0) return;
        if ((int32)host_byteorder_32(instance_list.numEntries) < 0) return;
        
        ImGui::Begin("Object types", display);
        
        ImGui::BeginChild("Family types", ImVec2(ImGui::GetContentRegionAvail().x / 3, ImGui::GetContentRegionAvail().y), true);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Family types");
        for (int i = 0; i < (int32)host_byteorder_32(family_list.numEntries); i++)
        {
            const tdstObjectTypeElement* type = ((const tdstObjectTypeElement*)pointer(family_list.first) + i);
            const char* name = (const char*)pointer(type->name);
            
            ImGui::TextColored(ImColor(color_table_index(2 * i + 1)), "%s", name);
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Model types", ImVec2(ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y), true);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Model types");
        for (int i = 0; i < (int32)host_byteorder_32(model_list.numEntries); i++)
        {
            const tdstObjectTypeElement* type = ((const tdstObjectTypeElement*)pointer(model_list.first) + i);
            const char* name = (const char*)pointer(type->name);

            ImVec4 color = (i % 2) == 0 ? ImVec4(1.0f, 1.0f, 1.0f, 0.5f) : ImVec4(1.0f, 1.0f, 1.0f, 0.35f);
            ImGui::TextColored(color, "%s", name);
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Instance types", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Instance types");
        for (int i = 0; i < (int32)host_byteorder_32(instance_list.numEntries); i++)
        {
            const tdstObjectTypeElement* type = ((const tdstObjectTypeElement*)pointer(instance_list.first) + i);
            const char* name = (const char*)pointer(type->name);

            ImVec4 color = (i % 2) == 0 ? ImVec4(1.0f, 1.0f, 1.0f, 0.5f) : ImVec4(1.0f, 1.0f, 1.0f, 0.35f);
            ImGui::TextColored(color, "%s", name);
        }
        ImGui::EndChild();
        
        ImGui::End();
    }
}
