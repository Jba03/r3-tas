#include "actor.h"
#include "dynamics.h"

static void display_dynamics(const struct dynamics* dynamics)
{
    if (!dynamics) return;
    
    const char* type = "Base";
    if (host_byteorder_32(dynamics->base.endflags) & dynamics_size_advanced) type = "Advanced";
    if (host_byteorder_32(dynamics->base.endflags) & dynamics_size_complex) type = "Complex";
    
    //DisplayBits((dynamics->base.idcard));
    
    
#define f32(v) host_byteorder_f32(*(uint32_t*)&v)
    
    #pragma mark Base
    ImGui::BeginChild("Dynamics: base block", ImVec2(ImMax(ImGui::GetContentRegionAvail().x / 3, 150.0f), ImGui::GetContentRegionAvail().y / 1.5));
    {
        ImGui::TextColored(ImVec4(0.7f, 0.4f, 0.0f, 1.0f), "Base @ %X", offset(&dynamics->base));
        
        ImGui::Text("Object type: %X", host_byteorder_32(dynamics->base.object_type));
        ImGui::Text("ID card: %X", host_byteorder_32(dynamics->base.idcard));
        ImGui::Text("Flags: %X", host_byteorder_32(dynamics->base.flags));
        ImGui::Text("EndFlags: %X", host_byteorder_32(dynamics->base.endflags));
        
        ImGui::Text("Gravity: %f", f32(dynamics->base.gravity));
        ImGui::Text("Slope limit: %f", f32(dynamics->base.slopelimit));
        ImGui::Text("Slope: %f°", degrees(f32(dynamics->base.slope_cosine)));
        ImGui::Text("Slide: %f", f32(dynamics->base.rebound));
        ImGui::Text("Rebound: %f", f32(dynamics->base.rebound));
        
        const vector3 imposedSpeed = vector3_host_byteorder(dynamics->base.speed_impose);
        const vector3 proposedSpeed = vector3_host_byteorder(dynamics->base.speed_propose);
        const vector3 previousSpeed = vector3_host_byteorder(dynamics->base.speed_previous);
        const vector3 scale = vector3_host_byteorder(dynamics->base.scale);
        const vector3 animSpeed = vector3_host_byteorder(dynamics->base.anim_speed);
        const vector3 safeTranslation = vector3_host_byteorder(dynamics->base.translation_safe);
        const vector3 addedTranslation = vector3_host_byteorder(dynamics->base.translation_add);
        
        ImGui::Spacing();
        ImGui::Text("Imposed speed:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", imposedSpeed.x, imposedSpeed.y, imposedSpeed.z);
        ImGui::Spacing();
        ImGui::Text("Proposed speed:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", proposedSpeed.x, proposedSpeed.y, proposedSpeed.z);
        ImGui::Spacing();
        ImGui::Text("Previous speed: %.3f", sqrt(previousSpeed.x * previousSpeed.x + previousSpeed.y * previousSpeed.y));
        ImGui::Text(" (%.3f, %.3f, %.3f)", previousSpeed.x, previousSpeed.y, previousSpeed.z);
        ImGui::Spacing();
        ImGui::Text("Scale:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", scale.x, scale.y, scale.z);
        ImGui::Spacing();
        ImGui::Text("Animation speed:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", animSpeed.x, animSpeed.y, animSpeed.z);
        ImGui::Spacing();
        ImGui::Text("Safe translation:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", safeTranslation.x, safeTranslation.y, safeTranslation.z);
        ImGui::Spacing();
        ImGui::Text("Added translation:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", addedTranslation.x, addedTranslation.y, addedTranslation.z);
    }
    ImGui::EndChild();
    
    
//    readonly float inertia_x;
//    readonly float inertia_y;
//    readonly float inertia_z;
//    readonly float streamprio;
//    readonly float streamfactor;
//    readonly float slide_factor_x;
//    readonly float slide_factor_y;
//    readonly float slide_factor_z;
//    readonly float slide_previous;
//    readonly struct vector3 speed_max;
//    readonly struct vector3 speed_stream;
//    readonly struct vector3 speed_add;
//    readonly struct vector3 limit;
//    readonly struct vector3 collision_translation;
//    readonly struct vector3 inertia_translation;
//    readonly struct vector3 ground_normal;
//    readonly struct vector3 wall_normal;
//    readonly int8_t collide_count;
//    padding(3)
    
    ImGui::SameLine();
    
    #pragma mark Advanced
    ImGui::BeginChild("Dynamics: advanced block", ImVec2(ImMax(ImGui::GetContentRegionAvail().x / 3, 150.0f), ImGui::GetContentRegionAvail().y / 1.5));
    {
        ImGui::TextColored(ImVec4(0.7f, 0.4f, 0.0f, 1.0f), "Advanced @ %X", offset(&dynamics->advanced));
        
        ImGui::Text("Inertia X: %f", f32(dynamics->advanced.inertia_x));
        ImGui::Text("Inertia Y: %f", f32(dynamics->advanced.inertia_y));
        ImGui::Text("Inertia Z: %f", f32(dynamics->advanced.inertia_z));
        ImGui::Text("Stream priority: %f", f32(dynamics->advanced.streamprio));
        ImGui::Text("Stream factor: %f", f32(dynamics->advanced.streamfactor));
        ImGui::Text("Slide factor X: %f", f32(dynamics->advanced.slide_factor_x));
        ImGui::Text("Slide factor y: %f", f32(dynamics->advanced.slide_factor_y));
        ImGui::Text("Slide factor z: %f", f32(dynamics->advanced.slide_factor_z));
        ImGui::Text("Previous slide: %f", f32(dynamics->advanced.slide_previous));
        ImGui::Text("Slide factor X: %f", f32(dynamics->advanced.slide_factor_x));
        
        ImGui::Spacing();
        
        const vector3 maxSpeed = vector3_host_byteorder(dynamics->advanced.speed_max);
        const vector3 streamSpeed = vector3_host_byteorder(dynamics->advanced.speed_stream);
        const vector3 addSpeed = vector3_host_byteorder(dynamics->advanced.speed_add);
        const vector3 limit = vector3_host_byteorder(dynamics->advanced.limit);
        const vector3 collisionTranslation = vector3_host_byteorder(dynamics->advanced.collision_translation);
        const vector3 inertiaTranslation = vector3_host_byteorder(dynamics->advanced.inertia_translation);
        const vector3 groundNormal = vector3_host_byteorder(dynamics->advanced.ground_normal);
        const vector3 wallNormal = vector3_host_byteorder(dynamics->advanced.wall_normal);
        
        ImGui::Text("Max speed:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", maxSpeed.x, maxSpeed.y, maxSpeed.z);
        ImGui::Spacing();
        
        ImGui::Text("Stream speed:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", streamSpeed.x, streamSpeed.y, streamSpeed.z);
        ImGui::Spacing();
        
        ImGui::Text("+Speed:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", addSpeed.x, addSpeed.y, addSpeed.z);
        ImGui::Spacing();

        ImGui::Text("Limit:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", limit.x, limit.y, limit.z);
        ImGui::Spacing();
        
        ImGui::Text("Collision translation:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", collisionTranslation.x, collisionTranslation.y, collisionTranslation.z);
        ImGui::Spacing();
        
        ImGui::Text("Intertia translation:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", inertiaTranslation.x, inertiaTranslation.y, inertiaTranslation.z);
        ImGui::Spacing();
        
        ImGui::Text("Ground normal:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", groundNormal.x, groundNormal.y, groundNormal.z);
        ImGui::Spacing();
        
        ImGui::Text("Wall normal:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", wallNormal.x, wallNormal.y, wallNormal.z);
        ImGui::Spacing();
        
        ImGui::Text("Collide count: %d", dynamics->advanced.collide_count);
        ImGui::Spacing();
        
    }
    ImGui::EndChild();
    
    const struct dynamics_report* report = (const struct dynamics_report*)pointer(dynamics->base.report);
    if (report)
    {
        ImGui::BeginChild("Dynamics report");
        {
            ImGui::Text("%X", offset(&dynamics->base.report));
            
            vector3 currentspeed = vector3_host_byteorder(report->position_absolute_current.linear);
            vector3 previousspeed = vector3_host_byteorder(report->position_absolute_previous.linear);
            
            ImGui::Text("Current speed: (%.3f, %.3f, %.3f)", currentspeed.x, currentspeed.y, currentspeed.z);
            ImGui::Text("Previous speed: (%.3f, %.3f, %.3f)", previousspeed.x, previousspeed.y, previousspeed.z);
        }
        ImGui::EndChild();
    }
    
    
#undef f32
    
}
