#include "actor.h"
#include "dynamics.h"

#define TableColumn(label, ...)     \
    ImGui::TableNextColumn();       \
    ImGui::Text(label);             \
    ImGui::TableNextColumn();       \
    ImGui::Text(__VA_ARGS__);       \

#define f32(v) host_byteorder_f32(*(uint32_t*)&v)

static void display_dynamics(const struct dynamics* dynamics)
{
    if (!dynamics) return;
    
    const struct dynamics_base base = dynamics->base;
    const struct dynamics_advanced advanced = dynamics->advanced;
    const struct dynamics_complex complex = dynamics->complex;
    const struct dynamics_report* report = (const struct dynamics_report*)pointer(base.report);
    
    const char* type = "Base";
    if (host_byteorder_32(dynamics->base.endflags) & dynamics_size_advanced) type = "Advanced";
    if (host_byteorder_32(dynamics->base.endflags) & dynamics_size_complex) type = "Complex";
    
    bool is_advanced = host_byteorder_32(base.endflags) & dynamics_size_advanced;
    bool is_complex = host_byteorder_32(base.endflags) & dynamics_size_complex;
    
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
    
    ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 0.75f);
    
    ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(1, 1, 1, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4(1, 1, 1, 0.05f));
    
    ImGui::BeginChild("Container", ImVec2(ImGui::GetContentRegionAvail().x, 250.0f));
    {
        #pragma mark Base
        ImGui::BeginChild("Dynamics: base block", ImVec2(ImMax(250.0f, ImGui::GetContentRegionAvail().x / 3), 0));
        {
            ImGui::TextColored(ImVec4(0.7f, 0.4f, 0.0f, 1.0f), "Base @ %X", offset(&dynamics->base));
            if (ImGui::BeginTable("Dynamics base block", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg))
            {
                const vector3 imposedSpeed = vector3_host_byteorder(dynamics->base.speed_impose);
                const vector3 proposedSpeed = vector3_host_byteorder(dynamics->base.speed_propose);
                const vector3 previousSpeed = vector3_host_byteorder(dynamics->base.speed_previous);
                const vector3 scale = vector3_host_byteorder(dynamics->base.scale);
                const vector3 animSpeed = vector3_host_byteorder(dynamics->base.anim_speed);
                const vector3 safeTranslation = vector3_host_byteorder(dynamics->base.translation_safe);
                const vector3 addedTranslation = vector3_host_byteorder(dynamics->base.translation_add);
                
                TableColumn("Object type", "%X", host_byteorder_32(base.object_type))
                TableColumn("ID card", "%X", host_byteorder_32(base.idcard))
                TableColumn("Flags", "%X", host_byteorder_32(base.flags))
                TableColumn("Endflags", "%X", host_byteorder_32(base.endflags))
                TableColumn("Gravity", "%f", host_byteorder_f32(base.gravity))
                TableColumn("Slope limit", "%f", host_byteorder_f32(base.slopelimit));
                TableColumn("Slope", "%f°", degrees(host_byteorder_f32(base.slope_cosine)));
                TableColumn("Slide", "%f", host_byteorder_f32(base.rebound));
                TableColumn("Rebound", "%f", host_byteorder_f32(base.rebound));
                
                TableColumn("Imposed speed", "[%.2f, %.2f, %.2f]", imposedSpeed.x, imposedSpeed.y, imposedSpeed.z);
                TableColumn("Proposed speed", "[%.2f, %.2f, %.2f]", proposedSpeed.x, proposedSpeed.y, proposedSpeed.z);
                TableColumn("Previous speed", "[%.2f, %.2f, %.2f]", previousSpeed.x, previousSpeed.y, previousSpeed.z);
                TableColumn("Scale", "[%.2f, %.2f, %.2f]", scale.x, scale.y, scale.z);
                TableColumn("Anim speed", "[%.2f, %.2f, %.2f]", animSpeed.x, animSpeed.y, animSpeed.z);
                TableColumn("Safe translation", "[%.2f, %.2f, %.2f]", safeTranslation.x, safeTranslation.y, safeTranslation.z);
                TableColumn("Added translation", "[%.2f, %.2f, %.2f]", addedTranslation.x, addedTranslation.y, addedTranslation.z);
                
                ImGui::EndTable();
            }
        }
        ImGui::EndChild();
        
#pragma mark Advanced
        if (is_advanced)
        {
            ImGui::SameLine();
            ImGui::BeginChild("Dynamics: advanced block", ImVec2(ImMax(275.0f, ImGui::GetContentRegionAvail().x / 2), 0));
            ImGui::TextColored(ImVec4(0.7f, 0.4f, 0.0f, 1.0f), "Advanced @ %X", offset(&dynamics->advanced));
            if (ImGui::BeginTable("Dynamics advanced block", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg))
            {
                const vector3 maxSpeed = vector3_host_byteorder(advanced.speed_max);
                const vector3 streamSpeed = vector3_host_byteorder(advanced.speed_stream);
                const vector3 addSpeed = vector3_host_byteorder(advanced.speed_add);
                const vector3 limit = vector3_host_byteorder(advanced.limit);
                const vector3 collisionTranslation = vector3_host_byteorder(advanced.collision_translation);
                const vector3 inertiaTranslation = vector3_host_byteorder(advanced.inertia_translation);
                const vector3 groundNormal = vector3_host_byteorder(advanced.ground_normal);
                const vector3 wallNormal = vector3_host_byteorder(advanced.wall_normal);
                
                TableColumn("Inertia", "[%.2f, %.2f, %.2f]", f32(advanced.inertia_x), f32(advanced.inertia_y), f32(advanced.inertia_z));
                TableColumn("Stream priority", "%f", f32(advanced.streamprio));
                TableColumn("Stream factor", "%f", f32(advanced.streamfactor));
                TableColumn("Slide factor", "[%.2f, %.2f, %.2f]", f32(advanced.slide_factor_x), f32(advanced.slide_factor_y), f32(advanced.slide_factor_z));
                TableColumn("Previous slide", "%f", f32(advanced.slide_previous));
                TableColumn("Max speed", "[%.2f, %.2f, %.2f]", maxSpeed.x, maxSpeed.y, maxSpeed.z);
                TableColumn("Stream speed", "[%.2f, %.2f, %.2f]", streamSpeed.x, streamSpeed.y, streamSpeed.z);
                TableColumn("Add speed", "[%.2f, %.2f, %.2f]", addSpeed.x, addSpeed.y, addSpeed.z);
                TableColumn("Limit", "[%.2f, %.2f, %.2f]", limit.x, limit.y, limit.z);
                TableColumn("Col. translation", "[%.2f, %.2f, %.2f]", collisionTranslation.x, collisionTranslation.y, collisionTranslation.z);
                TableColumn("Inert. translation", "[%.2f, %.2f, %.2f]", inertiaTranslation.x, inertiaTranslation.y, inertiaTranslation.z);
                TableColumn("Ground normal", "[%.2f, %.2f, %.2f]", groundNormal.x, groundNormal.y, groundNormal.z);
                TableColumn("Wall normal", "[%.2f, %.2f, %.2f]", wallNormal.x, wallNormal.y, wallNormal.z);
                TableColumn("Collide count", "%d", advanced.collide_count);
                
                ImGui::EndTable();
            }
            ImGui::EndChild();
        }
        
        #pragma mark Complex
        if (is_complex)
        {
            ImGui::SameLine();
            ImGui::BeginChild("Dynamics: complex block", ImVec2(ImMax(250.0f, ImGui::GetContentRegionAvail().x / 2), 0));
            ImGui::TextColored(ImVec4(0.7f, 0.4f, 0.0f, 1.0f), "Complex @ %X", offset(&dynamics->complex));
            if (ImGui::BeginTable("Dynamics complex block", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg))
            {
                const vector3 contact = vector3_host_byteorder(complex.contact);
                const vector3 fallTranslation = vector3_host_byteorder(complex.fall_translation);
                
                TableColumn("Tilt strength", "%f", f32(complex.tilt_strength));
                TableColumn("Tilt inertia", "%f", f32(complex.tilt_inertia));
                TableColumn("Tilt origin", "%f", f32(complex.tilt_origin));
                TableColumn("Tilt angle", "%f", f32(complex.tilt_angle));
                TableColumn("Hanging limit", "%f", f32(complex.hanging_limit));
                TableColumn("Contact", "[%.2f, %.2f, %.2f]", contact.x, contact.y, contact.z);
                TableColumn("Fall translation", "[%.2f, %.2f, %.2f]", fallTranslation.x, fallTranslation.y, fallTranslation.z);
                TableColumn("Platform SO", "%X", host_byteorder_32(complex.platform_so));
                
                ImGui::EndTable();
            }
            ImGui::EndChild();
        }
        
        ImGui::EndChild();
    }
    
    #pragma mark Report
    if (report)
    {
        ImGui::BeginChild("Dynamics report");
        ImGui::TextColored(ImVec4(0.7f, 0.4f, 0.0f, 1.0f), "Report @ %X", offset(&dynamics->base.report));
        
        const dynamics_obstacle obstacle = report->obstacle;
        const dynamics_obstacle ground = report->ground;
        const dynamics_obstacle wall = report->wall;
        const dynamics_obstacle character = report->character;
        const dynamics_obstacle water = report->water;
        const dynamics_obstacle ceiling = report->ceiling;
        
        const vector3 groundNormal = vector3_host_byteorder(ground.normal);
        const vector3 wallNormal = vector3_host_byteorder(wall.normal);
        const vector3 characterNormal = vector3_host_byteorder(character.normal);
        const vector3 waterNormal = vector3_host_byteorder(water.normal);
        const vector3 ceilingNormal = vector3_host_byteorder(ceiling.normal);
        
        const vector3 groundContact = vector3_host_byteorder(ground.contact);
        const vector3 wallContact = vector3_host_byteorder(wall.contact);
        const vector3 characterContact = vector3_host_byteorder(character.contact);
        const vector3 waterContact = vector3_host_byteorder(water.contact);
        const vector3 ceilingContact = vector3_host_byteorder(ceiling.contact);
        
        const vector3 currentPosition = vector3_host_byteorder(report->position_absolute_current.linear);
        const vector3 previousPosition = vector3_host_byteorder(report->position_absolute_previous.linear);
        const vector3 currentSpeed = vector3_host_byteorder(report->speed_absolute_current.linear);
        const vector3 previousSpeed = vector3_host_byteorder(report->speed_absolute_previous.linear);
        
        const superobject* ground_so = (const superobject*)pointer(ground.superobject);
        const superobject* wall_so = (const superobject*)pointer(wall.superobject);
        const superobject* ceiling_so = (const superobject*)pointer(ceiling.superobject);
        const superobject* character_so = (const superobject*)pointer(character.superobject);
        
        if (ImGui::BeginTable("Dynamics report", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg))
        {
            TableColumn("Current surface state", "%X", host_byteorder_32(report->surface_state_current));
            TableColumn("Previous surface state", "%X", host_byteorder_32(report->surface_state_previous));
            
            /* Wall */
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 1.0f, 1.0f));
            TableColumn("wall.rate", "%.2f", f32(wall.rate));
            TableColumn("wall.normal", "[%.2f, %.2f, %.2f]", wallNormal.x, wallNormal.y, wallNormal.z);
            TableColumn("wall.contact", "[%.2f, %.2f, %.2f]", wallContact.x, wallContact.y, wallContact.z);
            TableColumn("wall.so", "%s", superobject_name(wall_so));
            ImGui::PopStyleColor();
            
            /* Ground */
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.8f, 0.4f, 1.0f));
            TableColumn("ground.rate", "%.2f", f32(ground.rate));
            TableColumn("ground.normal", "[%.2f, %.2f, %.2f]", groundNormal.x, groundNormal.y, groundNormal.z);
            TableColumn("ground.contact", "[%.2f, %.2f, %.2f]", groundContact.x, groundContact.y, groundContact.z);
            TableColumn("ground.so", "%s", superobject_name(ground_so));
            ImGui::PopStyleColor();
            
            /* Ceiling */
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.3f, 1.0f));
            TableColumn("ceiling.rate", "%.2f", f32(ceiling.rate));
            TableColumn("ceiling.normal", "[%.2f, %.2f, %.2f]", ceilingNormal.x, ceilingNormal.y, ceilingNormal.z);
            TableColumn("ceiling.contact", "[%.2f, %.2f, %.2f]", ceilingContact.x, ceilingContact.y, ceilingContact.z);
            TableColumn("ceiling.so", "%s", superobject_name(ceiling_so));
            ImGui::PopStyleColor();
            
            /* Character */
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.3f, 0.3f, 1.0f));
            TableColumn("character.rate", "%.2f", f32(character.rate));
            TableColumn("character.normal", "[%.2f, %.2f, %.2f]", characterNormal.x, characterNormal.y, characterNormal.z);
            TableColumn("character.contact", "[%.2f, %.2f, %.2f]", characterContact.x, characterContact.y, characterContact.z);
            TableColumn("character.so", "%s", superobject_name(character_so));
            ImGui::PopStyleColor();
            
            ImGui::EndTable();
        }
        
        ImGui::EndChild();
    }
    
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

#undef f32
