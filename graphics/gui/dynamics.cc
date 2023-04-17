#include "stSuperobject.h"
#include "stEngineObject.h"
#include "stDynamics.h"

#define TableColumn(label, ...)     \
    ImGui::TableNextColumn();       \
    ImGui::Text(label);             \
    ImGui::TableNextColumn();       \
    ImGui::Text(__VA_ARGS__);       \

#define f32(v) host_byteorder_f32(*(uint32_t*)&v)

static void display_dynamics(const tdstDynamics* dynamics)
{
    if (!dynamics) return;
    
    const tdstDynamicsBaseBlock base = dynamics->base;
    const tdstDynamicsAdvancedBlock advanced = dynamics->advanced;
    const tdstDynamicsComplexBlock complex = dynamics->complex;
    const tdstDynamicsReport* report = (const tdstDynamicsReport*)pointer(base.report);
    
    const char* type = "Base";
    if (host_byteorder_32(dynamics->base.endFlags) & dynamics_size_advanced) type = "Advanced";
    if (host_byteorder_32(dynamics->base.endFlags) & dynamics_size_complex) type = "Complex";
    
    bool is_advanced = host_byteorder_32(base.endFlags) & dynamics_size_advanced;
    bool is_complex = host_byteorder_32(base.endFlags) & dynamics_size_complex;
    
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
                const tdstVector3D imposedSpeed = vector3_host_byteorder(dynamics->base.imposedSpeed);
                const tdstVector3D proposedSpeed = vector3_host_byteorder(dynamics->base.proposedSpeed);
                const tdstVector3D previousSpeed = vector3_host_byteorder(dynamics->base.previousSpeed);
                const tdstVector3D scale = vector3_host_byteorder(dynamics->base.scale);
                const tdstVector3D animSpeed = vector3_host_byteorder(dynamics->base.animationSpeed);
                const tdstVector3D safeTranslation = vector3_host_byteorder(dynamics->base.safeTranslation);
                const tdstVector3D addedTranslation = vector3_host_byteorder(dynamics->base.addedTranslation);
                
                TableColumn("Object type", "%X", host_byteorder_32(base.objectType))
                TableColumn("ID card", "%X", host_byteorder_32(base.idcard))
                TableColumn("Flags", "%X", host_byteorder_32(base.flags))
                TableColumn("Endflags", "%X", host_byteorder_32(base.endFlags))
                TableColumn("Gravity", "%f", host_byteorder_f32(base.gravity))
                TableColumn("Slope limit", "%f", host_byteorder_f32(base.slopeLimit));
                TableColumn("Slope", "%f°", degrees(host_byteorder_f32(base.slopeCosine)));
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
                const tdstVector3D maxSpeed = vector3_host_byteorder(advanced.maxSpeed);
                const tdstVector3D streamSpeed = vector3_host_byteorder(advanced.streamSpeed);
                const tdstVector3D addSpeed = vector3_host_byteorder(advanced.addedSpeed);
                const tdstVector3D limit = vector3_host_byteorder(advanced.limit);
                const tdstVector3D collisionTranslation = vector3_host_byteorder(advanced.collisionTranslation);
                const tdstVector3D inertiaTranslation = vector3_host_byteorder(advanced.inertiaTranslation);
                const tdstVector3D groundNormal = vector3_host_byteorder(advanced.groundNormal);
                const tdstVector3D wallNormal = vector3_host_byteorder(advanced.wallNormal);
                
                TableColumn("Inertia", "[%.2f, %.2f, %.2f]", f32(advanced.xInertia), f32(advanced.yInertia), f32(advanced.zInertia));
                TableColumn("Stream priority", "%f", f32(advanced.streamPriority));
                TableColumn("Stream factor", "%f", f32(advanced.streamFactor));
                TableColumn("Slide factor", "[%.2f, %.2f, %.2f]", f32(advanced.xSlideFactor), f32(advanced.ySlideFactor), f32(advanced.zSlideFactor));
                TableColumn("Previous slide", "%f", f32(advanced.previousSlide));
                TableColumn("Max speed", "[%.2f, %.2f, %.2f]", maxSpeed.x, maxSpeed.y, maxSpeed.z);
                TableColumn("Stream speed", "[%.2f, %.2f, %.2f]", streamSpeed.x, streamSpeed.y, streamSpeed.z);
                TableColumn("Add speed", "[%.2f, %.2f, %.2f]", addSpeed.x, addSpeed.y, addSpeed.z);
                TableColumn("Limit", "[%.2f, %.2f, %.2f]", limit.x, limit.y, limit.z);
                TableColumn("Col. translation", "[%.2f, %.2f, %.2f]", collisionTranslation.x, collisionTranslation.y, collisionTranslation.z);
                TableColumn("Inert. translation", "[%.2f, %.2f, %.2f]", inertiaTranslation.x, inertiaTranslation.y, inertiaTranslation.z);
                TableColumn("Ground normal", "[%.2f, %.2f, %.2f]", groundNormal.x, groundNormal.y, groundNormal.z);
                TableColumn("Wall normal", "[%.2f, %.2f, %.2f]", wallNormal.x, wallNormal.y, wallNormal.z);
                TableColumn("Collide count", "%d", advanced.collideCount);
                
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
                const tdstVector3D contact = vector3_host_byteorder(complex.contact);
                const tdstVector3D fallTranslation = vector3_host_byteorder(complex.fallTranslation);
                
                TableColumn("Tilt strength", "%f", f32(complex.tiltStrength));
                TableColumn("Tilt inertia", "%f", f32(complex.tiltInertia));
                TableColumn("Tilt origin", "%f", f32(complex.tiltOrigin));
                TableColumn("Tilt angle", "%f", f32(complex.tiltAngle));
                TableColumn("Hanging limit", "%f", f32(complex.hangingLimit));
                TableColumn("Contact", "[%.2f, %.2f, %.2f]", contact.x, contact.y, contact.z);
                TableColumn("Fall translation", "[%.2f, %.2f, %.2f]", fallTranslation.x, fallTranslation.y, fallTranslation.z);
                TableColumn("Platform SO", "%X", host_byteorder_32(complex.platformSuperObject));
                
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
        
        const tdstDynamicsObstacle obstacle = report->obstacle;
        const tdstDynamicsObstacle ground = report->ground;
        const tdstDynamicsObstacle wall = report->wall;
        const tdstDynamicsObstacle character = report->character;
        const tdstDynamicsObstacle water = report->water;
        const tdstDynamicsObstacle ceiling = report->ceiling;
        
        const tdstVector3D groundNormal = vector3_host_byteorder(ground.normal);
        const tdstVector3D wallNormal = vector3_host_byteorder(wall.normal);
        const tdstVector3D characterNormal = vector3_host_byteorder(character.normal);
        const tdstVector3D waterNormal = vector3_host_byteorder(water.normal);
        const tdstVector3D ceilingNormal = vector3_host_byteorder(ceiling.normal);
        
        const tdstVector3D groundContact = vector3_host_byteorder(ground.contact);
        const tdstVector3D wallContact = vector3_host_byteorder(wall.contact);
        const tdstVector3D characterContact = vector3_host_byteorder(character.contact);
        const tdstVector3D waterContact = vector3_host_byteorder(water.contact);
        const tdstVector3D ceilingContact = vector3_host_byteorder(ceiling.contact);
        
        const tdstVector3D currentPosition = vector3_host_byteorder(report->currentAbsolutePosition.linear);
        const tdstVector3D previousPosition = vector3_host_byteorder(report->previousAbsolutePosition.linear);
        const tdstVector3D currentSpeed = vector3_host_byteorder(report->currentAbsoluteSpeed.linear);
        const tdstVector3D previousSpeed = vector3_host_byteorder(report->previousAbsoluteSpeed.linear);
        
        const tdstSuperObject* ground_so = (const tdstSuperObject*)pointer(ground.superObject);
        const tdstSuperObject* wall_so = (const tdstSuperObject*)pointer(wall.superObject);
        const tdstSuperObject* ceiling_so = (const tdstSuperObject*)pointer(ceiling.superObject);
        const tdstSuperObject* character_so = (const tdstSuperObject*)pointer(character.superObject);
        
        if (ImGui::BeginTable("Dynamics report", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg))
        {
            TableColumn("Current surface state", "%X", host_byteorder_32(report->currentSurfaceState));
            TableColumn("Previous surface state", "%X", host_byteorder_32(report->previousSurfaceState));
            
            /* Wall */
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 1.0f, 1.0f));
            TableColumn("wall.rate", "%.2f", f32(wall.rate));
            TableColumn("wall.normal", "[%.2f, %.2f, %.2f]", wallNormal.x, wallNormal.y, wallNormal.z);
            TableColumn("wall.contact", "[%.2f, %.2f, %.2f]", wallContact.x, wallContact.y, wallContact.z);
            TableColumn("wall.so", "%s", fnSuperobjectGetName(wall_so));
            ImGui::PopStyleColor();
            
            /* Ground */
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.8f, 0.4f, 1.0f));
            TableColumn("ground.rate", "%.2f", f32(ground.rate));
            TableColumn("ground.normal", "[%.2f, %.2f, %.2f]", groundNormal.x, groundNormal.y, groundNormal.z);
            TableColumn("ground.contact", "[%.2f, %.2f, %.2f]", groundContact.x, groundContact.y, groundContact.z);
            TableColumn("ground.so", "%s", fnSuperobjectGetName(ground_so));
            ImGui::PopStyleColor();
            
            /* Ceiling */
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.3f, 1.0f));
            TableColumn("ceiling.rate", "%.2f", f32(ceiling.rate));
            TableColumn("ceiling.normal", "[%.2f, %.2f, %.2f]", ceilingNormal.x, ceilingNormal.y, ceilingNormal.z);
            TableColumn("ceiling.contact", "[%.2f, %.2f, %.2f]", ceilingContact.x, ceilingContact.y, ceilingContact.z);
            TableColumn("ceiling.so", "%s", fnSuperobjectGetName(ceiling_so));
            ImGui::PopStyleColor();
            
            /* Character */
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.3f, 0.3f, 1.0f));
            TableColumn("character.rate", "%.2f", f32(character.rate));
            TableColumn("character.normal", "[%.2f, %.2f, %.2f]", characterNormal.x, characterNormal.y, characterNormal.z);
            TableColumn("character.contact", "[%.2f, %.2f, %.2f]", characterContact.x, characterContact.y, characterContact.z);
            TableColumn("character.so", "%s", fnSuperobjectGetName(character_so));
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
