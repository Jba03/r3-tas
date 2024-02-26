#ifndef script_hh
#define script_hh

#include "structure.hh"
#include <vector>

namespace CPA {
  namespace Script {
    
    using Node = CPA::Structure::stNodeInterpret;
    
    enum ScriptNodeType : uint8_t {
      Keyword           = 0,
      Condition         = 1,
      Operator          = 2,
      Function          = 3,
      Procedure         = 4,
      MetaAction        = 5,
      BeginMacro        = 6,
      BeginMacro2       = 7,
      EndMacro          = 8,
      Field             = 9,
      DsgVarRef         = 10,
      DsgVarRef2        = 11,
      Constant          = 12,
      Real              = 13,
      Button            = 14,
      ConstantVector    = 15,
      Vector            = 16,
      Mask              = 17,
      ModuleRef         = 18,
      DsgVarID          = 19,
      String            = 20,
      LipsSynchroRef    = 21,
      FamilyRef         = 22,
      ActorRef          = 23,
      ActionRef         = 24,
      SuperObjectRef    = 25,
      SOLinksRef        = 26, // ?
      WaypointRef       = 27,
      TextRef           = 28,
      BehaviorRef       = 29,
      ModuleRef2        = 30,
      SoundEventRef     = 31,
      ObjectTableRef    = 32,
      GameMaterialRef   = 33,
      VisualMaterial    = 34,
      ParticleGenerator = 35,
      ModelRef          = 36,
      ModelRef2         = 37,
      CustomBits        = 38,
      Caps              = 39,
      Graph             = 40, // ?
      Subroutine        = 41,
      Null              = 42,
      CineRef           = 43, // ?
      GraphRef          = 44,
    };
    
    struct TranslationToken {
      // TranslationToken(const char* text, pointer<Node> originalNode = nullptr);
      TranslationToken(std::string text, Node *originalNode = nullptr);
      /// Mode: Tree -> Source
      ///   Text string of the generated token
      std::string text;
      /// Mode: Tree -> Source
      ///   A description of the token symbol
      std::string description;
      /// Mode: Tree -> Source
      ///   The original script node from which this token was generated
      Node *originalNode = nullptr;
      /// Mode: Source -> Tree
      ///   Output node
      Node node;
      /// equals
      bool operator ==(TranslationToken other);
    };
    
    enum TranslationMode {
      TreeToSource,
      SourceToTree,
    };
    
    struct TranslationOptions {
      /// Mode: Tree -> Source
      ///   Remove parentheses which do not affect the control flow of the program.
      bool removeUnnecessaryParentheses;
      /// Mode: Tree -> Source
      ///   Replace macrorefs with their respective source trees
      bool expandMacroReferences;
      
      std::vector<std::string> conditionTable;
      std::vector<std::string> functionTable;
      std::vector<std::string> procedureTable;
      std::vector<std::string> metaActionTable;
      std::vector<std::string> fieldTable;
    };
    
    enum TranslationTokenType {
      Default,
      Space,
    };
    
    struct TranslationResult {
      TranslationResult(TranslationMode mode) : mode(mode) {};
      /// @`TranslationMode`
      TranslationMode mode;
      /// Options used in the translation
      TranslationOptions options;
      /// Token list
      std::vector<TranslationToken> tokens;
    };
    
    /// Context for script translation
    struct TranslationEngine {
      TranslationEngine(TranslationOptions opt);
      TranslationResult *translate(pointer<Structure::stSuperObject> actor, pointer<Node> node);
      TranslationResult *translate(pointer<Structure::stSuperObject> actor, std::string source);
      TranslationOptions options;
      
      TranslationMode mode;
      pointer<Node> initialNode;
      pointer<Node> currentNode;
    };
    
    void testLexer(std::string source);
    
  };
};

#endif /* script_hh */
