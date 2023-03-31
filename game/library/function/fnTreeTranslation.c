//
//  fnTreeTranslation.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-29.
//

#include "stTreeInterpret.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define TokenAllocationMultiplier 256

typedef struct stTreeTranslationToken {
    char* translatedText;
    struct stTreeTranslationToken *next;
    struct stTreeTranslationToken *prev;
    struct stTreeTranslationToken *parent;
    const tdstNodeInterpret *originalNode;
    const tdstNodeInterpret *treeRoot;
} stTreeTranslationToken;

typedef struct stTreeTranslationOptions {
    uint8 indentationSize;
    uint8 indentationStyle; // 0 = Allman, 1 = K&R
    uint8 skipParentheses;
} tdstTreeTranslationOptions;

typedef struct stTreeTranslationContext {
    bool finished;
    tdstNodeInterpret *tree;
    tdstNodeInterpret *currentNode;
    stTreeTranslationToken* token;
    unsigned int numTokens, numBlocksAllocated;
    unsigned int indentation;
    tdstTreeTranslationOptions *opt;
} stTreeTranslationContext;

static const char * const translatedConditions[] = {"&&", "||", "!", "^", "==", "!=", "<", ">", "<=", ">="};

static void fnTranslationContextAppendToken(stTreeTranslationContext *const c, stTreeTranslationToken *tok) {
    if (c->numTokens + 1 > c->numBlocksAllocated * TokenAllocationMultiplier) {
        c->numBlocksAllocated++;
        c->token = (stTreeTranslationToken*)realloc(c->token, sizeof(stTreeTranslationToken) * TokenAllocationMultiplier * c->numBlocksAllocated);
    }
    c->token[c->numTokens] = *tok;
    c->numTokens++;
}

/* this needs to be void* (to avoid) stupid vsprintf warnings */
static void fnTranslationTokenAppendFormat(stTreeTranslationToken *tok, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    vsprintf(tok->translatedText, (const char*)fmt, args);
    va_end(args);
}

static const char* fnMacroGetName(tdstMacro *macro) {
    if (!macro) return NULL;
    const char* name = (const char*)memchr(macro->name, ':', 0x100);
    return name ? name + 1 : NULL;
}

static void fnEmit(stTreeTranslationContext *const c, const char* fmt, ...) {
    stTreeTranslationToken token;
    token.translatedText = (char*)malloc(2024);
    memset(token.translatedText, 0, 2024);
    token.originalNode = c->currentNode;
    
    va_list args;
    va_start(args, fmt);
    vsprintf(token.translatedText, (const char*)fmt, args);
    //printf("%s", token.translatedText);
    va_end(args);
    
    c->currentNode = NULL;
    
    fnTranslationContextAppendToken(c, &token);
}

#define IsEndOfTree(Node) (Node->type == script_node_type_end_macro)

static void fnNodeTranslate(stTreeTranslationContext *const c, tdstNodeInterpret *const node);

static void fnIndent(stTreeTranslationContext *const c) {
    fnEmit(c, "%*s", c->indentation * c->opt->indentationSize, "");
}

static void fnBranchTranslate(stTreeTranslationContext *const c, tdstNodeInterpret *const branch, bool asArguments, bool indent) {
    tdstNodeInterpret *node = branch;
    tdstNodeInterpret *root = branch - 1;
    tdstNodeInterpret *last = NULL;
    
    // Only translate branches whose depth is greater than that of the root node.
    if (asArguments && !indent && root->depth >= branch->depth) return;
    
    while (!IsEndOfTree(node) && node->depth >= branch->depth) {
        if (node->depth == branch->depth) {
            
            if (!asArguments && indent)
                fnIndent(c);
            
            fnNodeTranslate(c, node);
            
            /* Separate arguments, if any. */
            if (asArguments) fnEmit(c, ", ");
        }
        
        last = node++;
        c->currentNode = NULL;
    }
    
    /* If this was the last node part of an argument list, remove the comma. */
    if (asArguments && last && c->token[c->numTokens--].translatedText)
        free(c->token[c->numTokens].translatedText);
}

static void fnNodeTranslateChild(stTreeTranslationContext *const c, tdstNodeInterpret *node, uint8 n) {
    uint8 minimumDepth = (node++)->depth, occurrence = 0;
    while (!IsEndOfTree(node) && node->depth > minimumDepth) {
        if ((node++)->depth == minimumDepth + 1 && occurrence++ == n) {
            fnNodeTranslate(c, node - 1);
            break;
        }
    }
}

static void fnNodeTranslate(stTreeTranslationContext *const c, tdstNodeInterpret *const node)
{
    if (c->finished) return;
    
    if (node->type == script_node_type_end_macro)
    {
        c->finished = true;
        return;
    }
    
    c->currentNode = node;
    
    const tdstTreeTranslationOptions opt = *c->opt;
    const uint8 style = opt.indentationStyle;
    const uint8 paren = opt.skipParentheses;
    
    const uint8  t = node->type;
    const uint8  d = node->depth;
    const uint32 x = host_byteorder_32(node->param);

//    #define DECODE2(pattern, ...) if (pattern[0]-48 == t && pattern[1]-46 == 0) { \
//        (__VA_ARGS__); \
//    }
    
    #define first() fnNodeTranslate(c, node + 1)
    #define second() fnNodeTranslate(c, node + 2)
    #define fmt(...) (fnTranslationTokenAppendFormat(token, __VA_ARGS__))
    #define indent { fnIndent(c); c->indentation++; }
    
    #define e(...) fnEmit(c, __VA_ARGS__);
    #define b(B,arg,in) fnBranchTranslate(c, B, arg, in);
    #define c(i) fnNodeTranslateChild(c, node, i);
    
    /* First character: node type to check for */
    /* Second character: options */
    /* Third character: skip formatting? */
    /* The rest of the characters: which parameter to check */
    /* Flag bit 1: Don't check pattern */
    #define S(P,code) if (P[0]-48 == t) { if ((P[1]-32) & 1) { code; } else if (P[2+x]-32 == 1) { code; } }
    
    
    #pragma mark Keyword
    S("0 !!!!!!!!!!!!!!!!                                           ", e("%s", "if "))
    S("0                  !                                         ", e("%s", "else "))
    S("0  !                                                         ", e("%s", "!"))
    S("0 !!!!!!!!!!!!!!!!                                           ", if (!paren && x != 1) e("%s", "("))
    S("0   !!!!!!                                                   ", e("%s", "randomizer % X == 0 && "))
    S("0         !!!!!!                                             ", e("%s", "randomizer % X != 0 && "))
    S("0 !!!!!!!!!!!!!!!!                                           ", b(node + 1, false, false)) // If statement condition
    S("0 !!!!!!!!!!!!!!!!                                           ", if (!paren && x != 1) e("%s", ")"))
    S("0                 !!                                         ", if (style == 0) e("\n"); if (style == 0) indent else e(" "); e("{"); e("\n"))
    S("0                 !!                                         ", b(node + 1, false, true); --c->indentation) // Then, else
    S("0                 !!                                         ", /*e("%s", "\n");*/ indent; e("%s", "}"); if(style == 0) e("%s", "\n"); --c->indentation)
    //S ("0..                !!                                         ", "%s", "")
    
    //S ("0..                  !                                        ", "goto")
    S("0..                   !!!                                     ", e("%s", (const char*[]){"self", "mainActor", "nobody"}[x-19]))
    //S ("0..                      !!!!!!!!!!!!                         ", "null")
    
    #pragma mark Conditions
    S("1   !                                                         ", c->currentNode = NULL; e("!("); c->currentNode = node)
    S("1 !!!!!!!!!!                                                  ", fnNodeTranslateChild(c, node, 0))
    S("1 !! !!!!!!!                                                  ", e(" %s ", translatedConditions[x]))
    S("1 !! !!!!!!!                                                  ", fnNodeTranslateChild(c, node, 1))
    S("1!                                                            ", if (x > 9) e("%s", script_condition_table[x]))
    S("1!                                                            ", if (x > 9) e("%s", "("))
    S("1!                                                            ", if (x > 9) fnNodeTranslateChild(c, node, 0))
    S("1!                                                            ", if (x > 9 || x == 2) e(")"))
    
    #pragma mark Operators
    S("2     !              !                                       ", e("-"))
    S("2 !!!! !           !! !!                                     ", e("("))
    S("2 !!!! !!!!!  !!!!!!! !!!!!!!!!                              ", c( 0 ))
    S("2 !!!! !!!!!  !    !! !!    !!!                              ", e(" "))
    S("2 !                                                          ", e("+"))
    S("2  !                                                         ", e("-"))
    S("2   !                                                        ", e("*"))
    S("2    !                                                       ", e("/"))
    S("2      !                                                     ", e("%"))
    S("2       !                                                    ", e("+="))
    S("2        !                                                   ", e("-="))
    S("2         !                                                  ", e("*="))
    S("2          !                                                 ", e("/="))
    S("2           !                                                ", e("++"))
    S("2            !                                               ", e("--"))
    S("2             !                                              ", e("="))
    S("2              !                                             ", e("."))
    S("2               !                                            ", e(".X"))
    S("2                !                                           ", e(".Y"))
    S("2                 !                                          ", e(".Z"))
    S("2                  !                                         ", e("+"))
    S("2                   !                                        ", e("-"))
    S("2                     !                                      ", e("*"))
    S("2                      !                                     ", e("/"))
    S("2                       !                                    ", e(".X = "))
    S("2                        !                                   ", e(".Y = "))
    S("2                         !                                  ", e(".Z = "))
    S("2                          !                                 ", e(".") /* ultra */)
    S("2                           !                                ", e("(%s)", "Model")) // Model cast
    S("2                            !                               ", e("[%s]", ".X = ")) // Array
    S("2 !!!! !!!!!  !    !! !!    !!!                              ", e(" "))
    S("2 !!!! !!!!!  !!   !! !!!!!!!!!                              ", c( 1 ))
    S("2 !!!! !           !! !!                                     ", e(")"))
    S("2       !!!!!!!        !!!!                                  ", e(";") e("\n"))
    // BEGIN FUNCTIONS
    S("3!                                                            ", e("%s", script_function_table[x]))
    S("3!                                                            ", e("("))
    S("3!                                                            ", b(node + 1, true, false)) // Function parameters
    S("3!                                                            ", e(")"))
    // BEGIN PROCEDURES
    S("4!                                                            ", e("%s", script_procedure_table[x]))
    S("4!                                                            ", e("("))
    S("4!                                                            ", b(node + 1, true, false)) // Procedure parameters
    S("4!                                                            ", e(");") e("\n"))
    S("5!                                                            ", e("metaAction(@%X)", x)) // Meta action
    S("6!                                                            ", e("MACRO_BEGIN"))
    S("7!                                                            ", e("MACRO_BEGIN_2"))
    S("8!                                                            ", e("MACRO_END"))
    S("9!                                                            ", e("%s", script_field_table[x])) // Field
    S(":!                                                            ", e("dsgVar_%d", x)) // DsgVarRef
    S(";!                                                            ", e("dsgVar_%d", x)) // DsgVarRef2
    S("<!                                                            ", e("%d", x)) // Constant
    S("=!                                                            ", e("%g", host_byteorder_f32(*(float32*)&node->param))) // Real
    S(">!                                                            ", e("button(@%X)", x)) // Button
    S("?!                                                            ", e("Vector3") e("(") b(node+1, true, false) e(")")) // Constant vector
    S("@!                                                            ", e("Vector3") e("(") b(node+1, true, false) e(")")) // Vector
    S("A!                                                            ", e("mask(%X)", x))
    S("B!                                                            ", e("moduleRef(%X)", x))
    S("C!                                                            ", e("dsgVarID(%d)", x))
    S("D!                                                            ", e("string(%X)", x))
    S("E!                                                            ", e("lipsSynchro(@%X)", x))
    S("F!                                                            ", e("family(@%X)", x))
    S("G!                                                            ", e("engineobject(@%X)", x))
    S("H!                                                            ", e("action(@%X)", x))
    S("I!                                                            ", e("superObject(@%X)", x))
    S("J!                                                            ", e("unknown(@%X)", x))
    S("K!                                                            ", e("waypoint(@%X)", x))
    S("L!                                                            ", e("text(@%X)", x))
    S("M!                                                            ", e("behavior(@%X)", x))
    S("N!                                                            ", e("module(@%X)", x))
    S("O!                                                            ", e("soundEvent(@%X)", x))
    S("P!                                                            ", e("objectTable(@%X)", x))
    S("Q!                                                            ", e("gameMaterial(@%X)", x))
//    S("R ", )
//    S("S ", )
//    S("T ", )
//    S("U ", )
//    S("V ", )
//    S("W ", )
//    S("X ", )
    
    /* Subroutine */
    S("Y!                                                            ", e("macro(@%X)", x); e("\n"));
    
    #undef S
    
    //printf("type: %d\n", token->originalNode->type);
    //if (token->translatedText)
    //printf("token: %s\n", token.translatedText);
    
    //fnTranslationContextAppendToken(c, &token);
}

static int fnTreeTranslate(stTreeTranslationContext **const ctx, tdstNodeInterpret *const tree, tdstTreeTranslationOptions *const opt)
{
    stTreeTranslationContext *const c = *ctx = (stTreeTranslationContext*)malloc(sizeof *c);
    if (!c) return -1;
    
    c->finished = false;
    
    c->currentNode = NULL;
    c->token = NULL;
    c->numTokens = 0;
    c->numBlocksAllocated = 0;
    c->indentation = 0;
    
    if (!opt)
    {
        tdstTreeTranslationOptions defaultOptions;
        defaultOptions.indentationStyle = 0;
        defaultOptions.indentationSize = 2;
        defaultOptions.skipParentheses = 1;
        c->opt = &defaultOptions;
    }
    else
    {
        c->opt = opt;
    }
    
    //c->token = (stTreeTranslationToken*)malloc(sizeof(stTreeTranslationToken));
    //c->token->originalNode = tree;
    //c->token->translatedText = "BeginTree";
    //printf("INITIAL: %d %d\n", tree->type);
    fnBranchTranslate(c, tree, false, true);
    
//    for (int i = 0; i < c->numTokens; i++)
//    {
//        stTreeTranslationToken* tok = &c->token[i];
//        
//        printf("%s", tok->translatedText);
//    }
    
//    tdstNodeInterpret *nd = tree;
//    while (!IsEndOfTree(nd))
//    {
//        uint32 param = host_byteorder_32(nd->param);
//
//        for (int i = 0; i < (nd->depth - 1) * 4; i++) printf(" ");
//        switch (nd->type)
//        {
//            case script_node_type_keyword: printf("%s\n", script_keyword_table[param]); break;
//            case script_node_type_condition: printf("%s\n", script_condition_table[param]); break;
//            case script_node_type_operator: printf("%s\n", script_operator_table[param]); break;
//            case script_node_type_function: printf("%s\n", script_function_table[param]); break;
//            case script_node_type_procedure: printf("%s\n", script_procedure_table[param]); break;
//            default: printf("%s\n", script_nodetype_table[nd->type]);
//        }
//
//        //printf("%s\n", script_nodetype_table[nd->type]);
//
//        nd++;
//    }
    //printf("\n\n");
    
    //printf("Translated: %d\n", c->numTokens);
    //c->token->next = NULL;
    
    return 1;
}
