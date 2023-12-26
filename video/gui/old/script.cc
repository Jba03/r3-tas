#include "tables.hh"

#include "stTreeInterpret.hh"

#include <map>

const ImVec4 syntaxColorDefault = ImVec4(0.75,0.75,0.75,1);
const ImVec4 syntaxColorRed     = ImVec4(0.9, 0.4, 0.45, 1.0f);
const ImVec4 syntaxColorGreen   = ImVec4(84.0f / 255.0f, 222.0f / 255.0f, 101.0f / 255.0f, 1.0f);
const ImVec4 syntaxColorLightGreen = ImVec4(144.0f / 255.0f, 195.0f / 255.0f, 120.0f / 255.0f, 1.0f);
const ImVec4 syntaxColorBlue    = ImVec4(0.3, 0.5, 1.0f, 1.0f);
const ImVec4 syntaxColorYellow  = ImVec4(229.0f / 255.0f, 193.0f / 255.0f, 124.0f / 255.0f, 1.0f);
const ImVec4 syntaxColorOrange  = ImVec4(210.0f / 255.0f, 148.0f / 255.0f,  93.0f / 255.0f, 1.0f);
const ImVec4 syntaxColorPink    = ImVec4(198.0f / 255.0f, 121.0f / 255.0f, 221.0f / 255.0f, 1.0f);
const ImVec4 syntaxColorPurple  = ImVec4(170.0f / 255.0f, 13.0f / 255.0f, 145.0f / 255.0f, 1.0f);
const ImVec4 syntaxColorCyan    = ImVec4(100.0f / 255.0f, 255.0f / 255.0f, 255.0f, 1.0f);

std::map<uint8, ImVec4> nodeColoration =
{
    { script_node_type_keyword,             syntaxColorPink },
    { script_node_type_condition,           syntaxColorBlue },
    { script_node_type_operator,            syntaxColorBlue },
    { script_node_type_function,            syntaxColorBlue },
    { script_node_type_procedure,           syntaxColorBlue },
    { script_node_type_meta_action,         syntaxColorBlue },
    { script_node_type_begin_macro,         syntaxColorRed },
    { script_node_type_begin_macro2,        syntaxColorRed },
    { script_node_type_end_macro,           syntaxColorRed },
    { script_node_type_field,               syntaxColorPurple },
    { script_node_type_dsgvarref,           syntaxColorRed },
    { script_node_type_dsgvarref2,          syntaxColorRed },
    { script_node_type_constant,            syntaxColorOrange },
    { script_node_type_real,                syntaxColorOrange },
    { script_node_type_button,              syntaxColorLightGreen },
    { script_node_type_constant_vector,     syntaxColorYellow },
    { script_node_type_vector,              syntaxColorYellow },
    { script_node_type_mask,                syntaxColorOrange },
    { script_node_type_moduleref,           syntaxColorRed },
    { script_node_type_dsgvarid,            syntaxColorRed },
    { script_node_type_string,              syntaxColorLightGreen },
    { script_node_type_lipssynchroref,      syntaxColorRed },
    { script_node_type_familyref,           syntaxColorRed },
    { script_node_type_actorref,            syntaxColorYellow },
    { script_node_type_actionref,           syntaxColorLightGreen },
    { script_node_type_superobjectref,      syntaxColorYellow },
    { script_node_type_solinksref,          syntaxColorRed },
    { script_node_type_waypointref,         syntaxColorRed },
    { script_node_type_textref,             syntaxColorRed },
    { script_node_type_behaviorref,         syntaxColorLightGreen },
    { script_node_type_moduleref2,          syntaxColorRed },
    { script_node_type_soundeventref,       syntaxColorRed },
    { script_node_type_objecttableref,      syntaxColorRed },
    { script_node_type_gamematerialref,     syntaxColorPink },
    { script_node_type_visualmaterial,      syntaxColorPink },
    { script_node_type_particlegenerator,   syntaxColorRed },
    { script_node_type_modelref,            syntaxColorYellow },
    { script_node_type_modelref2,           syntaxColorYellow },
    { script_node_type_custombits,          syntaxColorRed },
    { script_node_type_caps,                syntaxColorOrange },
    { script_node_type_graph,               syntaxColorRed },
    { script_node_type_subroutine,          syntaxColorGreen },
    { script_node_type_null,                syntaxColorRed },
    { script_node_type_cineref,             syntaxColorRed },
    { script_node_type_graphref,            syntaxColorRed },
};

struct ScriptWindow
{
    stNodeInterpret* tree = nullptr;
    bool displayNodeTree = false;
    bool displayLineNumbers = true;
    uint8 indentationSize = 4;
    uint8 indentationStyle = 0;
    unsigned pc = 0;
    
    ImVec4 nodeColor(const stNodeInterpret *node)
    {
        return node ? nodeColoration[node->type] : ImVec4(0.75,0.75,0.75,1);
    }
    
    void display()
    {
        if (tree)
        {
            currentLine = 0;
            displayNodeTree ? displayOriginalTree() : displayTranslatedTree();
        }
    }
    
private:
    unsigned int currentLine = 0;
    bool isNewLine = true;
    ImVec4 currentColor = ImVec4(0,0,0,0);
    
    void displayOriginalTree()
    {
        unsigned c = 0;
        stNodeInterpret *node = tree;
        while (1)
        {
            for (int i = 0; i < node->depth * 4; i++)
            {
                ImGui::Spacing();
                ImGui::SameLine();
            }
            
            ImVec4 color = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
            if (c == pc)
            {
                color = ImColor(0.0f, 1.0f, 0.0f, 1.0f);
                
                ImDrawList *drawlist = ImGui::GetWindowDrawList();
                ImVec2 windowPos = ImGui::GetWindowPos();
                ImVec2 windowSize = ImGui::GetWindowSize();
                
                float height = ImGui::GetTextLineHeightWithSpacing();
                ImVec2 min = ImVec2(windowPos.x, windowPos.y + (pc + 3) * height - height/4);
                ImVec2 max = ImVec2(min.x + windowSize.x, min.y + height);
                
                ImColor col = color;
                col.Value.w = 0.25f;
                drawlist->AddRectFilled(min, max, col);
            }
            
            uint32 param = (node->param);
            ImGui::TextColored(color, "%s: %d", R3NodeTypeTable[node->type], param);
            //if (c == pc) ImGui::ScrollToItem();
            if IsEndOfTree(node) break;
            node++;
            c++;
        }
    }
    
    void displayTranslatedTree()
    {
        stTreeTranslationOptions options;
        options.indentationStyle = indentationStyle;
        options.indentationSize = indentationSize;
        options.skipParentheses = 0;
        
        stTreeTranslationContext *c = NULL;
        if (!fnTreeTranslate(&c, tree, &options)) return;
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f,0.0f));
        for (unsigned int n = 0; n < c->numTokens; n++)
        {
            stTreeTranslationToken tok = c->token[n];
            currentColor = nodeColor(tok.originalNode);
            
            if (isNewLine && displayLineNumbers)
            {
                int line = currentLine + 1;
                const char* spacing = line >= 10 ? (line >= 100 ? "" : " ") : "  ";
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.25f), "%s%d   ", spacing, line);
                ImGui::SameLine();
                isNewLine = false;
            }
            
            if (tok.originalNode)
            {
                switch (tok.originalNode->type)
                {
                    case script_node_type_button: printButton(&tok); continue;
                    case script_node_type_string: printString(&tok); continue;
                    case script_node_type_actorref: printActorRef(&tok); continue;
                    case script_node_type_actionref: printActionRef(&tok); continue;
                    case script_node_type_superobjectref: printSuperObjectRef(&tok); continue;
                    case script_node_type_behaviorref: printBehaviorRef(&tok); continue;
                    case script_node_type_visualmaterial: printVisualMaterial(&tok); continue;
                    case script_node_type_modelref:
                    case script_node_type_modelref2: printModelRef(&tok); continue;
                    case script_node_type_subroutine: printSubroutine(&tok); continue;
                }
            }
            
            ImGui::TextColored(currentColor, "%s", tok.translatedText);
            
            if (tok.translatedText[0] == '\n')
            {
                ++currentLine;
                isNewLine = true;
            }
            if (tok.translatedText[0] != '\n') ImGui::SameLine();
        }
        ImGui::PopStyleVar();
    }
    
    void printButton(stTreeTranslationToken *tok)
    {
        const stInputEntryElement *button = (const stInputEntryElement*)pointer(tok->originalNode->param);
        const char* name = (const char*)pointer(button->actionName);
        const bool state = (host_byteorder_32(button->state) & 0xFF000000) == 0;
        ImVec4 color = state ? syntaxColorCyan : currentColor;
        ImGui::TextColored(color, "\"%s\"", name);
        ImGui::SameLine();
    }
    
    void printString(stTreeTranslationToken *tok)
    {
        const char* string = (const char*)pointer(tok->originalNode->param);
        ImGui::TextColored(currentColor, "\"%s\"", string);
        ImGui::SameLine();
    }
    
    void printActorRef(stTreeTranslationToken *tok)
    {
        const stEngineObject *object = (const stEngineObject*)pointer(tok->originalNode->param);
        const char* name = fnActorGetName(actor_instance_name, object, objectType);
        if (!name) name = fnActorGetName(actor_model_name, object, objectType);
        if (!name) name = fnActorGetName(actor_family_name, object, objectType);

        ImGui::TextColored(currentColor, "%s", name);
        ImGui::SameLine();
    }
    
    void printActionRef(stTreeTranslationToken *tok)
    {
        const stActionTableEntry *entry = (const stActionTableEntry*)pointer(tok->originalNode->param);
        ImGui::TextColored(currentColor, "\"%s\"", entry->name);
        ImGui::SameLine();
    }
    
    void printSuperObjectRef(stTreeTranslationToken *tok)
    {
        const stSuperObject *object = (const stSuperObject*)pointer(tok->originalNode->param);
        const char* name = fnSuperobjectGetName(object);
        ImGui::TextColored(currentColor, "%s", name);
        ImGui::SameLine();
    }
    
    void printBehaviorRef(stTreeTranslationToken *tok)
    {
        const stComport *behavior = (const stComport*)pointer(tok->originalNode->param);
        const char* shortName = strrchr(behavior->name, ':') + 1;
        ImGui::TextColored(currentColor, "\"%s\"", shortName);
        ImGui::SameLine();
    }
    
    void printVisualMaterial(stTreeTranslationToken *tok)
    {
        const stMaterialGLI *material = (const stMaterialGLI*)pointer(tok->originalNode->param);
        
        std::string name = tok->translatedText;
#if PLATFORM == platformPS2
        const stAnimatedTextureNodeGLI *animated = (const stAnimatedTextureNodeGLI*)pointer(material->firstTextureNodeAnimated);
#elif PLATFORM == platformGCN
        const stTextureGLI *animated = (const stTextureGLI*)pointer(material->firstTextureNodeAnimated);
        if (animated) name = animated->filename;
#endif
        
        //if (texture) name = texture->filename;
        ImGui::TextColored(currentColor, "VMT");
        ImGui::SameLine();
        ImGui::TextColored(syntaxColorDefault, "(");
        ImGui::SameLine();
        ImGui::TextColored(syntaxColorLightGreen, "\"%s\"", name.c_str());
        ImGui::SameLine();
        ImGui::TextColored(syntaxColorDefault, ")");
        ImGui::SameLine();
    }
    
    void printModelRef(stTreeTranslationToken *tok)
    {
        std::string name = "NULL";
        const address *nameAddress = (const address*)doublepointer(tok->originalNode->param);
        if (nameAddress)
        {
            name = (const char*)pointer(*nameAddress);
            std::string::size_type pos = name.find('\\');
            name = name.substr(0, pos);
            
        }
        ImGui::TextColored(currentColor, "%s", name.c_str());
        
        ImGui::SameLine();
    }
    
    void printSubroutine(stTreeTranslationToken *tok)
    {
        const stMacro *macro = (const stMacro*)pointer(tok->originalNode->param);
        const char* shortName = strrchr(macro->name, ':') + 1;
        ImGui::TextColored(currentColor, "%s", shortName);
        ImGui::SameLine();
        ImGui::TextColored(syntaxColorDefault, "();");
        ImGui::SameLine();
    }
    
    void printDefault(stTreeTranslationToken *tok)
    {
        ImGui::TextColored(currentColor, "%s", tok->translatedText);
    }
};

#pragma mark - Debugger

static int animat = 0;

struct Debugger
{
    void load(const stNodeInterpret *tree)
    {
        stTreeInterpretOptions options;
        fnTreeInterpreterInit(&interpreter, tree, &options);
    }
    
    void setOwner(const stEngineObject *actor)
    {
        if (interpreter) interpreter->owner = (stEngineObject*)actor;
    }
    
    void step()
    {
        if (interpreter) fnTreeInterpreterStep(interpreter);
    }
    
    long getPC()
    {
        return interpreter ? fnTreeInterpreterGetPC(interpreter) : 0;
    }
    
    stNodeInterpret *currentTree()
    {
        if (!interpreter) return nullptr;
        return (stNodeInterpret*)interpreter->frame->duplicateTree;
    }
    
    stTreeInterpretContext* interpreter = nullptr;
};

struct DebuggerWindow
{
    bool show = false;
    
    void load(const stNodeInterpret *tree)
    {
        debugger.load(tree);
        scriptWindow.displayNodeTree = true;
    }
    
    void setOwner(const stEngineObject *actor)
    {
        debugger.setOwner(actor);
    }
    
    void display()
    {
        if (show)
        {
            ImGui::Begin("Debugger", &show);
            if (ImGui::Button("Step"))
            {
                debugger.step();
            }
            ImGui::SameLine();
            if (debugger.interpreter) ImGui::Text("%s", debugger.interpreter->frame->name);
            
            scriptWindow.pc = debugger.getPC();
            scriptWindow.tree = debugger.currentTree();
            scriptWindow.display();
            ImGui::End();
            
            ImGui::Begin("Stack frames");
            if (debugger.interpreter)
            {
                for (int i = 0; i <= debugger.interpreter->currentFrameIndex; i++)
                {
                    ImGui::Text("%s", debugger.interpreter->frameStack[i]->name);
                }
            }
            ImGui::End();
        }
    }
    
    ScriptWindow scriptWindow;
    Debugger debugger;
};

DebuggerWindow debuggerWindow;
