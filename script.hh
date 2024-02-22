#ifndef script_hh
#define script_hh

#include "structure.hh"
#include <vector>

namespace CPA {
  namespace Script {
    
    using Node = CPA::Structure::stNodeInterpret;
    
    struct Tokenizer {
     
    };
    
    struct TranslationToken {
      //yTranslationToken(const char* text, pointer<Node> originalNode = nullptr);
      TranslationToken(std::string text, pointer<Node> originalNode = nullptr);
      /// Mode: Tree -> Source
      ///   Text string of the generated token
      std::string text;
      /// Mode: Tree -> Source
      ///   A description of the token symbol
      std::string description;
      /// Mode: Tree -> Source
      ///   The original script node from which this token was generated
      pointer<Node> originalNode = nullptr;
      /// Mode: Source -> Tree
      ///   Output node
      Node node;
      /// equals
      bool operator ==(TranslationToken other);
    };
    
    namespace TokenConstants {
      static TranslationToken Space = TranslationToken("#[space]"); /* ' ' */
      static TranslationToken NewLine = TranslationToken("#[newline]"); /* '\n' */
      static TranslationToken Indentation = TranslationToken("#[indentation=]"); /* '!' */
      
      static TranslationToken IfDef = TranslationToken("#[ifdef]"); /* 'defined' */
      
      static TranslationToken LeftParen = TranslationToken("#[lpar]"); /* '(' */
      static TranslationToken RightParen = TranslationToken("#[rpar]"); /* ')' */
      static TranslationToken Negation = TranslationToken("#[negation]"); /* '!' */
      static TranslationToken Eq = TranslationToken("#[equals]"); /* '==' */
      static TranslationToken NEq = TranslationToken("#[not-equals]"); /* '!=' */
      static TranslationToken And = TranslationToken("#[and]"); /* '&&' */
      static TranslationToken Or = TranslationToken("#[or]"); /* '||' */
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
    private:
      TranslationMode mode;
      pointer<Node> initialNode;
      pointer<Node> currentNode;
      
      
    };
    
  };
};

#endif /* script_hh */
