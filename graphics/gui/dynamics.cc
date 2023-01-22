#include "actor.h"
#include "dynamics.h"

static void display_dynamics(const struct dynamics* dynamics)
{
    if (!dynamics) return;
    
    const char* type = "Base";
    if (host_byteorder_32(dynamics->base.endflags) & dynamics_size_advanced) type = "Advanced";
    if (host_byteorder_32(dynamics->base.endflags) & dynamics_size_complex) type = "Complex";
    
    //DisplayBits((dynamics->base.idcard));
    
    
#define f32 host_byteorder_f32
    
    #pragma mark Base
    ImGui::BeginChild("Dynamics: base block", ImVec2(ImMax(ImGui::GetContentRegionAvail().x / 3, 150.0f), ImGui::GetContentRegionAvail().y));
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
        
        ImGui::Spacing();
        ImGui::Text("Imposed speed:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->base.speed_impose.x), f32(dynamics->base.speed_impose.y), f32(dynamics->base.speed_impose.z));
        ImGui::Spacing();
        ImGui::Text("Proposed speed:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->base.speed_propose.x), f32(dynamics->base.speed_propose.y), f32(dynamics->base.speed_propose.z));
        ImGui::Spacing();
        ImGui::Text("Previous speed:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->base.speed_previous.x), f32(dynamics->base.speed_previous.y), f32(dynamics->base.speed_previous.z));
        ImGui::Spacing();
        ImGui::Text("Scale:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->base.scale.x), f32(dynamics->base.scale.y), f32(dynamics->base.scale.z));
        ImGui::Spacing();
        ImGui::Text("Animation speed:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->base.anim_speed.x), f32(dynamics->base.anim_speed.y), f32(dynamics->base.anim_speed.z));
        ImGui::Spacing();
        ImGui::Text("Safe translation:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->base.translation_safe.x), f32(dynamics->base.translation_safe.y), f32(dynamics->base.translation_safe.z));
        ImGui::Spacing();
        ImGui::Text("Added translation:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->base.translation_add.x), f32(dynamics->base.translation_add.y), f32(dynamics->base.translation_add.z));
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
    ImGui::BeginChild("Dynamics: advanced block", ImVec2(ImMax(ImGui::GetContentRegionAvail().x / 3, 150.0f), ImGui::GetContentRegionAvail().y));
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
        
        ImGui::Text("Max speed:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->advanced.speed_max.x), f32(dynamics->advanced.speed_max.y), f32(dynamics->advanced.speed_max.z));
        ImGui::Spacing();
        
        ImGui::Text("Stream speed:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->advanced.speed_stream.x), f32(dynamics->advanced.speed_stream.y), f32(dynamics->advanced.speed_stream.z));
        ImGui::Spacing();
        
        ImGui::Text("+Speed:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->advanced.speed_add.x), f32(dynamics->advanced.speed_add.y), f32(dynamics->advanced.speed_add.z));
        ImGui::Spacing();

        ImGui::Text("Limit:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->advanced.limit.x), f32(dynamics->advanced.limit.y), f32(dynamics->advanced.limit.z));
        ImGui::Spacing();
        
        ImGui::Text("Collision translation:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->advanced.collision_translation.x), f32(dynamics->advanced.collision_translation.y), f32(dynamics->advanced.collision_translation.z));
        ImGui::Spacing();
        
        ImGui::Text("Intertia translation:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->advanced.inertia_translation.x), f32(dynamics->advanced.inertia_translation.y), f32(dynamics->advanced.inertia_translation.z));
        ImGui::Spacing();
        
        ImGui::Text("Ground normal:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->advanced.ground_normal.x), f32(dynamics->advanced.ground_normal.y), f32(dynamics->advanced.ground_normal.z));
        ImGui::Spacing();
        
        ImGui::Text("Wall normal:");
        ImGui::Text(" (%.3f, %.3f, %.3f)", f32(dynamics->advanced.wall_normal.x), f32(dynamics->advanced.wall_normal.y), f32(dynamics->advanced.wall_normal.z));
        ImGui::Spacing();
        
        ImGui::Text("Collide count: %d", dynamics->advanced.collide_count);
        ImGui::Spacing();
        
    }
    ImGui::EndChild();
    
    
#undef f32
    
}
