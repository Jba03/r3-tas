#include "constants.hh"

#define TC(label, ...)     \
  ImGui::TableNextColumn();       \
  ImGui::Text(label);             \
  ImGui::TableNextColumn();       \
  ImGui::Text(__VA_ARGS__);       \

static auto radians(float degrees) -> float {
  return degrees * (M_PI / 180.0f);
}

static auto degrees(float radians) -> float {
  return radians * (180.0f / M_PI);
}

static auto drawDynamicsReport(stDynamicsReport *report) -> void {
  
}

static auto drawDynamics(stDynamics *dynamics) -> void {
  
  stDynamicsBaseBlock base = dynamics->base;
  stDynamicsAdvancedBlock advanced = dynamics->advanced;
  stDynamicsComplexBlock complex = dynamics->complex;
  
  ImGui::BeginChild("Container", ImVec2(ImGui::GetContentRegionAvail().x, 300.0f));
  #pragma mark Base
  ImGui::BeginChild("Dynamics: base block", ImVec2(std::max(250.0f, ImGui::GetContentRegionAvail().x / 3), 0));
  {
    ImGui::TextColored(ImVec4(0.7f, 0.4f, 0.0f, 1.0f), "Base @ %X", dynamics->base.objectType.memoryOffset().physicalAddress());
    if (ImGui::BeginTable("Dynamics base block", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg))
    {
      stVector3D imposedSpeed = base.imposeSpeed;
      stVector3D proposedSpeed = base.proposeSpeed;
      stVector3D previousSpeed = base.previousSpeed;
      stVector3D scale = base.scale;
      stVector3D animSpeed = base.animationProposeSpeed;
      stVector3D safeTranslation = base.safeTranslation;
      stVector3D addedTranslation = base.addTranslation;
      
      TC("Object type", "%X", uint32_t(base.objectType))
      TC("ID card", "%X", uint32_t(base.idcard))
      TC("Flags", "%X", uint32_t(base.flags))
      TC("Endflags", "%X", uint32_t(base.endFlags))
      TC("Gravity", "%f", float(base.gravity))
      TC("Slope limit", "%f", float(base.slopeLimit));
      TC("Slope", "%f°", degrees(base.slopeCosine));
      TC("Slide", "%f", float(base.rebound));
      TC("Rebound", "%f", float(base.rebound));
              
      TC("Impose speed", "[%.2f, %.2f, %.2f]", (float)imposedSpeed.x, (float)imposedSpeed.y, (float)imposedSpeed.z);
      TC("Propose speed", "[%.2f, %.2f, %.2f]", (float)proposedSpeed.x, (float)proposedSpeed.y, (float)proposedSpeed.z);
      TC("Previous speed", "[%.2f, %.2f, %.2f]", (float)previousSpeed.x, (float)previousSpeed.y, (float)previousSpeed.z);
      TC("Scale", "[%.2f, %.2f, %.2f]", (float)scale.x, (float)scale.y, (float)scale.z);
      TC("Anim proposespeed", "[%.2f, %.2f, %.2f]", (float)animSpeed.x, (float)animSpeed.y, (float)animSpeed.z);
      TC("Safe translation", "[%.2f, %.2f, %.2f]", (float)safeTranslation.x, (float)safeTranslation.y, (float)safeTranslation.z);
      TC("Added translation", "[%.2f, %.2f, %.2f]", (float)addedTranslation.x, (float)addedTranslation.y, (float)addedTranslation.z);
              
      ImGui::EndTable();
    }
  }
  ImGui::EndChild();
  
  if (dynamics->endFlag(dynamicsSizeAdvanced)) {
    ImGui::SameLine();
    ImGui::BeginChild("Dynamics: advanced block", ImVec2(std::max(275.0f, ImGui::GetContentRegionAvail().x / 2), 0));
    ImGui::TextColored(ImVec4(0.7f, 0.4f, 0.0f, 1.0f), "Advanced @ %X", dynamics->advanced.xInertia.memoryOffset().physicalAddress());
    if (ImGui::BeginTable("Dynamics advanced block", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg)) {
      stVector3D maxSpeed = advanced.maxSpeed;
      stVector3D streamSpeed = advanced.streamSpeed;
      stVector3D addSpeed = advanced.addedSpeed;
      stVector3D limit = advanced.limit;
      stVector3D collisionTranslation = advanced.collisionTranslation;
      stVector3D inertiaTranslation = advanced.inertiaTranslation;
      stVector3D groundNormal = advanced.groundNormal;
      stVector3D wallNormal = advanced.wallNormal;

      TC("Inertia", "[%.2f, %.2f, %.2f]", float(advanced.xInertia), float(advanced.yInertia), float(advanced.zInertia));
      TC("Stream priority", "%f", float(advanced.streamPriority));
      TC("Stream factor", "%f", float(advanced.streamFactor));
      TC("Slide factor", "[%.2f, %.2f, %.2f]", float(advanced.xSlideFactor), float(advanced.ySlideFactor), float(advanced.zSlideFactor));
      TC("Previous slide", "%f", float(advanced.previousSlide));
      TC("Max speed", "[%.2f, %.2f, %.2f]", (float)maxSpeed.x, (float)maxSpeed.y, (float)maxSpeed.z);
      TC("Stream speed", "[%.2f, %.2f, %.2f]", (float)streamSpeed.x, (float)streamSpeed.y, (float)streamSpeed.z);
      TC("Add speed", "[%.2f, %.2f, %.2f]", (float)addSpeed.x, (float)addSpeed.y, (float)addSpeed.z);
      TC("Limit", "[%.2f, %.2f, %.2f]", (float)limit.x, (float)limit.y, (float)limit.z);
      TC("Col. translation", "[%.2f, %.2f, %.2f]", (float)collisionTranslation.x, (float)collisionTranslation.y, (float)collisionTranslation.z);
      TC("Inert. translation", "[%.2f, %.2f, %.2f]", (float)inertiaTranslation.x, (float)inertiaTranslation.y, (float)inertiaTranslation.z);
      TC("Ground normal", "[%.2f, %.2f, %.2f]", (float)groundNormal.x, (float)groundNormal.y, (float)groundNormal.z);
      TC("Wall normal", "[%.2f, %.2f, %.2f]", (float)wallNormal.x, (float)wallNormal.y, (float)wallNormal.z);
      TC("Collide count", "%d", (int)advanced.collideCount);

      ImGui::EndTable();
    }
    ImGui::EndChild();
  }
  
  if (dynamics->endFlag(dynamicsSizeComplex)) {
    ImGui::SameLine();
    ImGui::BeginChild("Dynamics: complex block", ImVec2(std::max(250.0f, ImGui::GetContentRegionAvail().x / 2), 0));
    ImGui::TextColored(ImVec4(0.7f, 0.4f, 0.0f, 1.0f), "Complex @ %X", dynamics->complex.tiltStrength.memoryOffset().physicalAddress());
    if (ImGui::BeginTable("Dynamics complex block", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg)) {
      stVector3D contact = complex.contact;
      stVector3D fallTranslation = complex.fallTranslation;

      TC("Tilt strength", "%f", float(complex.tiltStrength));
      TC("Tilt inertia", "%f", float(complex.tiltInertia));
      TC("Tilt origin", "%f", float(complex.tiltOrigin));
      TC("Tilt angle", "%f", float(complex.tiltAngle));
      TC("Hanging limit", "%f", float(complex.hangingLimit));
      TC("Contact", "[%.2f, %.2f, %.2f]", (float)contact.x, (float)contact.y, (float)contact.z);
      TC("Fall translation", "[%.2f, %.2f, %.2f]", (float)fallTranslation.x, (float)fallTranslation.y, (float)fallTranslation.z);
      TC("Platform SO", "%X", (uint32_t)complex.platformSuperObject);

      ImGui::EndTable();
    }
    ImGui::EndChild();
  }
      
  ImGui::EndChild();
}
