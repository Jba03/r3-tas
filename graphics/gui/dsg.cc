//
//  dsg.cc
//  r3-tas
//
//  Created by Jba03 on 2022-12-08.
//

#include "dsg.h"
#include "mind.h"
#include "brain.h"

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
    [dsgvar_type_boolean] = dark_blue,
    [dsgvar_type_ubyte] = dark_yellow,
    [dsgvar_type_int] = bright_blue,
    [dsgvar_type_uint] = dark_blue,
    [dsgvar_type_short] = bright_red,
    
    
    [dsgvar_type_float] = bright_pink,
    
    [dsgvar_type_float_array] = bright_red,
    [dsgvar_type_actor_array] = bright_red,
    [dsgvar_type_text_ref_array] = bright_red,
    
    [dsgvar_type_vector] = bright_green,
    
    [dsgvar_type_actor] = bright_yellow,
    [dsgvar_type_superobject] = orange,
    [dsgvar_type_graph] = dark_green,
    [dsgvar_type_waypoint] = dark_purple,
    
    [dsgvar_type_text] = dark_purple,
    [dsgvar_type_soundevent] = dark_purple,
    
    [n_dsgvar_types] = orange,
};

static std::string fmt_boolean(void* offset) { return std::to_string((*(uint8_t*)(offset))); }
static std::string fmt_byte(void* offset) { return std::to_string((*(int8_t*)(offset))); }
static std::string fmt_ubyte(void* offset) { return std::to_string((*(uint8_t*)(offset))); }
static std::string fmt_short(void* offset) { return std::to_string(host_byteorder_16(*(int16_t*)(offset))); }
static std::string fmt_ushort(void* offset) { return std::to_string(host_byteorder_16(*(uint16_t*)(offset))); }
static std::string fmt_int(void* offset) { return std::to_string(host_byteorder_32(*(int32_t*)(offset))); }
static std::string fmt_uint(void* offset) { return std::to_string(host_byteorder_32(*(uint32_t*)(offset))); }
static std::string fmt_float(void* offset) { std::stringstream s; s << std::setprecision(3) << host_byteorder_f32(*(uint32_t*)offset); return s.str(); }

static std::string fmt_vector(void* offset)
{
    struct vector3 v = *(struct vector3*)(offset);
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

static std::string fmt_superobject(address offset)
{
    
    return "NULL";
}

static std::string fmt_actor(void* offset)
{
    const struct superobject* so = (const struct superobject*)pointer(*(address*)offset);
    if (!so) return "NULL";
    
    const struct actor* actor = (const struct actor*)superobject_data(so);
    if (!actor) return "NULL";
    
    char str[256];
    memset(str, 0, 256);
    
    const char* name = actor_name(actor_instance_name, actor);
    if (!name) name = actor_name(actor_model_name, actor);
    
    return std::string(name ? name : "NULL");
}

static std::string dsgvar_fmt(const struct dsgvar_info* info, const uint8_t* buffer)
{
    void* data = (void*)(buffer + host_byteorder_32(info->mem_offset));
    
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
            
        case dsgvar_type_actor: return fmt_actor(data);
    }
    
    return "";
}

//static bool display_dsg_info = false;
//static struct DSGVariableInfo dsg_display_var;
//static int dsg_display_var_id = 0;
//static struct Actor* dsg_display_actor;
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

static void* actor_dsgvar(struct actor* actor, unsigned var)
{
    if (!actor) return NULL;
    
    const struct brain* brain = (const struct brain*)actor_brain(actor);
    if (!brain) return NULL;
    
    const struct mind* mind = (const struct mind*)pointer(brain->mind);
    if (!mind) return NULL;
    
    const struct dsgmem* dsgmem = (const struct dsgmem*)pointer(mind->dsgmemory);
    if (!dsgmem) return NULL;
    
    const struct dsgvar* dsgvars = (const struct dsgvar*)doublepointer(dsgmem->dsgvars);
    if (!dsgvars) return NULL;
        
    const uint8_t* buffer = (const uint8_t*)pointer(dsgmem->buffer_current);
    const struct dsgvar_info* info = (const struct dsgvar_info*)pointer(dsgvars->info) + var;
    if (!info) return NULL;
    
    void* data = (void*)(buffer + host_byteorder_32(info->mem_offset));
    
    return data;
}

static void display_actor_dsg(struct actor* actor, bool initial = false)
{
    const struct brain* brain = (const struct brain*)actor_brain(actor);
    if (!brain) return;
    
    const struct mind* mind = (const struct mind*)pointer(brain->mind);
    if (!mind) return;
    
    const struct dsgmem* dsgmem = (const struct dsgmem*)pointer(mind->dsgmemory);
    if (!dsgmem) return;
    
    const struct dsgvar* dsgvars = (const struct dsgvar*)doublepointer(dsgmem->dsgvars);
    if (!dsgvars) return;
        
    const uint8_t* buffer = (const uint8_t*)pointer(dsgmem->buffer_current);
    for (unsigned int var = 0; var < dsgvars->info_length; var++)
    {
        
        
        const struct dsgvar_info* info = (const struct dsgvar_info*)pointer(dsgvars->info) + var;
        
        const uint32_t type = host_byteorder_32(info->type);
        //const uint8_t* data = buffer + host_byteorder_32(info->mem_offset);
        
        std::string fmt = dsgvar_fmt(info, buffer);
        
        ImVec4 color = color_table[type];
        
        ImGui::TextColored(color, "%s_%d: %s", dsgvar_typenames[type], var, fmt.c_str());
    }
    
    
    
//    struct DSGMemory* mem = actor->brain->mind->dsg;
//
//    for (int i = 0; i < mem->n_variables; i++)
//    {
//        struct DSGVariableInfo var = mem->current[i];
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
