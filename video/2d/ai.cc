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
          setTargetBehavior(behavior);
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
          if (ImGui::Selectable(macro->name.lastPathComponent().c_str())) {
            setTargetMacro(macro);
          }
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
      
      TranslationResult *result = t.translate(targetObject, node);
      
      for (int i = 0; i < result->tokens.size(); i++) {
        TranslationToken& tok = result->tokens[i];
        
        ImVec4 color = defaultColor;
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
            marker(tok, Memory::readonly, actor->instanceName(game::nameResolver).c_str(), quietReferences);
            continue;
          } else if (tok.originalNode->type == SuperObjectRef) {
            pointer<stSuperObject> obj = pointer<stSuperObject>(param);
            //ImGui::TextColored(classColor, "%s", obj->name(game::nameResolver).c_str());
            marker(tok, Memory::readonly, obj->name(game::nameResolver), quietReferences);
            continue;
          } else if (tok.originalNode->type == Subroutine) {
            pointer<stMacro> macro = pointer<stMacro>(param);
            marker(tok, Memory::readonly, macro->name.lastPathComponent(), quietReferences);
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
            uint32_t var = param;
            std::string text = "DsgVar_" + std::to_string(var);
            marker(tok, Memory::readonly, text, quietReferences);
//            ImGui::TextColored(stringColor, "\"%s\"", behavior->name.lastPathComponent().c_str());
//            ImGui::SameLine();
            continue;
          }
        }
        
        ImDrawList *drawlist = ImGui::GetForegroundDrawList();
        drawlist->PushClipRect(ImVec2(), ImVec2());
        
        ImGui::TextColored(color, "%s", text.c_str());
        
        if (text != "\n") {
          ImGui::SameLine();
        } else {
          newLine = true;
        }
      }
      ImGui::PopStyleVar();
    };
    
    ImGui::BeginChild("##script", ImVec2(ImGui::GetContentRegionAvail().x, 0), true);
    
//    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
//    ImGui::BeginChild("##script-line-numbers", ImVec2(18, 0), false);
//    for (int i = 0; i < 100; i++) {
//      ImGui::Text("%02d", i);
//    }
//    ImGui::EndChild();
//    ImGui::SameLine();
//    ImGui::PopStyleVar();
    
    try {
      if (targetBehavior) {
        for (int i = 0; i < targetBehavior->numScripts; i++) {
          pointer<stNodeInterpret> node = targetBehavior->scripts[i]->node;
          draw(node, targetBehavior->name.lastPathComponent().c_str(), 0);
        }
      } else if (targetMacro) {
        draw(targetMacro->currentTree->node, targetMacro->name.lastPathComponent().c_str(), 0);
      }
      
//      ImDrawList *drawlist = ImGui::GetForegroundDrawList();
//      ImVec2 min = ImGui::GetWindowPos();
//     // min.x -= 3;
//      float length = 100;
//      float width = 40;
//      for (int i = 0; i < lineNum; i++) {
//        ImGui::SetCursorScreenPos(ImVec2(min.x, min.y + i * ImGui::GetTextLineHeightWithSpacing()));
//        ImGui::Text("%02d", i);
//      }
      
     //drawlist->AddRectFilled(min, ImVec2(min.x + width, min.y + length), ImColor(255, 255, 255, 10), 5.0f, ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersBottomLeft);
      //drawlist->AddLine(ImVec2(min.x + width, min.y), ImVec2(min.x + width, min.y + length), ImColor(255,255,255,25));
      
    } catch (BadPointer& e) {
      fprintf(stderr, "Failed to draw translated script tree: %s\n", e.what().c_str());
    }
    ImGui::EndChild();
  }
}

void AIWindow::drawInfo() {
  try {
    //targetMacro = targetObject->actor->brain->mind->aiModel->macroList->macros[0];
    pointer<stBehavior> currentIntelligence = targetObject->actor->brain->mind->intelligence->currentBehavior;
    pointer<stBehavior> currentReflex = targetObject->actor->brain->mind->reflex->currentBehavior;
    
    ImGui::BeginChild("##ai-general-info", ImVec2(std::max(200.0f, ImGui::GetContentRegionAvail().x / 5.0f), 100), true);
    
    //ImGui::Text("%s", targetObject->name(game::nameResolver).c_str());
    
    marker(targetObject, true);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), ">");
    ImGui::SameLine();
    marker(targetObject->actor->brain, true);
    ImGui::NewLine();
    
    ImGui::Text("INTL:");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1,1,1,0.75f), "%s", currentIntelligence->name.lastPathComponent().c_str());
    
    ImGui::Text("RFLX:");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1,1,1,0.75f), "%s", currentReflex->name.lastPathComponent().c_str());
    
    
    if (displayActive) {
      targetBehavior = currentIntelligence;
    }
    
    ImGui::EndChild();
  } catch (BadPointer& e) {
    /* ... */
  }
}

AIWindow::AIWindow(pointer<stSuperObject> target) {
  targetObject = target;
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

void AIWindow::draw() {
  if (interface->mode == Speedrun)
    return;
  
  
  
  std::string name = "AI";//"AI - " + targetObject->name(game::nameResolver);
  ImGui::SetNextWindowSize(ImVec2(200,100));
  ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_MenuBar);
  if (targetObject->type == eSuperObjectType::superObjectTypeActor) {
    ImGui::BeginGroup();
    drawMenuBar();
    drawInfo();
    drawBehaviorLists();
    ImGui::EndGroup();
    ImGui::SameLine();
    drawScript();
  } else {
    ImGui::Text("Invalid target actor");
  }
  ImGui::End();
}
