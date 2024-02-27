#include "gui.hh"
#include "script.hh"
#include "constants.hh"
#include "tables.hh"

using namespace CPA::Script;

void AIWindow::drawBehaviorLists() {
  try {
    pointer<stAIModel> aiModel = targetObject->actor->aiModel();
    pointer<stScriptAI> intelligenceList = aiModel->intelligenceBehaviorList;
    pointer<stScriptAI> reflexList = aiModel->reflexBehaviorList;
    pointer<stMacroList> macroList = aiModel->macroList;
    pointer<stBehavior> currentIntelligenceBehavior = targetObject->actor->brain->mind->intelligence->currentBehavior;
    pointer<stBehavior> currentReflexBehavior = targetObject->actor->brain->mind->reflex->currentBehavior;
    
    auto list = [this](pointer<stScriptAI> list, pointer<stBehavior> current) {
      for (int i = 0; i < list->numBehaviors; i++) {
        pointer<stBehavior> behavior = list->behavior[i];
        ImGui::PushStyleColor(ImGuiCol_Text, behavior == current ? ImVec4(0.1f, 1.0f, 0.25f, 1.0f) :  ImVec4(1.0f, 1.0f, 1.0f, 0.75f));
        if (ImGui::Selectable(behavior->name.lastPathComponent().c_str())) {
          targetBehavior = behavior;
        }
        ImGui::PopStyleColor();
      }
    };
    
    if (ImGui::BeginChild("Intelligence", ImVec2(std::max(200.0f, ImGui::GetContentRegionAvail().x / 5.0f), 0), true)) {
      if (ImGui::TreeNode("Intelligence")) {
        list(intelligenceList, currentIntelligenceBehavior);
        ImGui::TreePop();
      }
      
      if (ImGui::TreeNode("Reflex")) {
        list(reflexList, currentReflexBehavior);
        ImGui::TreePop();
      }
      
      if (ImGui::TreeNode("Macro")) {
        for (uint8 n = 0; n < macroList->numMacros; n++) {
          pointer<stMacro> macro = macroList->macros[n];
          ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.75f));
          ImGui::Selectable(macro->name.lastPathComponent().c_str());
          ImGui::PopStyleColor();
        }
        ImGui::TreePop();
      }
      
      
    }
    ImGui::EndChild();
  } catch (CPA::BadPointer& e) {
    std::cout << "Failed to draw behavior list: " << e.what() << "\n";
  }
}

void AIWindow::drawScript() {
  if (targetBehavior) {
    TranslationOptions opt;
    opt.expandMacroReferences = false;
    opt.removeUnnecessaryParentheses = true;
    
    opt.conditionTable = R3ConditionTable;
    opt.functionTable = R3FunctionTable;
    opt.procedureTable = R3ProcedureTable;
    opt.metaActionTable = R3MetaActionTable;
    opt.fieldTable = R3FieldTable;
    
    TranslationEngine t(opt);
    
    ImGui::BeginChild("##Script", ImVec2(ImGui::GetContentRegionAvail().x, 0), true);
      for (int i = 0; i < targetBehavior->numScripts; i++) {
        pointer<stNodeInterpret> node = targetBehavior->scripts[i]->node;
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.4), "// %s.%d @ %X", targetBehavior->name.lastPathComponent().c_str(), i, node.memoryOffset().effectiveAddress());
        TranslationResult *result = t.translate(targetObject, node);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
        for (TranslationToken& tok : result->tokens) {
          ImVec4 color = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
          if (tok.originalNode) {
            try {
              switch (tok.originalNode->type) {
                case Keyword: color = ImVec4(198.0f / 255.0f, 121.0f / 255.0f, 221.0f / 255.0f, 1.0f); break;
                case Condition:
                case Function:
                case Procedure: color = ImVec4(0.3, 0.5, 1.0f, 1.0f); break;
                case Constant:
                case Real: color = ImVec4(210.0f / 255.0f, 148.0f / 255.0f, 93.0f / 255.0f, 1.0f); break;
                case DsgVarRef:
                case DsgVarRef2: color = ImVec4(0.9, 0.4, 0.45, 1.0f); break;
                case ConstantVector:
                case Vector: color = ImVec4(229.0f / 255.0f, 193.0f / 255.0f, 124.0f / 255.0f, 1.0f); break;
                case Field: color = ImVec4(170.0f / 255.0f, 13.0f / 255.0f, 145.0f / 255.0f, 1.0f); break;
                case Subroutine: color = ImVec4(84.0f / 255.0f, 222.0f / 255.0f, 101.0f / 255.0f, 1.0f); break;
                case ScriptNodeType::String: color = ImVec4(144.0f / 255.0f, 195.0f / 255.0f, 120.0f / 255.0f, 1.0f); break;
              }
            } catch (BadPointer& e) {
              
            }
          }
          
          std::string text = tok.text;
          
          
          static ImVec4 stringColor = ImVec4(144.0f / 255.0f, 210.0f / 255.0f, 130.0f / 255.0f, 1.0f);
          static ImVec4 classColor = ImVec4(229.0f / 255.0f, 193.0f / 255.0f, 124.0f / 255.0f, 1.0f);
          
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
              ImGui::TextColored(classColor, "%s", actor->instanceName(game::nameResolver).c_str());
              ImGui::SameLine();
              continue;
            } else if (tok.originalNode->type == SuperObjectRef) {
              pointer<stSuperObject> obj = pointer<stSuperObject>(param);
              ImGui::TextColored(classColor, "%s", obj->name(game::nameResolver).c_str());
              ImGui::SameLine();
              continue;
            } else if (tok.originalNode->type == ActionRef) {
              pointer<stActionTableEntry> entry = pointer<stActionTableEntry>(param);
              
            } else if (tok.originalNode->type == ActionRef) {
              pointer<stActionTableEntry> entry = pointer<stActionTableEntry>(param);
              
            } else if (tok.originalNode->type == BehaviorRef) {
              pointer<stBehavior> behavior = pointer<stBehavior>(param);
              ImGui::TextColored(stringColor, "\"%s\"", behavior->name.lastPathComponent().c_str());
              ImGui::SameLine();
              continue;
            }
          }
          
          ImGui::TextColored(color, "%s", text.c_str());
          
          if (text != "\n") ImGui::SameLine();
        }
        ImGui::PopStyleVar();
      }
    ImGui::EndChild();
  }
}

void AIWindow::drawInfo() {
  
}

AIWindow::AIWindow(pointer<stSuperObject> target) {
  targetObject = target;
}

void AIWindow::draw() {
  if (interface->mode == Speedrun)
    return;
  
  std::string name = "AI";//"AI - " + targetObject->name(game::nameResolver);
  ImGui::SetNextWindowSize(ImVec2(200,100));
  ImGui::Begin(name.c_str());
  if (targetObject->type == eSuperObjectType::superObjectTypeActor) {
    drawBehaviorLists();
    ImGui::SameLine();
    drawScript();
  } else {
    ImGui::Text("Invalid target actor");
  }
  ImGui::End();
}
