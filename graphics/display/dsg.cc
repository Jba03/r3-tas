//
//  dsg.cc
//  r3-tas
//
//  Created by Jba03 on 2022-12-08.
//

static const ImColor bright_red = ImColor(0xff3d11ee);
static const ImColor bright_pink = ImColor(0xffa501d3);
static const ImColor bright_yellow = ImColor(0xff49bef8);
static const ImColor bright_green = ImColor(0xff63f147);
static const ImColor bright_blue = ImColor(0xfff8c212);

static const ImColor dark_red = ImColor(0xff1c0b98);
static const ImColor orange = ImColor(0xff107dea);
static const ImColor dark_yellow = ImColor(0xff1ac6ff);
static const ImColor dark_green = ImColor(0xff3e6803);
static const ImColor dark_blue = ImColor(0xffb54103);
static const ImColor dark_purple = ImColor(0xff650183);

static ImColor color_table[] =
{
    [DSGVAR_TYPE_BOOLEAN] = dark_blue,
    [DSGVAR_TYPE_UBYTE] = dark_yellow,
    [DSGVAR_TYPE_INT] = bright_blue,
    [DSGVAR_TYPE_UINT] = dark_blue,
    [DSGVAR_TYPE_SHORT] = bright_red,
    
    
    [DSGVAR_TYPE_FLOAT] = bright_pink,
    
    [DSGVAR_TYPE_FLOAT_ARRAY] = bright_red,
    [DSGVAR_TYPE_ACTOR_ARRAY] = bright_red,
    [DSGVAR_TYPE_TEXT_REF_ARRAY] = bright_red,
    
    [DSGVAR_TYPE_VECTOR] = bright_green,
    
    [DSGVAR_TYPE_ACTOR] = bright_yellow,
    [DSGVAR_TYPE_SUPEROBJECT] = orange,
    [DSGVAR_TYPE_GRAPH] = dark_green,
    [DSGVAR_TYPE_WAYPOINT] = dark_purple,
    
    [DSGVAR_TYPE_TEXT] = dark_purple,
    [DSGVAR_TYPE_SOUNDEVENT] = dark_purple,
    
    [N_DSGVAR_TYPES] = orange,
};

static std::string fmt_int(address offset) { return std::to_string(int32_t(memory.read_32(offset))); }
static std::string fmt_uint(address offset) { return std::to_string(uint32_t(memory.read_32(offset))); }
static std::string fmt_byte(address offset) { return std::to_string(int8_t(memory.read_8(offset))); }
static std::string fmt_ubyte(address offset) { return std::to_string(uint8_t(memory.read_8(offset))); }
static std::string fmt_boolean(address offset) { return uint8_t(memory.read_8(offset)) != 0 ? "true" : "false"; }
static std::string fmt_float(address offset) { return std::to_string(float(memory.read_float(offset))); }

static std::string fmt_vector(address offset)
{
    struct Vector3 v = vector3_read(offset);
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.z) + ", " + std::to_string(v.y) + ")";
}

static std::string fmt_superobject(address offset)
{
    
    return "NULL";
}

static std::string fmt_actor(address offset)
{
    if (offset != 0x00)
    {
        uint32_t actor_offset = memory.read_32(offset) & 0xFFFFFFF;
        if (actor_offset != 0x00)
        {
            for (int i = 0; i < array_element_count(actor_list); i++)
            {
                struct Actor* actor = (struct Actor*)array_get(actor_list, i);
                if (actor->superobject)
                {
                    if (actor->superobject->offset == actor_offset)
                    {
                        return std::string(actor->instance_name);
                        break;
                    }
                }
            }
        }
    }
    
    return "NULL";
}

static std::string dsg_fmt(struct DSGVariableInfo var)
{
    switch (var.type_id)
    {
        case DSGVAR_TYPE_INT: return fmt_int(var.data_offset); break;
        case DSGVAR_TYPE_UINT: return fmt_uint(var.data_offset); break;
        case DSGVAR_TYPE_BYTE: return fmt_byte(var.data_offset); break;
        case DSGVAR_TYPE_UBYTE: return fmt_ubyte(var.data_offset); break;
        case DSGVAR_TYPE_BOOLEAN: return fmt_boolean(var.data_offset); break;
        case DSGVAR_TYPE_FLOAT: return fmt_float(var.data_offset); break;
        case DSGVAR_TYPE_ACTOR: return fmt_actor(var.data_offset); break;
    }
    
    return "";
}

static bool display_dsg_info = false;
static struct DSGVariableInfo dsg_display_var;
static int dsg_display_var_id = 0;
static struct Actor* dsg_display_actor;

static void display_dsg_editor()
{
    std::string name = std::string(dsg_display_actor->instance_name) + ": " +
                       std::string(dsg_display_var.type_name) + "_" + std::to_string(dsg_display_var_id);
    
    ImGui::Begin(name.c_str(), &display_dsg_info, ImGuiWindowFlags_Modal);
    
    ImGui::End();
}

static void display_dsg(struct Actor* actor)
{
    struct DSGMemory* mem = actor->brain->mind->dsg;
    
    for (int i = 0; i < mem->n_variables; i++)
    {
        struct DSGVariableInfo var = mem->current[i];
        var.type_id += 1;
        
        std::string fmt = "";
        ImColor color = color_table[var.type_id];
        ImVec4 color2 = color;
        color2.w = 0.8;
        
        if ((i % 2) == 0) color2.w = 0.6;
        
        switch (var.type_id)
        {
            case DSGVAR_TYPE_INT: fmt = fmt_int(var.data_offset); break;
            case DSGVAR_TYPE_UINT: fmt = fmt_uint(var.data_offset); break;
            case DSGVAR_TYPE_BYTE: fmt = fmt_byte(var.data_offset); break;
            case DSGVAR_TYPE_UBYTE: fmt = fmt_ubyte(var.data_offset); break;
            case DSGVAR_TYPE_BOOLEAN: fmt = fmt_boolean(var.data_offset); break;
            case DSGVAR_TYPE_FLOAT: fmt = fmt_float(var.data_offset); break;
            case DSGVAR_TYPE_VECTOR: fmt = fmt_vector(var.data_offset); break;
            case DSGVAR_TYPE_ACTOR: fmt = fmt_actor(var.data_offset); break;
        }
        
        ImGui::TextColored(color2, "%s_%d: %s", var.type_name, i, fmt.c_str());
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextColored(color2, "%s_%d @ %X: %s", var.type_name, i, var.data_offset, fmt.c_str());
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Click to edit");
            ImGui::EndTooltip();
        }
        
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            dsg_display_var = var;
            dsg_display_var_id = i;
            display_dsg_info = true;
            dsg_display_actor = actor;
            ImGui::SetNextWindowPos(ImGui::GetMousePos());
        }
    }
    
    if (display_dsg_info) display_dsg_editor();
}
