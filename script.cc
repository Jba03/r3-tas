#include "script.hh"
#include "constants.hh"
#include "tables.hh"

#include <any>
#include <sstream>


namespace CPA {
  namespace Script {
    
    TranslationToken::TranslationToken(std::string text, Node* originalNode) {
      this->text = text;
      this->originalNode = originalNode;
    }
    
    bool TranslationToken::operator ==(TranslationToken other) {
      return text == other.text;
    }
    
    struct TranslationContext {
      TranslationContext(pointer<Node> tree, std::function<void(TranslationContext&)> fn, TranslationEngine *e) : currentNode(tree), function(fn), engine(e) {
        /* ... */
      }
      
      TranslationEngine *engine;
      
      template <typename ... Args>
      void emit(bool condition, Args ...args) {
        if (condition) {
          //printf("afassaf\n");
          for (const auto v : { args... }) {
            std::string str = v;
            pointer<Node> *targetNode = nullptr;
            if (str.starts_with(":")) {
             // *targetNode = currentNode;
              str = str.substr(1, std::string::npos);
            }
            
            
            TranslationToken tok(str, std::string(v).starts_with(":") ? (Node*)currentNode : nullptr);
            tokens.push_back(tok);
            
            if (str == "\n") {
              for (int i = 0; i < indentationLevel; i++) {
                TranslationToken tok("  ", nullptr);
                tokens.push_back(tok);
              }
            }
            //printf("%s", v);
//            if constexpr (std::is_same<decltype(v), const char*>::value) {
//              printf("%s\n", v);
//            }
          }
        }
      }
      
      void indent(bool condition, int off) {
        if (condition) {
          indentationLevel += off;
        }
      }
      
      //template <const char* ... Args>
//      void emit(const char * ...args) {
//        for (const auto& v : { args... }) {
//          if constexpr (std::is_same<decltype(v), const char*>::value) {
//
//          }
//        }
//      }
      
      uint32_t param() { return currentNode->param; }
      operator uint32_t() { return currentNode->param; }
      bool done() { return currentNode->type == scriptNodeTypeEndMacro || currentNode->depth == 0; }
      
      pointer<Node> firstNode;
      pointer<Node> currentNode;
      int indentationLevel = 0;
      
      void child(bool condition, int child = 0) {
        if (condition) {
          pointer<Node> orig = currentNode;
          pointer<Node> node = currentNode + 1;
          uint8_t min = currentNode->depth, occ = 0;
          while (node->type != scriptNodeTypeEndMacro && node->depth > min) {
            if (node->depth == min + 1 && occ++ == child) {
              currentNode = node;
              function(*this);
              break;
            }
            node++;
          }
          currentNode = orig;
        }
      }
      
      void branch(bool args = false) {
        pointer<Node> orig = currentNode;
        pointer<Node> node = currentNode + 1;
        uint8_t depth = currentNode->depth + 1, numArgs = 0;
        while (node->type != scriptNodeTypeEndMacro && node->depth >= depth) {
          if (node->depth == depth) {
            currentNode = node;
//            if (!args)
            // indent
//              emit(<#bool condition#>, <#Args args...#>)
            function(*this);
            
            // Avoid emitting newline twice
            //if (tokens.back().text == "}") emit(true, "\n");
//            if (node->type != scriptNodeTypeOperator
//            &&  node->type != scriptNodeTypeProcedure
//            &&  node->type != scriptNodeTypeSubroutine && !args) emit(true, "\n");
            //if (tokens.back().text == "}" || (node->type != scriptNodeTypeKeyword && (uint32_t)node->param != 17)) emit(true, "\n");
            // Argument separator
            if (args) {
              numArgs++;
              emit(true, ",", " ");
            }
          }
          node++;
        }
        
        if (numArgs >= 1) {
          tokens.pop_back();
          tokens.pop_back();
        }
        
        currentNode = orig;
      }
      
      bool isEnd(pointer<Node> node) {
        return node->type == scriptNodeTypeEndMacro || node->depth == 0;
      }
      
      void seekNextDepth() {
        uint8_t depth = currentNode->depth;
        if (isEnd(currentNode++))
          return;
        while (!isEnd(currentNode) && currentNode->depth != depth)
          currentNode++;
      }
      
      void translate() {
        function(*this);
      }
      
      std::vector<TranslationToken> tokens;
    //private:
      
      std::function<void(TranslationContext&)> function;
      void _emit();
    };
    
    //std::vector<token>
    
    //    std::vector<token> collect() {
    //      std::vector<token> c;
    //      return c;
    //    }
    //
    
//        using tok = token;
//
//        void childNode(int child = 0) {
//          uint8 min = current->depth, occ = 0;
//          pointer<stNodeInterpret> node = current + 1;
//          while (node->type != scriptNodeTypeEndMacro && node->depth > min) {
//            if (node->depth == min + 1 && occ++ == child) {
//              translate(node);
//              break;
//            }
//            ++node;
//          }
//        }
    
    static void keyword(TranslationContext& s) {
      s.emit(s >= 0 && s <= 15, ":if", " ");
      s.emit(s == 1 || s == 15, "!");
      s.emit(s == 14, "#debug");
      s.emit(s == 15, "defined", "(", "U64", ")");
      s.emit(s >= 2 && s <= 13, "framerule", " ", "%", " ", std::to_string(1 << (s.param() - 1)).c_str(), " ", s <= 7 ? "==" : "!=", " ", "0");
      s.emit(s >= 2 && s <= 15, " ", "&&", " ");
      s.child(s >= 0 && s <= 15);
      s.emit(s >= 0 && s <= 15, " ");
      
      s.emit(s == 17, ":else", " ");
      s.emit(s == 16 || s == 17, "\n", "{");
      s.indent(s == 16 || s == 17, +1);
      s.emit(s == 16 || s == 17, "\n");
      if (s == 16 || s == 17) s.branch();
      if ((s == 16 || s == 17)) {
        s.indent(s == 16 || s == 17, -1);
        s.tokens.pop_back();
      }
      s.emit(s == 16 || s == 17, "}", "\n");
    }
    
    static void condition(TranslationContext& s) {
      //printf("condition: %d %X\n", s.currentNode->type, (uint32_t)s.currentNode->param);
      s.emit(s == 2, "!", "(");
      s.child(true, 0);
      s.emit(s != 2 && s <= 9, " ");
      s.emit(s == 0, ":&&");
      s.emit(s == 1, ":||");
      s.emit(s == 3, ":^");
      s.emit(s == 4, ":==");
      s.emit(s == 5, ":!=");
      s.emit(s == 6, ":<");
      s.emit(s == 7, ":>");
      s.emit(s == 8, ":<=");
      s.emit(s == 9, ":>=");
      s.emit(s != 2 && s <= 9, " ");
      s.emit(s >= 10, std::string(":" + R3ConditionTable[s.param()]).c_str(), "(");
      s.child(true, 1);
      s.emit(s == 2 || s >= 10, ")");
      //s.emit(true, " ");
    }
    
    static void _operator(TranslationContext& s) {
      s.emit(s == 4 || s == 19, "-");
      s.emit(s <= 4 || (s >= 17 && s <= 21 && s != 19), "(");
      s.emit(s == 26, "(");
      s.child(s <= 27, 0);
      s.emit(s == 0, " ", ":+", " ");
      s.emit(s == 1, " ", ":-", " ");
      s.emit(s == 2, " ", ":*", " ");
      s.emit(s == 3, " ", ":/", " ");
      s.emit(s == 5, " ", ":%%", " ");
      s.emit(s == 6, " ", ":+=", " ");
      s.emit(s == 7, " ", ":-=", " ");
      s.emit(s == 8, " ", ":*=", " ");
      s.emit(s == 9, " ", ":/=", " ");
      s.emit(s == 10, ":++", ";", "\n");
      s.emit(s == 11, ":--", ";", "\n");
      s.emit(s == 12, " ", ":=", " ");
      s.emit(s == 13, ":.");
      s.emit(s == 14, ".", ":X"); // vector x
      s.emit(s == 15, ".", ":Y"); // vector y
      s.emit(s == 16, ".", ":Z"); // vector z
      s.emit(s == 17, " ", ":+", " "); // vector + vector
      s.emit(s == 18, " ", ":-", " "); // vector - vector
      //s.emit(s == 20, <#Args args...#>)
      
      
      s.child(s <= 27 && s != 4 && s != 10 && s != 11 && !(s >= 14 && s <= 16) && s != 19, 1);
      s.emit(s == 12, ";", "\n");
    }
    
    static void function(TranslationContext& s) {
      s.emit(true, std::string(":" + R3FunctionTable[s.param()]).c_str(), "(");
      s.branch(true);
      s.emit(true, ")");
    }
    
    static void procedure(TranslationContext& s) {
      s.emit(true, std::string(":" + R3ProcedureTable[s.param()]).c_str(), "(");
      s.branch(true);
      s.emit(true, ")", ";", "\n");
    }
    
    static void metaAction(TranslationContext& s) {
      s.emit(true, std::string(":" + R3MetaActionTable[s.param()]).c_str(), "(");
      s.branch(true);
      s.emit(true, ")", ";", "\n");
    }
    
    static void dsgvar(TranslationContext& s) {
      s.emit(true, (std::string(":DsgVar_") + std::to_string(s.param())).c_str());
    }
    
    static void constant(TranslationContext& s) {
      s.emit(true, (":" + std::to_string((int32_t)s.param())).c_str());
    }
    
    static void real(TranslationContext& s) {
      s.emit(true, std::to_string(*(float*)&s.currentNode->param).c_str());
    }
    
    static void button(TranslationContext& s) {
      // s.emit(true, "", )
    }
    
    static void _string(TranslationContext& s) {
      const char *str = pointer<string<>>(s.param());
      s.emit(true, "\"", (":" + std::string(str)).c_str(), "\"");
    }
    
    static void subroutine(TranslationContext& s) {
      pointer<Structure::stMacro> macro = pointer<Structure::stMacro>(s.param());
      if (s.engine->options.expandMacroReferences) {
        TranslationEngine t(s.engine->options);
        TranslationResult *result = t.translate(nullptr, macro->currentTree->node);
        
        for (TranslationToken& tok : result->tokens) {
          s.tokens.push_back(tok);
        }
      } else {
        s.emit(true, std::string(":" + macro->name.lastPathComponent()).c_str(), "(", ")", ";", "\n");
      }
    }
    
    static void reference(TranslationContext& s) {
      s.emit(true, ":" + std::to_string(s.param()));
    }
    
    static std::map<int, std::function<void(TranslationContext& s)>> TranslationTable {
      { scriptNodeTypeKeyword, &keyword },
      { scriptNodeTypeCondition, &condition },
      { scriptNodeTypeOperator, &_operator },
      { scriptNodeTypeFunction, &function },
      { scriptNodeTypeProcedure, &procedure },
      { scriptNodeTypeMetaAction, &metaAction },
      { scriptNodeTypeBeginMacro, nullptr },
      { scriptNodeTypeBeginMacro2, nullptr },
      { scriptNodeTypeEndMacro, nullptr },
      { scriptNodeTypeField, nullptr },
      { scriptNodeTypeDsgVarRef, &dsgvar },
      { scriptNodeTypeDsgVarRef2, &dsgvar },
      { scriptNodeTypeConstant, &constant },
      { scriptNodeTypeReal, &real },
      { scriptNodeTypeButton, nullptr },
      { scriptNodeTypeConstantVector, nullptr },
      { scriptNodeTypeVector, nullptr },
      { scriptNodeTypeMask, nullptr },
      { scriptNodeTypeModuleRef, nullptr },
      { scriptNodeTypeDsgVarID, nullptr },
      { scriptNodeTypeString, &_string },
      { scriptNodeTypeLipsSynchroRef, &reference },
      { scriptNodeTypeFamilyRef, &reference },
      { scriptNodeTypeActorRef, &reference},
      { scriptNodeTypeActionRef, &reference },
      { scriptNodeTypeSuperObjectRef, &reference },
      { scriptNodeTypeSOLinksRef, &reference },
      { scriptNodeTypeWaypointRef, &reference },
      { scriptNodeTypeTextRef, &reference },
      { scriptNodeTypeBehaviorRef, &reference },
      { scriptNodeTypeModuleRef2, &reference },
      { scriptNodeTypeSoundEventRef, &reference },
      { scriptNodeTypeObjectTableRef, &reference },
      { scriptNodeTypeGameMaterialRef, &reference },
      { scriptNodeTypeVisualMaterial, nullptr },
      { scriptNodeTypeParticleGenerator, nullptr },
      { scriptNodeTypeModelRef, nullptr },
      { scriptNodeTypeModelRef2, nullptr },
      { scriptNodeTypeCustomBits, nullptr },
      { scriptNodeTypeCaps, nullptr },
      { scriptNodeTypeGraph, nullptr },
      { scriptNodeTypeSubroutine, &subroutine },
      { scriptNodeTypeNULL, nullptr },
      { scriptNodeTypeCineRef, nullptr },
      { scriptNodeTypeGraphRef, nullptr },
    };
    
    static void NodeTranslate(TranslationContext& s) {
      //printf("node: %d %d %d\n", s.currentNode->type, (uint32_t)s.currentNode->param, s.currentNode->depth);
      pointer<Node> node = s.currentNode;
      if (TranslationTable.find(node->type) != TranslationTable.end()) {
        if (TranslationTable[node->type] != nullptr)
          TranslationTable[node->type](s);
      }
      s.seekNextDepth();
    }
    
    TranslationEngine::TranslationEngine(TranslationOptions opt) : options(opt) {
      /* ... */
    }
    
    TranslationResult *TranslationEngine::translate(pointer<Structure::stSuperObject> actor, pointer<Node> tree) {
      initialNode = tree;
      TranslationContext s(tree, NodeTranslate, this);
      while (!s.done())
        s.translate();
      
      TranslationResult *result = new TranslationResult(TranslationMode::TreeToSource);
      result->tokens = s.tokens;
      return result;
    }
    
    TranslationResult *TranslationEngine::translate(pointer<Structure::stSuperObject> actor, std::string source) {
      TranslationResult *result = new TranslationResult(TranslationMode::SourceToTree);
      return result;
    }
    
  };
};
