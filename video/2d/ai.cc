#include "gui.hh"
#include "script.hpp"
#include "constants.hh"
#include "tables.hh"
#include "imgui_internal.h"

#include <sstream>

using namespace cpa::script;

#pragma mark - DSG

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

static ImColor dsgVarColorTable[] = {
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


static std::string dsgVarFormatBoolean(pointer<uint8> v) {
  return *(uint8*)v ? "TRUE" : "FALSE";
}

template <typename T>
static std::string dsgVarFormatIntegral(pointer<T> v) {
  return std::to_string(*(T*)v);
}

static std::string dsgVarFormatFloat(pointer<float> v) {
  std::stringstream s;
  s << std::setprecision(3);
  s << *(float*)v;
  return s.str();
}

static std::string dsgVarFormatVector(pointer<stVector3D> v) {
  std::stringstream s;
  s << std::setprecision(3);
  s << "(" << float(v->x) << ", " << float(v->y) << ", " << float(v->z) << ")";
  return s.str();
}

static std::map<DsgVarType, std::function<std::string(pointer<>)>> dsgFormatTable {
  { DsgVarType::Boolean, dsgVarFormatBoolean },
  { DsgVarType::Byte, dsgVarFormatIntegral<int8> },
  { DsgVarType::UByte, dsgVarFormatIntegral<uint8> },
  { DsgVarType::Short, dsgVarFormatIntegral<int16> },
  { DsgVarType::UShort, dsgVarFormatIntegral<uint16> },
  { DsgVarType::Int, dsgVarFormatIntegral<int32> },
  { DsgVarType::UInt, dsgVarFormatIntegral<uint32> },
  { DsgVarType::Float, dsgVarFormatFloat },
  { DsgVarType::Vector, &dsgVarFormatVector },
  { DsgVarType::List, nullptr },
  { DsgVarType::Comport, nullptr },
  { DsgVarType::Action, nullptr },
  { DsgVarType::Capabilities, nullptr },
  { DsgVarType::Input, nullptr },
  { DsgVarType::SoundEvent, nullptr },
  { DsgVarType::Light, nullptr },
  { DsgVarType::GameMaterial, nullptr },
  { DsgVarType::VisualMaterial, nullptr },
  { DsgVarType::Actor, nullptr },
  { DsgVarType::Waypoint, nullptr },
  { DsgVarType::Graph, nullptr },
  { DsgVarType::Text, nullptr },
  { DsgVarType::SuperObject, nullptr },
  { DsgVarType::SOLinks, nullptr },
  { DsgVarType::ActorArray, nullptr },
  { DsgVarType::VectorArray, nullptr },
  { DsgVarType::FloatArray, nullptr },
  { DsgVarType::IntArray, nullptr },
  { DsgVarType::WaypointArray, nullptr },
  { DsgVarType::TextArray, nullptr },
  { DsgVarType::TextRefArray, nullptr },
  { DsgVarType::GraphArray, nullptr },
  { DsgVarType::Array9, nullptr },
  { DsgVarType::SNDEventArray, nullptr },
  { DsgVarType::Array11, nullptr },
  { DsgVarType::Way, nullptr },
  { DsgVarType::ActionArray, nullptr },
  { DsgVarType::SuperObjectrArray, nullptr },
  { DsgVarType::ObjectList, nullptr },
};

static std::string dsgVarFormat(pointer<stDsgVarInfo> info, pointer<> memory) {
  try {
    DsgVarType type = static_cast<DsgVarType>((uint32_t)info->type);
    if (dsgFormatTable.find(type) != dsgFormatTable.end()) {
      std::function<std::string(pointer<>)> fn = dsgFormatTable[type];
      return fn ? fn(memory) : "";
    }
  } catch (bad_pointer& e) {
      
  }
  return "";
}

#pragma mark - Script

void AIWindow::drawBehaviorLists() {
  auto list = [this](pointer<stScriptAI> list, pointer<stBehavior> current) {
    for (int i = 0; i < list->numBehaviors; i++) {
      pointer<stBehavior> behavior = list->behavior[i];
      ImGui::PushStyleColor(ImGuiCol_Text, behavior == current ? ImVec4(0.1f, 1.0f, 0.25f, 1.0f) :  ImVec4(1.0f, 1.0f, 1.0f, 0.75f));
      if (ImGui::Selectable(behavior->name.lastPathComponent().c_str())) {
        setTargetBehavior(behavior);
      }
      ImGui::PopStyleColor();
    }
  };
  
  pointer<stAIModel> aiModel;
  pointer<stScriptAI> intelligenceList;
  pointer<stScriptAI> reflexList;
  pointer<stMacroList> macroList;
  pointer<stBehavior> currentIntelligenceBehavior;
  pointer<stBehavior> currentReflexBehavior;
  
  try {
    aiModel = targetObject->actor->aiModel();
    intelligenceList = aiModel->intelligenceBehaviorList;
    reflexList = aiModel->reflexBehaviorList;
    macroList = aiModel->macroList;
    currentIntelligenceBehavior = targetObject->actor->brain->mind->intelligence->currentBehavior;
    currentReflexBehavior = targetObject->actor->brain->mind->reflex->currentBehavior;
  } catch (bad_pointer& e) {
    /* ... */
  }
  
  try {
    if (ImGui::BeginChild("Intelligence", ImVec2(std::max(200.0f, ImGui::GetContentRegionAvail().x / 5.0f), 0), true, ImGuiWindowFlags_HorizontalScrollbar)) {
      if (intelligenceList) {
        if (ImGui::TreeNode("Intelligence")) {
          list(intelligenceList, currentIntelligenceBehavior);
          ImGui::TreePop();
        }
      }
      
      if (reflexList) {
        if (ImGui::TreeNode("Reflex")) {
          list(reflexList, currentReflexBehavior);
          ImGui::TreePop();
        }
      }
      
      if (macroList) {
        if (ImGui::TreeNode("Macro")) {
          for (uint8 n = 0; n < macroList->numMacros; n++) {
            pointer<stMacro> macro = macroList->macros[n];
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.75f));
            if (ImGui::Selectable(macro->name.lastPathComponent().c_str())) {
              setTargetMacro(macro);
            }
            ImGui::PopStyleColor();
          }
          ImGui::TreePop();
        }
      }
      
      
    }
    ImGui::EndChild();
  } catch (cpa::bad_pointer& e) {
    std::cout << "Failed to draw behavior list: " << e.what() << "\n";
  }
}

void AIWindow::drawInfo() {
  pointer<stBehavior> currentIntelligence = nullptr;
  pointer<stBehavior> currentReflex = nullptr;
  
  ImGui::BeginChild("##ai-general-info", ImVec2(ImGui::GetContentRegionAvail().x, 28), true);
  
  
  try { marker(targetObject, true); } catch (...) {}
  ImGui::SameLine();
  
  try {
    currentIntelligence = targetObject->actor->brain->mind->intelligence->currentBehavior;
    ImGui::TextColored(ImVec4(1,1,1,0.75f), "%s", currentIntelligence->name.lastPathComponent().c_str());
    
    if (displayActive) targetBehavior = currentIntelligence;
  } catch (bad_pointer& e) {
    /* ... */
  }
  
  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();
  
  try {
    currentReflex = targetObject->actor->brain->mind->reflex->currentBehavior;
    ImGui::TextColored(ImVec4(1,1,1,0.75f), "%s", currentReflex->name.lastPathComponent().c_str());
  } catch(bad_pointer&e) {
    /* ... */
  }
  
  ImGui::EndChild();
}

void AIWindow::setTargetMacro(pointer<stMacro> macro) {
  targetMacro = macro;
  targetBehavior = nullptr;
}

void AIWindow::setTargetBehavior(pointer<stBehavior> behavior) {
  targetMacro = nullptr;
  targetBehavior = behavior;
}

void AIWindow::drawScript() {
  if (true) {
    TranslationOptions opt;
    opt.expandMacroReferences = false;
    opt.removeUnnecessaryParentheses = true;
    
    opt.conditionTable = R3ConditionTable;
    opt.functionTable = R3FunctionTable;
    opt.procedureTable = R3ProcedureTable;
    opt.metaActionTable = R3MetaActionTable;
    opt.fieldTable = R3FieldTable;
    
    TranslationEngine t(opt);
    
    int lineNum = 1;
    auto draw = [this, &t, &lineNum](pointer<stNodeInterpret> node, const char *name, int idx) {
      ImVec4 defaultColor = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
      ImVec4 lineNumColor = ImVec4(1.0f, 1.0f, 1.0f, 0.25f);
      ImVec4 stringColor = ImVec4(144.0f / 255.0f, 210.0f / 255.0f, 130.0f / 255.0f, 1.0f);
      ImVec4 classColor = ImVec4(229.0f / 255.0f, 193.0f / 255.0f, 124.0f / 255.0f, 1.0f);
      
      if (drawLineNumbers) {
        ImGui::TextColored(lineNumColor, "%03d", lineNum++);
        ImGui::SameLine();
      }
      
      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
      ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.4), "// %s.%d @ %X", name, idx, node->param.memoryOffset().effectiveAddress());
      if (drawLineNumbers) {
        ImGui::TextColored(lineNumColor, "%03d ", lineNum++);
        ImGui::SameLine();
      }
      
      bool newLine = false;
      pointer<stEngineObject> referenceActorTmp = nullptr;
      
      TranslationResult *result = t.translate(targetObject, node);
      
      for (int i = 0; i < result->tokens.size(); i++) {
        TranslationToken& tok = result->tokens[i];
        
        ImVec4 color = defaultColor;
        if (tok.originalNode) {
          try {
            switch (tok.originalNode->type) {
              case ScriptNodeType::Keyword: color = ImVec4(198.0f / 255.0f, 121.0f / 255.0f, 221.0f / 255.0f, 1.0f); break;
              case ScriptNodeType::Condition: if (uint32_t(tok.originalNode->param) > 9)
              case ScriptNodeType::Function:
              case ScriptNodeType::Procedure: color = ImVec4(0.3, 0.5, 1.0f, 1.0f); break;
              case ScriptNodeType::Constant:
              case ScriptNodeType::Real: color = ImVec4(210.0f / 255.0f, 148.0f / 255.0f, 93.0f / 255.0f, 1.0f); break;
              case ScriptNodeType::DsgVarRef:
              case ScriptNodeType::DsgVarRef2: color = ImVec4(0.9, 0.4, 0.45, 1.0f); break;
              case ScriptNodeType::ConstantVector:
              case ScriptNodeType::Vector: color = ImVec4(229.0f / 255.0f, 193.0f / 255.0f, 124.0f / 255.0f, 1.0f); break;
              case ScriptNodeType::Field: color = ImVec4(170.0f / 255.0f, 13.0f / 255.0f, 145.0f / 255.0f, 1.0f); break;
              case ScriptNodeType::Subroutine: color = ImVec4(84.0f / 255.0f, 222.0f / 255.0f, 101.0f / 255.0f, 1.0f); break;
              case ScriptNodeType::String: color = ImVec4(144.0f / 255.0f, 195.0f / 255.0f, 120.0f / 255.0f, 1.0f); break;
            }
          } catch (bad_pointer& e) {
            
          }
        }
        
        std::string text = tok.text;
        
        if (newLine) {
          if (drawLineNumbers) {
            ImGui::TextColored(lineNumColor, "%03d ", lineNum++);
            ImGui::SameLine();
          }
          newLine = false;
        }
        
       
        
        if (tok.originalNode) {
          uint32_t type = tok.originalNode->type;
          uint32_t param = tok.originalNode->param;
          
          if (tok.originalNode->type == ActionRef) {
            pointer<stActionTableEntry> entry = pointer<stActionTableEntry>(param);
            ImGui::TextColored(stringColor, "%s", entry->name.lastPathComponent().c_str());
            ImGui::SameLine();
            continue;
          } else if (tok.originalNode->type == ActorRef) {
            pointer<stEngineObject> actor = pointer<stEngineObject>(param);
//            ImGui::TextColored(classColor, "%s", actor->instanceName(game::nameResolver).c_str());
//            ImGui::SameLine();
            marker(tok, memory::readonly, actor->name(Instance).c_str(), quietReferences);
            ImGui::SameLine();
            referenceActorTmp = actor;
            continue;
          } else if (tok.originalNode->type == SuperObjectRef) {
            pointer<stSuperObject> obj = pointer<stSuperObject>(param);
            //ImGui::TextColored(classColor, "%s", obj->name(game::nameResolver).c_str());
            marker(tok, memory::readonly, obj->name(), quietReferences);
            ImGui::SameLine();
            continue;
          } else if (tok.originalNode->type == Subroutine) {
            pointer<stMacro> macro = pointer<stMacro>(param);
            marker(tok, memory::readonly, macro->name.lastPathComponent(), quietReferences);
            ImGui::SameLine();
//            ImGui::TextColored(color, "%s", macro->name.lastPathComponent().c_str());
//            if (ImGui::IsItemClicked()) setTargetMacro(macro);
//            ImGui::SameLine();
            continue;
          } else if (tok.originalNode->type == ActionRef) {
            pointer<stActionTableEntry> entry = pointer<stActionTableEntry>(param);
            
          } else if (tok.originalNode->type == BehaviorRef) {
            pointer<stBehavior> behavior = pointer<stBehavior>(param);
            ImGui::TextColored(stringColor, "\"%s\"", behavior->name.lastPathComponent().c_str());
            ImGui::SameLine();
            continue;
          } else if (tok.originalNode->type == DsgVarRef || tok.originalNode->type == DsgVarRef2) {
            uint32_t var = param, type = 0;
            pointer<stEngineObject> obj = (referenceActorTmp.pointee() != nullptr) ? referenceActorTmp : targetObject->actor;
            obj->dsgVar(var, &type);
            
            std::string txt = DsgVarTypenameTable[type] + "_" + std::to_string(var);
            marker(tok, memory::readonly, txt, quietReferences);
            ImGui::SameLine();
            
            // clear the actor reference
            referenceActorTmp = nullptr;
            continue;
          }
        }
        
        ImGui::TextColored(color, "%s", text.c_str());
        
        if (text != "\n") {
          ImGui::SameLine();
        } else {
          newLine = true;
        }
      }
      ImGui::PopStyleVar();
    };
    
    ImGui::BeginChild("##script", ImVec2(ImGui::GetContentRegionAvail().x, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
    try {
      if (targetBehavior) {
        for (int i = 0; i < targetBehavior->numScripts; i++) {
          pointer<stNodeInterpret> node = targetBehavior->scripts[i]->node;
          draw(node, targetBehavior->name.lastPathComponent().c_str(), 0);
        }
      } else if (targetMacro) {
        draw(targetMacro->currentTree->node, targetMacro->name.lastPathComponent().c_str(), 0);
      }
    } catch (bad_pointer& e) {
      ImGui::PopStyleVar();
      fprintf(stderr, "Failed to draw translated script tree: %s\n", e.what().c_str());
    }
    ImGui::EndChild();
  }
}

void AIWindow::drawDsgVars() {
  try {
    pointer<stMind> mind = targetObject->actor->brain->mind;
    pointer<stDsgMem> mem = mind->dsgMem;
    pointer<stDsgVar> vars = mem->dsgVars;
    pointer<> memory = mem->currentBuffer;
    
    if (ImGui::BeginChild("Designer variables", ImVec2(std::max(200.0f, ImGui::GetContentRegionAvail().x / 5.0f), 0), true, ImGuiWindowFlags_HorizontalScrollbar)) {
      for (int i = 0; i < vars->infoLength; i++) {
        pointer<stDsgVarInfo> info = mem->dsgVarInfo(i);
        pointer<> data = (uint8_t*)memory.pointee() + info->memoryOffset;
        std::string format = dsgVarFormat(info, data);
        std::string name = DsgVarTypenameTable[info->type];
        ImGui::TextColored(dsgVarColorTable[info->type], "%s_%d: %s", name.c_str(), i, format.c_str());
      }
    }
    ImGui::EndChild();
    
  } catch (bad_pointer& e) {
    /* ... */
  }
}

AIWindow::AIWindow(pointer<stSuperObject> target) {
  setTargetObject(target);
}

void AIWindow::setTargetObject(pointer<stSuperObject> target) {
  targetObject = target;
  targetBehavior = nullptr;
}

void AIWindow::drawMenuBar() {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("AIModel")) {
      ImGui::Checkbox("Always display current schedule", &displayActive);
      ImGui::EndMenu();
    }
    
    if (ImGui::BeginMenu("View")) {
      if (ImGui::BeginMenu("Script")) {
        ImGui::Checkbox("Quiet reference markers", &quietReferences);
        ImGui::Checkbox("Show line numbers", &drawLineNumbers);
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
  }
  ImGui::EndMenuBar();
}

void AIWindow::drawDebugPanel() {
  ImGui::BeginChild("Debugger", ImVec2(ImGui::GetContentRegionAvail().x, 29), true);
  ImGui::Button("Continue");
  ImGui::SameLine();
  ImGui::Button("Stop");
  ImGui::SameLine();
  ImGui::InputInt("Steps", &debuggerStep);
  ImGui::EndChild();
}

void AIWindow::draw() {
  if (interface->mode == Speedrun)
    return;
  
  std::string name = "AI"; //"AI - " + targetObject->name(game::nameResolver);
  ImGui::SetNextWindowSize(ImVec2(200,100));
  ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_MenuBar);
  if (targetObject->type == stSuperObject::type::Actor) {
    drawMenuBar();
    drawInfo();
    ImGui::BeginGroup();
    drawBehaviorLists();
    ImGui::EndGroup();
    ImGui::SameLine();
    drawDsgVars();
    ImGui::SameLine();
    ImGui::BeginGroup();
    drawDebugPanel();
    drawScript();
    ImGui::EndGroup();
  } else {
    ImGui::Text("Invalid target actor");
  }
  ImGui::End();
}
