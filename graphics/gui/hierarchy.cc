//
//  hierarchy.cc
//  r3-tas
//
//  Created by Jba03 on 2022-12-20.
//


static tdstSuperObject* info_target_so = NULL;

tdstSuperObject* viewed_sector;

static void display_superobject_info()
{
    if (info_target_so)
    {
        uint32_t type = host_byteorder_32(info_target_so->type);
        if (type == superobject_type_actor)
        {
            const tdstEngineObject* actor = (const tdstEngineObject*)pointer(info_target_so->data);
            if (!actor) return;
            
            const tdstStandardGameInfo* stdgame = (const tdstStandardGameInfo*)pointer(actor->stdGame);
            if (!stdgame) return;
            
            /* Get actor names */
            const char* family_name = "a";//fnActorGetName(actor_family_name, actor);
//            const char* model_name = fnActorGetName(actor_model_name, actor);
//            const char* instance_name = fnActorGetName(actor_instance_name, actor);
            const char* display_name = NULL;//instance_name ? instance_name : model_name;
            if (!display_name) display_name = "Invalid object name";
                
            /* Get family color */
            const uint32_t col = actor_color(actor);
            const ImVec4 color = ImColor(col);
                                                       
            ImGui::Begin(display_name);



            ImGui::TextColored(color, "Family: %s", family_name);
            
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Superobject: %X -> %p", offset(info_target_so), info_target_so);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Address: %X -> %p", offset(actor), actor);
            
            
            ImGui::End();
        }
    }
}

static void display_hierarchy(struct stSuperObject *so, const char* first_obj_name = NULL)
{
    if (!so) return;
    
    ImGui::PushID(so);
    
    const uint32_t type = host_byteorder_32(so->type);
    const char* _typename = fnSuperobjectGetTypename(so);
    const char* name = first_obj_name ? first_obj_name : _typename;
    ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
    
    const tdstSuperObject* first_child = superobject_first_child(so);
    const tdstSuperObject* last_child = superobject_last_child(so);
    const uint32_t n_children = superobject_n_children(so);
    
    std::string extra = "";
    if (first_obj_name)
        extra += "(" + std::to_string(n_children) + " children)";
    
    if (type == superobject_type_actor)
    {
        if (host_byteorder_32(so->data) != 0x00)
        {
            tdstEngineObject* actor = (tdstEngineObject*)pointer(so->data);
            if (!actor) goto end;
            
            tdstStandardGameInfo* stdgame = (tdstStandardGameInfo* )pointer(actor->stdGame);
            if (!stdgame) goto end;
            
            uint32_t col = actor_color(actor);
            color = ImColor(col);
            
            name = fnSuperobjectGetName(so);
            if (!name) name = "Invalid object name";
        }
        else
        {
            name = "Invalid actor";
        }
    }
    
    if (type == superobject_type_ipo || type == superobject_type_ipo_mirror)
    {
        tdstInstantiatedPhysicalObject* ipo = (tdstInstantiatedPhysicalObject*)superobject_data(so);
        if (ipo)
        {
            name = ipo->name;
        }
    }
    
    if (type == superobject_type_sector)
    {
        tdstSector* sector = (tdstSector*)superobject_data(so);
        if (sector)
        {
            name = strchr(sector->name, ':') + 1;
            if (so == viewed_sector) color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            uint32_t prio = sector_priority(sector);
            uint8_t is_virtual = sector_is_virtual(sector);
            int8_t counter = sector_get_counter(sector);
            extra += "(prio:" + std::to_string(prio) + ", vir:" + std::to_string(is_virtual) + ", cnt:" + std::to_string(counter) + ")";
        }
    }
    
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    
    if (ImGui::TreeNodeEx(_typename, ImGuiTreeNodeFlags_None, "%s %s", name, extra.c_str()))
    {
        superobject_for_each(so, child)
        {
            display_hierarchy(child);
        }
        
//        if (first_child != 0x00 && last_child != 0x00)
//        {
//            tdstSuperObject* obj = (tdstSuperObject*)pointer(so->first_child);
//            if (!obj)
//            {
//                ImGui::TreePop();
//                goto end;
//            }
//
//            for (unsigned int n = 0; n < n_children; n++)
//            {
//                display_hierarchy(obj);
//                obj = (tdstSuperObject*)pointer(obj->next);
//                if (!obj) break;
//            }
//        }

        if (ImGui::BeginPopupContextItem())
        {
            ImGui::EndPopup();
        }

        if (ImGui::BeginDragDropTarget())
        {
            ImGui::EndDragDropTarget();
        }

        if (ImGui::BeginDragDropSource())
        {
            ImGui::EndDragDropSource();
        }
        
        ImGui::TreePop();
    }
    
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Left click to open/close node");
        ImGui::Text("Right click to view information");
        ImGui::EndTooltip();
    }
    
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        if (so != dynamic_world && so != inactive_dynamic_world && so != father_sector)
            superobject_info_windows.push_back(SuperobjectInfoWindow::CreateWindow(so));
    }
    
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
        const uint32_t addr = offset(so);
        memory_viewer.GotoAddrAndHighlight(addr, addr);
        selected_superobject = so;
        if (superobject_type(so) == superobject_type_sector)
        {
            viewed_sector = so;
        }
    }
    
    display_superobject_info();
    
    ImGui::PopStyleColor();
    
end:
    ImGui::PopID();
}
