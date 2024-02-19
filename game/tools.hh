////
////  utility.h
////  r3-tas-memorymap
////
////  Created by Jba03 on 2024-01-13.
////
//
//#ifndef utility_h
//#define utility_h
//
//#include "tables.hh"
//#include "structure.hh"
//
//#pragma mark - Script Translation Context
//
//struct ScriptTranslationContext {
//
//  ScriptTranslationContext() {
//
//  }
//
//#if platform == GCN
//  std::vector<std::string> ConditionTable = R3ConditionTable;
//#endif
//
//  struct token {
//    token() {}
//    token(std::string txt, pointer<stNodeInterpret> ref = nullptr) {
//      text = txt;
//      node = ref;
//    }
//    std::string text;
//    pointer<stNodeInterpret> node;
//
//    static inline const std::pair<std::string, pointer<stNodeInterpret>> non(std::string t) {
//      return {t, nullptr};
//    }
//
//    static inline const std::pair<std::string, pointer<stNodeInterpret>> space {" ", nullptr};
//    static inline const std::pair<std::string, pointer<stNodeInterpret>> exclamation {"!", nullptr};
//  };
//
//  //token& current;
//  pointer<stNodeInterpret> current;
//
//  template <typename... Args> void emit(bool condition, const Args... args) {
//    if (condition) {
//      for (const auto& v : { args... }) {
//        if (std::is_same_v<decltype(v), std::string>) {
//          token t(v, 0);
//        }
//      }
//    }
//  }
//
//  //std::vector<token>
//
////    std::vector<token> collect() {
////      std::vector<token> c;
////      return c;
////    }
////
//  using tok = token;
//
//  void childNode(int child = 0) {
//    uint8 min = current->depth, occ = 0;
//    pointer<stNodeInterpret> node = current + 1;
//    while (node->type != scriptNodeTypeEndMacro && node->depth > min) {
//      if (node->depth == min + 1 && occ++ == child) {
//        translate(node);
//        break;
//      }
//      ++node;
//    }
//  }
//
//  void keyword() {
////      emit(s == 15,           "if", tok::space);
////      emit(s >= 2 && s <= 7,  "if", tok::space, "framerule", tok::space, "%", tok::space, "==", tok::space);
////      emit(s >= 8 && s <= 13, "if", tok::space, "" "framerule", tok::space, "%", tok::space, "==", tok::space);
//  }
//
//
//#define S tok::space
//#define t0() childNode(0)
//#define t1() childNode(1)
//
////    void condition(unsigned s) {
////      std::string cond = ConditionTable[s];
////      emit(s == 0, t0(), S, "&&", S, t1()); /* And */
////      emit(s == 1, t0(), s, "||", S, t1()); /* Or  */
////      emit(s == 2, "!",  "(",         t0(),             ")"); /* Not */
////      emit(s == 3, t0(), S,  "^", S, t1()); /* XOr */
////      emit(s == 4, t0(), S, "==", S, t1()); /* Eq  */
////      emit(s == 5, t0(), S, "!=", S, t1()); /* NEq */
////      emit(s == 6, t0(), S,  "<", S, t1()); /* Lt  */
////      emit(s == 7, t0(), S,  ">", S, t1()); /* Gt  */
////      emit(s == 8, t0(), S, "<=", S, t1()); /* Le  */
////      emit(s == 9, t0(), S, ">=", S, t1()); /* Ge  */
////
////      emit(s > 9,  "if", tok::space);
////    }
//
//
//    void _operator(unsigned s) {
//      emit(s == 0 , "(", t0(), token::space, "+", token::space, ")");
//      emit(s == 1 , "(", t1(), token::space, "-", token::space, ")");
//      emit(s == 2 , "(", t1(), token::space, "*", token::space, ")");
//      emit(s == 3 , "(", t1(), token::space, "/", token::space, ")");
//      emit(s == 2);
//      emit(s == 3);
//      emit(s == 4);
//      emit(s == 5);
//      emit(s == 6);
//      emit(s == 7);
//      emit(s == 8);
//      emit(s == 9);
//      emit(s == 10);
//      emit(s == 11);
//      emit(s == 12);
//      emit(s == 13);
//      emit(s == 14);
//      emit(s == 15);
//      emit(s == 16);
//      emit(s == 17);
//      emit(s == 18);
//      emit(s == 19);
//      emit(s == 20);
//      emit(s == 21);
//      emit(s == 22);
//      emit(s == 23);
//      emit(s == 24);
//      emit(s == 25);
//      emit(s == 26);
//      emit(s == 27);
//    }
//
//  void endMacro() {
//    /* ... */
//  }
//
//  void translate(pointer<stNodeInterpret> node) {
//    switch (node->type) {
//      case scriptNodeTypeKeyword: return keyword();
//      case scriptNodeTypeCondition: return condition();
//      case scriptNodeTypeOperator: return _operator();
//      case scriptNodeTypeFunction: return function();
//      case scriptNodeTypeProcedure: return procedure();
//      case scriptNodeTypeMetaAction: return metaAction();
//      case scriptNodeTypeBeginMacro: return beginMacro();
//      case scriptNodeTypeBeginMacro2: return beginMacro();
//      case scriptNodeTypeEndMacro: return endMacro();
//      case scriptNodeTypeField: return field();
//      case scriptNodeTypeDsgVarRef:
//      case scriptNodeTypeDsgVarRef2:
//      case scriptNodeTypeConstant:
//      case scriptNodeTypeReal:
//      case scriptNodeTypeButton:
//      case scriptNodeTypeConstantVector:
//      case scriptNodeTypeVector:
//      case scriptNodeTypeMask:
//      case scriptNodeTypeModuleRef:
//      case scriptNodeTypeDsgVarID:
//      case scriptNodeTypeString:
//      case scriptNodeTypeLipsSynchroRef:
//      case scriptNodeTypeFamilyRef:
//      case scriptNodeTypeActorRef:
//      case scriptNodeTypeActionRef:
//      case scriptNodeTypeSuperObjectRef:
//      case scriptNodeTypeSOLinksRef:
//      case scriptNodeTypeWaypointRef:
//      case scriptNodeTypeTextRef:
//      case scriptNodeTypeBehaviorRef:
//      case scriptNodeTypeModuleRef2:
//      case scriptNodeTypeSoundEventRef:
//      case scriptNodeTypeObjectTableRef:
//      case scriptNodeTypeGameMaterialRef:
//      case scriptNodeTypeVisualMaterial:
//      case scriptNodeTypeParticleGenerator:
//      case scriptNodeTypeModelRef:
//      case scriptNodeTypeModelRef2:
//      case scriptNodeTypeCustomBits:
//      case scriptNodeTypeCaps:
//      case scriptNodeTypeGraph:
//      case scriptNodeTypeSubroutine:
//      case scriptNodeTypeNULL:
//      case scriptNodeTypeCineRef:
//      case scriptNodeTypeGraphRef:
//    }
//  }
////
////    std::vector<token> gameToText(stTreeInterpret *tree) {
////      std::vector<token> tokenList;
////      stNodeInterpret *firstNode = tree->node;
////      stNodeInterpret *currentNode = tree->node;
////
////      auto isEndOfTree = [currentNode] {
////        return currentNode->type != scriptNodeTypeEndMacro;
////      };
////
////      auto seekSameDepth = [&currentNode, isEndOfTree]() {
////        uint8 depth = currentNode->depth;
////        while (currentNode->depth != depth && !isEndOfTree()) currentNode++;
////      };
////
////      auto emit = [](std::string s) {
////
////      };
////
////      auto translateKeyword = [] {
////
////      };
////
////
////
////
////
////      return tokenList;
////    }
//};
//
//struct scriptInjection {
//
//};
//
//
//#endif /* utility_h */
