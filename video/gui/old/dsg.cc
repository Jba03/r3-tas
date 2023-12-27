//
//  dsg.cc
//  r3-tas
//
//  Created by Jba03 on 2022-12-08.
//

#include <sstream>
#include <iomanip>

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
    dark_blue, /* boolean */
    bright_blue, /* byte */
    dark_yellow, /* ubyte */
    bright_red, /* short */
    dark_red, /* ushort */
    bright_blue, /* int */
    dark_blue, /* uint */
    bright_pink, /* float */
    bright_green, /* vector */
    dark_green, /* list */
    dark_purple, /* behavior */
    dark_yellow, /* action */
    dark_blue, /* capabilities */
    dark_red, /* input */
    dark_purple, /* soundevent */
    bright_yellow, /* light */
    dark_yellow, /* game material */
    dark_yellow, /* visual material */
    bright_yellow, /* actor */
    bright_pink, /* waypoint */
    dark_green, /* graph */
    dark_purple, /* text */
    orange, /* superobject */
    orange, /* superobject links */
    bright_red, /* actor array */
    bright_red, /* vector array */
    bright_red, /* float array */
    bright_red, /* int array */
    bright_red, /* waypoint array */
    bright_red, /* text array */
    bright_red, /* textref array */
    bright_red, /* graph array */
    bright_red, /* ? */
    bright_red, /* sound event array */
    bright_red, /* ? */
    dark_purple, /* way */
    bright_red, /* action array */
    bright_red, /* superobject array */
    orange, /* object list */
};

static std::string fmt_boolean(void* offset) { return (*(uint8_t*)(offset)) ? "TRUE" : "FALSE"; }
static std::string fmt_byte(void* offset) { return std::to_string((*(int8_t*)(offset))); }
static std::string fmt_ubyte(void* offset) { return std::to_string((*(uint8_t*)(offset))); }
static std::string fmt_short(void* offset) { return std::to_string(host_byteorder_16(*(int16_t*)(offset))); }
static std::string fmt_ushort(void* offset) { return std::to_string(host_byteorder_16(*(uint16_t*)(offset))); }
static std::string fmt_int(void* offset) { return std::to_string(host_byteorder_32(*(int32_t*)(offset))); }
static std::string fmt_uint(void* offset) { return std::to_string(host_byteorder_32(*(uint32_t*)(offset))); }
static std::string fmt_float(void* offset) { std::stringstream s; s << std::setprecision(3) << host_byteorder_f32(*(uint32_t*)offset); return s.str(); }

static std::string fmt_vector(void* offset)
{
    stVector3D v = *(stVector3D*)(offset);
    uint32_t ix = host_byteorder_32(*(uint32_t*)&v.x);
    uint32_t iy = host_byteorder_32(*(uint32_t*)&v.y);
    uint32_t iz = host_byteorder_32(*(uint32_t*)&v.z);
    
    float* x = (float*)&ix;
    float* y = (float*)&iy;
    float* z = (float*)&iz;
    
    std::stringstream s;
    s << std::setprecision(3);
    s << "(" << *x << ", " << *y << ", " << *z << ")";
    
    return s.str();
}

static std::string fmt_actor(void* offset)
{
    const stSuperObject* so = (const stSuperObject*)pointer(*(address*)offset);
    if (!so) return "NULL";
    
    const stEngineObject* actor = (const stEngineObject*)superobject_data(so);
    if (!actor) return "NULL";
    
    const char* name = fnActorGetName(actor_instance_name, actor, objectType);
    if (!name) name = fnActorGetName(actor_model_name, actor, objectType);
    
    return std::string(name ? name : "NULL");
}

static std::string fmt_sector(const stSuperObject* offset)
{
    const stSector* sct = (const stSector*)superobject_data(offset);
    if (!sct) return "NULL";
    
    return std::string(sct->name);
}

static std::string fmt_ipo(const stSuperObject* offset)
{
    const stInstantiatedPhysicalObject* ipo = (const stInstantiatedPhysicalObject*)superobject_data(offset);
    if (!ipo) return "NULL";
    
    return std::string(ipo->name);
}

static std::string fmt_superobject(void* offset)
{
    const stSuperObject* so = (const stSuperObject*)pointer(*(address*)offset);
    if (!so) return "NULL";
    const char* name = fnSuperobjectGetName(so);
    return name ? name : "NULL";
}

static std::string fmt_superobject_array(void* offset)
{
    //printf("spo array: %X\n", offset(offset));
    
//    const stSuperObject* so = (const stSuperObject*)pointer(*(address*)offset);
//    if (!so) return "NULL";
    
    return "NULL";
}

static std::string fmt_actor_array(void* offset)
{
    //printf("actor array: %X\n", offset(offset));
//    const stSuperObject* so = (const stSuperObject*)pointer(*(address*)offset);
//    if (!so) return "NULL";
    
    return "NULL";
}

static std::string dsgvar_fmt(const stDsgVarInfo* info, const uint8_t* buffer)
{
    void* data = (void*)(buffer + host_byteorder_32(info->memOffset));
    
    std::setprecision(3);
    
    switch (host_byteorder_32(info->type))
    {
        case dsgvar_type_boolean: return fmt_boolean(data); break;
        case dsgvar_type_byte: return fmt_byte(data); break;
        case dsgvar_type_ubyte: return fmt_ubyte(data); break;
        case dsgvar_type_short: return fmt_short(data); break;
        case dsgvar_type_ushort: return fmt_ushort(data); break;
        case dsgvar_type_int: return fmt_int(data); break;
        case dsgvar_type_uint: return fmt_uint(data); break;
            
        case dsgvar_type_float: return fmt_float(data); break;
        case dsgvar_type_vector: return fmt_vector(data); break;
            
        case dsgvar_type_superobject: return fmt_superobject(data); break;
        case dsgvar_type_actor: return fmt_actor(data); break;
            
        case dsgvar_type_actor_array: return fmt_actor_array(data); break;
        case dsgvar_type_superobj_array: return fmt_superobject_array(data); break;
    }
    
    return "";
}

//static bool display_dsg_info = false;
//static stDsgVariableInfo dsg_display_var;
//static int dsg_display_var_id = 0;
//static stEngineObject* dsg_display_actor;
//
//static void display_dsg_editor()
//{
//    std::string name = std::string(dsg_display_actor->instance_name) + ": " +
//                       std::string(dsg_display_var.type_name) + "_" + std::to_string(dsg_display_var_id);
//
//    ImGui::Begin(name.c_str(), &display_dsg_info, ImGuiWindowFlags_Modal);
//
//    ImGui::End();
//}

static void* actor_dsgvar(stEngineObject* actor, unsigned var)
{
    if (!actor) return NULL;
    
    const stBrain* brain = (const stBrain*)actor_brain(actor);
    if (!brain) return NULL;
    
    const stMind* mind = (const stMind*)pointer(brain->mind);
    if (!mind) return NULL;
    
    const stDsgMem* dsgmem = (const stDsgMem*)pointer(mind->dsgMem);
    if (!dsgmem) return NULL;
    
    const stDsgVar* dsgvars = (const stDsgVar*)doublepointer(dsgmem->dsgVars);
    if (!dsgvars) return NULL;
        
    const uint8_t* buffer = (const uint8_t*)pointer(dsgmem->currentBuffer);
    const stDsgVarInfo* info = (const stDsgVarInfo*)pointer(dsgvars->info) + var;
    if (!info) return NULL;
    
    void* data = (void*)(buffer + host_byteorder_32(info->memOffset));
    
    return data;
}

static void display_actor_dsg(stEngineObject* actor, bool initial = false)
{
    const stBrain* brain = (const stBrain*)actor_brain(actor);
    if (!brain) return;
    
    const stMind* mind = (const stMind*)pointer(brain->mind);
    if (!mind) return;
    
    const stDsgMem* dsgmem = (const stDsgMem*)pointer(mind->dsgMem);
    if (!dsgmem) return;
    
    const stDsgVar* dsgvars = (const stDsgVar*)doublepointer(dsgmem->dsgVars);
    if (!dsgvars) return;
        
    const uint8_t* buffer = (const uint8_t*)pointer(dsgmem->currentBuffer);
    for (unsigned int var = 0; var < dsgvars->infoLength; var++)
    {        
        const stDsgVarInfo* info = (const stDsgVarInfo*)pointer(dsgvars->info) + var;
        
        const uint32_t type = host_byteorder_32(info->type);
        //const uint8_t* data = buffer + host_byteorder_32(info->mem_offset);
        
        std::string fmt = dsgvar_fmt(info, buffer);
        
        ImVec4 color = color_table[type];
        
        ImGui::TextColored(color, "%s_%d: %s", dsgvar_typenames[type], var, fmt.c_str());
        
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            memory_viewer.GotoAddr = offset((void*)(buffer + host_byteorder_32(info->memOffset)));
        }
        
        if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && configuration.cheats.enabled)
        {
            void* data = (void*)(buffer + host_byteorder_32(info->memOffset));
            *(uint8*)data = !*(uint8*)data;
        }
    }
    
    
    
//    stDsgMemory* mem = actor->brain->mind->dsg;
//
//    for (int i = 0; i < mem->n_variables; i++)
//    {
//        stDsgVariableInfo var = mem->current[i];
//        var.type_id += 1;
//
//        std::string fmt = "";
//        ImColor color = color_table[var.type_id];
//        ImVec4 color2 = color;
//        color2.w = 0.8;
//
//        if ((i % 2) == 0) color2.w = 0.6;
//
//        switch (var.type_id)
//        {
//            case DSGVAR_TYPE_INT: fmt = fmt_int(var.data_offset); break;
//            case DSGVAR_TYPE_UINT: fmt = fmt_uint(var.data_offset); break;
//            case DSGVAR_TYPE_BYTE: fmt = fmt_byte(var.data_offset); break;
//            case DSGVAR_TYPE_UBYTE: fmt = fmt_ubyte(var.data_offset); break;
//            case DSGVAR_TYPE_BOOLEAN: fmt = fmt_boolean(var.data_offset); break;
//            case DSGVAR_TYPE_FLOAT: fmt = fmt_float(var.data_offset); break;
//            case DSGVAR_TYPE_VECTOR: fmt = fmt_vector(var.data_offset); break;
//            case DSGVAR_TYPE_ACTOR: fmt = fmt_actor(var.data_offset); break;
//        }
//
//        ImGui::TextColored(color2, "%s_%d: %s", var.type_name, i, fmt.c_str());
//        if (ImGui::IsItemHovered())
//        {
//            ImGui::BeginTooltip();
//            ImGui::TextColored(color2, "%s_%d @ %X: %s", var.type_name, i, var.data_offset, fmt.c_str());
//            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Click to edit");
//            ImGui::EndTooltip();
//        }
//
//        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
//        {
//            dsg_display_var = var;
//            dsg_display_var_id = i;
//            display_dsg_info = true;
//            dsg_display_actor = actor;
//            ImGui::SetNextWindowPos(ImGui::GetMousePos());
//        }
//    }
//
//    if (display_dsg_info) display_dsg_editor();
}