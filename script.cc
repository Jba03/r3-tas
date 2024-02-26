#include "script.hh"

#include <any>
#include <sstream>
#include <format>

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
      bool done() { return currentNode->type == ScriptNodeType::EndMacro || currentNode->depth == 0; }
      
      pointer<Node> firstNode;
      pointer<Node> currentNode;
      int indentationLevel = 0;
      
      void child(bool condition, int child = 0) {
        if (condition) {
          pointer<Node> orig = currentNode;
          pointer<Node> node = currentNode + 1;
          uint8_t min = currentNode->depth, occ = 0;
          while (node->type != ScriptNodeType::EndMacro && node->depth > min) {
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
        while (node->type != ScriptNodeType::EndMacro && node->depth >= depth) {
          if (node->depth == depth) {
            currentNode = node;
            function(*this);
            
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
        return node->type == EndMacro || node->depth == 0;
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
      std::function<void(TranslationContext&)> function;
      
      std::vector<std::string> conditionTable;
      std::vector<std::string> functionTable;
      std::vector<std::string> procedureTable;
      std::vector<std::string> fieldTable;
      std::vector<std::string> metaActionTable;
    };
    
    static void keyword(TranslationContext& s) {
      s.emit(s >= 0 && s <= 15, ":if", " ");
      s.emit(s == 1 || s == 15, "!", "(");
      s.emit(s == 14, "#debug");
      s.emit(s == 15, "defined", "(", "U64", ")");
      s.emit(s >= 2 && s <= 13, "framerule", " ", "%", " ", std::to_string(1 << (s.param() - 1)).c_str(), " ", s <= 7 ? "==" : "!=", " ", "0");
      s.emit(s >= 2 && s <= 15, " ", "&&", " ");
      s.child(s >= 0 && s <= 15);
      //s.emit(s >= 0 && s <= 15, " ");
      s.emit(s == 1 || s == 15, ")");
      
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
      
      s.emit(s == 19, "self");
      s.emit(s == 20, "MainActor");
    }
    
    static void condition(TranslationContext& s) {
      //printf("condition: %d %X\n", s.currentNode->type, (uint32_t)s.currentNode->param);
      s.emit(s == 2, "!", "(");
      s.child(s <= 9, 0);
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
      s.emit(s >= 10, std::string(":" + s.conditionTable[s.param()]).c_str(), "(");
      s <= 9 ? s.child(true, 1) : s.branch(true);
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
      s.emit(s == 20, " ", ":*", " "); // vector * scalar
      s.emit(s == 21, " ", ":/", " "); // vector / scalar
      s.emit(s == 22, ".", ":X", " ", "="); // vector.x = s
      s.emit(s == 23, ".", ":Y", " ", "="); // vector.x = s
      s.emit(s == 24, ".", ":Z", " ", "="); // vector.x = s
      s.emit(s == 25, "."); // 'ultra'
      s.emit(s == 26, ")", "("); // modelcast
      s.emit(s == 27, "["); // array access
      
      s.child(s <= 27 && s != 4 && s != 10 && s != 11 && !(s >= 14 && s <= 16) && s != 19, 1);
      s.emit(s == 26, ")", ")"); // modelcast
      s.emit(s == 27, "]"); // array access
      s.emit(s <= 4 || (s >= 17 && s <= 21 && s != 19), ")");
      s.emit(s == 12 || (s >= 6 && s <= 9) || (s >= 22 && s <= 23), ";", "\n");
    }
    
    static void function(TranslationContext& s) {
      s.emit(true, std::string(":" + s.functionTable[s.param()]).c_str(), "(");
      s.branch(true);
      s.emit(true, ")");
    }
    
    static void procedure(TranslationContext& s) {
      s.emit(true, std::string(":" + s.procedureTable[s.param()]).c_str(), "(");
      s.branch(true);
      s.emit(true, ")", ";", "\n");
    }
    
    static void metaAction(TranslationContext& s) {
      s.emit(true, std::string(":" + s.metaActionTable[s.param()]).c_str(), "(");
      s.branch(true);
      s.emit(true, ")", ";", "\n");
    }
    
    static void field(TranslationContext& s) {
      s.emit(true, (":" + s.fieldTable[s.param()]).c_str());
    }
    
    static void dsgvar(TranslationContext& s) {
      s.emit(true, (std::string(":DsgVar_") + std::to_string(s.param())).c_str());
    }
    
    static void constant(TranslationContext& s) {
      s.emit(true, (":" + std::to_string((int32_t)s.param())).c_str());
    }
    
    static void real(TranslationContext& s) {
      uint32_t p = s.param();
      char buf[64];
      std::sprintf(buf, ":%.5gf", *(float*)&p);
      
      s.emit(true, std::string(buf).c_str());
    }
    
    static void vector(TranslationContext& s) {
      s.emit(true, ":Vector", "(");
      s.branch(true);
      s.emit(true, ")");
    }
    
    static void button(TranslationContext& s) {
      // s.emit(true, "", )
    }
    
    static void _string(TranslationContext& s) {
      const char *str = pointer<string<>>(s.param());
      s.emit(true, "\"", (":" + std::string(str)).c_str(), "\"");
    }
    
    static void reference(TranslationContext& s) {
      s.emit(true, ":" + std::to_string(s.param()));
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
    
    static void null(TranslationContext& s) {
      s.emit(true, ":NULL");
    }
    
    static std::map<int, std::function<void(TranslationContext& s)>> TranslationTable {
      { ScriptNodeType::Keyword, &keyword },
      { ScriptNodeType::Condition, &condition },
      { ScriptNodeType::Operator, &_operator },
      { ScriptNodeType::Function, &function },
      { ScriptNodeType::Procedure, &procedure },
      { ScriptNodeType::MetaAction, &metaAction },
      { ScriptNodeType::BeginMacro, nullptr },
      { ScriptNodeType::BeginMacro2, nullptr },
      { ScriptNodeType::EndMacro, nullptr },
      { ScriptNodeType::Field, &field },
      { ScriptNodeType::DsgVarRef, &dsgvar },
      { ScriptNodeType::DsgVarRef2, &dsgvar },
      { ScriptNodeType::Constant, &constant },
      { ScriptNodeType::Real, &real },
      { ScriptNodeType::Button, nullptr },
      { ScriptNodeType::ConstantVector, &vector },
      { ScriptNodeType::Vector, &vector },
      { ScriptNodeType::Mask, nullptr },
      { ScriptNodeType::ModuleRef, nullptr },
      { ScriptNodeType::DsgVarID, nullptr },
      { ScriptNodeType::String, &_string },
      { ScriptNodeType::LipsSynchroRef, &reference },
      { ScriptNodeType::FamilyRef, &reference },
      { ScriptNodeType::ActorRef, &reference},
      { ScriptNodeType::ActionRef, &reference },
      { ScriptNodeType::SuperObjectRef, &reference },
      { ScriptNodeType::SOLinksRef, &reference },
      { ScriptNodeType::WaypointRef, &reference },
      { ScriptNodeType::TextRef, &reference },
      { ScriptNodeType::BehaviorRef, &reference },
      { ScriptNodeType::ModuleRef2, &reference },
      { ScriptNodeType::SoundEventRef, &reference },
      { ScriptNodeType::ObjectTableRef, &reference },
      { ScriptNodeType::GameMaterialRef, &reference },
      { ScriptNodeType::VisualMaterial, nullptr },
      { ScriptNodeType::ParticleGenerator, nullptr },
      { ScriptNodeType::ModelRef, nullptr },
      { ScriptNodeType::ModelRef2, nullptr },
      { ScriptNodeType::CustomBits, nullptr },
      { ScriptNodeType::Caps, nullptr },
      { ScriptNodeType::Graph, nullptr },
      { ScriptNodeType::Subroutine, &subroutine },
      { ScriptNodeType::Null, &null },
      { ScriptNodeType::CineRef, nullptr },
      { ScriptNodeType::GraphRef, nullptr },
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
      s.conditionTable = options.conditionTable;
      s.functionTable = options.functionTable;
      s.procedureTable = options.procedureTable;
      s.metaActionTable = options.metaActionTable;
      s.fieldTable = options.fieldTable;
    
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
    
#pragma mark - Source to Tree
    
    struct Lexer {
      struct Token {
        enum class Type {
          None,
          LeftParen,
          RightParen,
          LeftCurlyBracket,
          RightCurlyBracket,
          LeftSquareBracket,
          RightSquareBracket,
          Identifier,
          Number,
          Comma,
          Colon,
          SemiColon,
          Plus,
          Minus,
          Asterisk,
          Slash,
          ExclamationMark,
          DoubleQuote,
          SingleQuote,
          NumberSign,
          Modulo,
          Ampersand,
          Dot,
          Equal,
          LessThan,
          GreaterThan,
          Comment,
          Invalid,
          End,
        };
        
        Token() {}
        Token(Type t, const char *start, const char *end) {
          type = t;
          text = std::string(start).substr(0, std::distance(start, end));
        }
        
        Type type;
        std::string text;
      };
      
      Lexer(const char* source) : current(source) {
        /* ... */
      }
      
      bool is_space(char c) { return (c == ' ') || (c == '\n') || (c == '\t') || (c == '\r'); }
      bool is_alpha(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_'); };
      bool is_digit(char c) { return (c >= '0') && (c <= '9'); }
      bool is_alphanumeric(char c) { return is_alpha(c) || is_digit(c); };
      
      Token number() {
        const char *start = current;
        while (is_digit(*current) || *current == '.') current++;
        return Token(Token::Type::Number, start, current);
      }
      
      Token identifier() {
        const char *start = current;
        while (is_alphanumeric(*current)) {
          current++;
        }
        //current--;
        return Token(Token::Type::Identifier, start, current);
      }
      
      Token lex() {
        while ((is_space(*current))) current++;
        
        Token t = Token(Token::Type::None, current, current + 1);
        if (*current == '\0') t = Token(Token::Type::End, current, current + 1);
        else if (*current == '(') t = Token(Token::Type::LeftParen, current, current + 1);
        else if (*current == ')') t = Token(Token::Type::RightParen, current, current + 1);
        else if (*current == '[') t = Token(Token::Type::LeftSquareBracket, current, current + 1);
        else if (*current == ']') t = Token(Token::Type::RightSquareBracket, current, current + 1);
        else if (*current == '{') t = Token(Token::Type::LeftCurlyBracket, current, current + 1);
        else if (*current == '}') t = Token(Token::Type::RightCurlyBracket, current, current + 1);
        else if (*current == '+') t = Token(Token::Type::Plus, current, current + 1);
        else if (*current == '-') t = Token(Token::Type::Minus, current, current + 1);
        else if (*current == '*') t = Token(Token::Type::Asterisk, current, current + 1);
        else if (*current == '/') t = Token(Token::Type::Slash, current, current + 1); // also comment
        else if (*current == '%') t = Token(Token::Type::Modulo, current, current + 1);
        else if (*current == '#') t = Token(Token::Type::NumberSign, current, current + 1);
        else if (*current == '=') t = Token(Token::Type::Equal, current, current + 1);
        else if (*current == '<') t = Token(Token::Type::LessThan, current, current + 1);
        else if (*current == '>') t = Token(Token::Type::GreaterThan, current, current + 1);
        else if (*current == ':') t = Token(Token::Type::Colon, current, current + 1);
        else if (*current == ';') t = Token(Token::Type::SemiColon, current, current + 1);
        else if (*current == '\'') t = Token(Token::Type::SingleQuote, current, current + 1);
        else if (*current == '\"') t = Token(Token::Type::DoubleQuote, current, current + 1);
        else if (is_alpha(*current)) t = identifier();
        else if (is_digit(*current)) t = number();
        
        current++;
        
        return t;
      }
      
      const char *current;
    };
    
    std::map<Lexer::Token::Type, std::string> TokenNames {
      { Lexer::Token::Type::LeftParen, "LeftParen" },
      { Lexer::Token::Type::RightParen, "RightParen" },
      { Lexer::Token::Type::LeftCurlyBracket, "LeftCurlyBracket" },
      { Lexer::Token::Type::RightCurlyBracket, "RightCurlyBracket" },
      { Lexer::Token::Type::LeftSquareBracket, "LeftSquareBracket" },
      { Lexer::Token::Type::RightSquareBracket, "RightSquareBracket" },
      { Lexer::Token::Type::Identifier, "Identifier" },
      { Lexer::Token::Type::Number, "Number" },
      { Lexer::Token::Type::Comma, "Comma" },
      { Lexer::Token::Type::Colon, "Colon" },
      { Lexer::Token::Type::SemiColon, "SemiColon" },
      { Lexer::Token::Type::Plus, "Plus" },
      { Lexer::Token::Type::Minus, "Minus" },
      { Lexer::Token::Type::Asterisk, "Asterisk" },
      { Lexer::Token::Type::Slash, "Slash" },
      { Lexer::Token::Type::ExclamationMark, "ExclamationMark" },
      { Lexer::Token::Type::DoubleQuote, "DoubleQuote" },
      { Lexer::Token::Type::SingleQuote, "SingleQuote" },
      { Lexer::Token::Type::NumberSign, "NumberSign" },
      { Lexer::Token::Type::Modulo, "Modulo" },
      { Lexer::Token::Type::Ampersand, "Ampersand" },
      { Lexer::Token::Type::Equal, "Equal" },
      { Lexer::Token::Type::LessThan, "LessThan" },
      { Lexer::Token::Type::GreaterThan, "GreaterThan" },
      { Lexer::Token::Type::Dot, "Dot" },
      { Lexer::Token::Type::Comment, "Comment" },
      { Lexer::Token::Type::Invalid, "Invalid" },
      { Lexer::Token::Type::End, "End" },
    };
    
    void testLexer(std::string source) {
      Lexer lexer(source.c_str());
      Lexer::Token t = lexer.lex();
      while (t.type != Lexer::Token::Type::End && t.type != Lexer::Token::Type::Invalid) {
        printf("%s |%s|\n", TokenNames[t.type].c_str(), t.text.c_str());
        t = lexer.lex();
      }
    }
    
  };
};
