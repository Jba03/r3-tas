//
//  fnTreeTranslation.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-29.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "memory.hh"
#include "tables.hh"

#include "stTreeInterpret.hh"

#define IsEndOfTree(Node) (Node->type == script_node_type_end_macro)

static const char * const conditions[] =
{
    "&&", "||", "!", "^", "==", "!=", "<", ">", "<=", ">="
};

static const char * const operators[] =
{
    "+", "-", "*", "/", "-", "%", "+=", "-=", "*=", "/=", "++", "--", "=", ".",
    ".X", ".Y", ".Z", "+", "-", "-", "*", "/", ".X = ", ".Y = ", ".Z = ", "."
};

static void fnNodeTranslate(stTreeTranslationContext *const c, stNodeInterpret *const node);

static void fnTranslationContextAppendToken(stTreeTranslationContext *const c, stTreeTranslationToken *tok)
{
    if (c->numTokens + 1 > c->numBlocksAllocated * TRANSLATION_TOKEN_ALLOCATION_MULTIPLIER)
    {
        c->numBlocksAllocated++;
        c->token = (stTreeTranslationToken*)realloc(c->token, sizeof(stTreeTranslationToken) * TRANSLATION_TOKEN_ALLOCATION_MULTIPLIER * c->numBlocksAllocated);
    }
    c->token[c->numTokens] = *tok;
    c->numTokens++;
}

static void fnEmit(stTreeTranslationContext *const c, const char* fmt, ...)
{
    stTreeTranslationToken token;
    memset(token.translatedText, 0, TRANSLATION_TOKEN_MAX_STRING_LENGTH);
    token.originalNode = c->currentNode;
    
    va_list args;
    va_start(args, fmt);
    vsnprintf(token.translatedText, TRANSLATION_TOKEN_MAX_STRING_LENGTH, fmt, args);
    va_end(args);
    
    c->currentNode = NULL;
    fnTranslationContextAppendToken(c, &token);
}

static void fnIndent(stTreeTranslationContext *const c)
{
    fnEmit(c, "%*s", c->indentation * c->opt->indentationSize, "");
}

static void fnNodeTranslateBranch(stTreeTranslationContext *const c, stNodeInterpret *branch, bool asArguments, bool indent)
{
    stNodeInterpret *node = branch;
    stNodeInterpret *root = branch - 1;
    stNodeInterpret *last = NULL;
    
    /* Only translate branches whose depth is greater than that of the root node. */
    if (asArguments && !indent && root->depth >= branch->depth) return;
    
    while (!IsEndOfTree(node) && node->depth >= branch->depth)
    {
        if (node->depth == branch->depth)
        {
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
    if (asArguments && last) --c->numTokens;
}

static void fnNodeTranslateChild(stTreeTranslationContext *const c, stNodeInterpret *node, uint8 n)
{
    uint8 minimumDepth = (node++)->depth, occurrence = 0;
    while (!IsEndOfTree(node) && node->depth > minimumDepth)
    {
        if ((node++)->depth == minimumDepth + 1 && occurrence++ == n)
        {
            fnNodeTranslate(c, node - 1);
            break;
        }
    }
}

static void fnNodeTranslate(stTreeTranslationContext *const c, stNodeInterpret *const node)
{
    const stTreeTranslationOptions opt = *c->opt;
    const uint8 style = opt.indentationStyle;
    const uint8 paren = opt.skipParentheses;
    
    #define b(arg, in) fnNodeTranslateBranch(c, node + 1, arg, in);
    #define c(i) fnNodeTranslateChild(c, node, i);
    #define e(...) fnEmit(c, __VA_ARGS__);
    #define indent { fnIndent(c); c->indentation++; }
    
    /* Format: [0] = node type, [1] = skip pattern, [2...] = pattern */
    #define S(P,code) if (P[0]-48==t){ if ((P[1]-32)&1) { code; } else if (P[2+x]-93==1) { code; }}
    
    c->currentNode = node;
    const uint8  t = node->type;
    const uint32 x = host_byteorder_32(node->param);
    S("0 ^^^^^^^^^^^^^^^^                                            ", e("%s", "if "))
    S("0                  ^                                          ", e("%s", "else "))
    S("0  ^                                                          ", e("%s", "!"))
    S("0 ^^^^^^^^^^^^^^^^                                            ", if (!paren && x != 1) e("%s", "("))
    S("0   ^^^^^^                                                    ", c->currentNode = node; e("randomizer %% %d == 0 && ", 1 << (x - 1)))
    S("0         ^^^^^^                                              ", c->currentNode = node; e("randomizer %% %d != 0 && ", 1 << (x - 1)))
    S("0 ^^^^^^^^^^^^^^^^                                            ", b(false, false)) // If statement condition
    S("0 ^^^^^^^^^^^^^^^^                                            ", if (!paren && x != 1) e("%s", ")"))
    S("0                 ^^                                          ", if (style == 0) e("\n"); if (style == 0) indent else e(" "); e("{"); e("\n"))
    S("0                 ^^                                          ", b(false, true); --c->indentation) // Then, else
    S("0                 ^^                                          ", /*e("%s", "\n");*/ indent; e("%s", "}"); if(style == 0) e("%s", "\n"); --c->indentation)
    S("0                   ^                                         ", e("goto"))
    S("0                    ^^^                                      ", e("%s", (const char*[]){"self", "mainActor", "nobody"}[x-19]))
    S("0                       ^^^^^^^^^^^^^^^                       ", e("%s", R3KeywordTable[x]))
    S("1   ^                                                         ", c->currentNode = NULL; e("!("); c->currentNode = node)
    S("1 ^^^^^^^^^^                                                  ", c(0))
    S("1 ^^ ^^^^^^^                                                  ", e(" %s ", conditions[x]))
    S("1 ^^ ^^^^^^^                                                  ", c(1))
    S("1!                                                            ", if (x > 9) e("%s", R3ConditionTable[x]))
    S("1!                                                            ", if (x > 9) e("%s", "("))
    S("1!                                                            ", if (x > 9) c(0))
    S("1!                                                            ", if (x > 9 || x == 2) e(")"))
    S("2     ^              ^                                        ", e("-"))
    S("2 ^^^^ ^           ^^ ^^                                      ", c->currentNode = NULL; e("("); c->currentNode = node)
    S("2 ^^^^^^^^^^^^^^^^^^^^^^^^^^ ^^                               ", c( 0 ))
    S("2 ^^^^ ^^^^^  ^    ^^ ^^      ^                               ", e(" "))
    S("2 ^^^^ ^^^^^^^^^^^^^^ ^^^^^^                                  ", e("%s", operators[x]))
    S("2                           ^                                 ", c->currentNode = NULL; e("(("); c(0); e(")"); e("("); c(1); e("))")) /* TODO: model cast */
    S("2                            ^                                ", e("["))
    S("2 ^^^^ ^^^^^  ^    ^^ ^^      ^                               ", e(" "))
    S("2 ^^^^ ^^^^^  ^^   ^^ ^^^^^^ ^^                               ", c( 1 ))
    S("2                            ^                                ", e("]"))
    S("2 ^^^^ ^           ^^ ^^                                      ", e(")"))
    S("2       ^^^^^^^        ^^^^                                   ", e(";") e("\n"))
    S("3!                                                            ", e("%s", R3FunctionTable[x]))
    S("3!                                                            ", e("("))
    S("3!                                                            ", b(true, false))
    S("3!                                                            ", e(")"))
    S("4!                                                            ", e("%s", R3ProcedureTable[x]))
    S("4!                                                            ", e("("))
    S("4!                                                            ", b(true, false))
    S("4!                                                            ", e(");") e("\n"))
    S("5!                                                            ", e("%s", R3MetaActionTable[x]))
    S("5!                                                            ", e("("))
    S("5!                                                            ", b(true, false))
    S("5!                                                            ", e(");") e("\n"))
    S("6!                                                            ", e("MACRO_BEGIN"))
    S("7!                                                            ", e("MACRO_BEGIN_2"))
    S("8!                                                            ", e("MACRO_END"))
    S("9!                                                            ", e("%s", R3FieldTable[x])) /* Field */
    S(":!                                                            ", e("dsgVar_%d", x)) /* DsgVarRef */
    S(";!                                                            ", e("dsgVar_%d", x)) /* DsgVarRef2 */
    S("<!                                                            ", e("%d", x)) /* Constant */
    S("=!                                                            ", e("%g", host_byteorder_f32(*(float32*)&node->param))) /* Real */
    S(">!                                                            ", e("button(@%X)", x)) /* Joypad button */
    S("?!      vector3 -> (x,y,z):[real/const]                       ", e("Vector3") e("(") b(true, false) e(")")) /* Constant vector */
    S("@!      vector3 -> (x,y,z):[real/const]                       ", e("Vector3") e("(") b(true, false) e(")")) /* Vector */
    S("A!                                                            ", e("mask(%X)", x)) /* Mask */
    S("B!      module reference                                      ", e("%d", x)) /* Module reference */
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
    S("R!                                                            ", e("visualMaterial(@%X)", x))
    S("S!                                                            ", e("particleGenerator(@%X)", x))
    S("T!                                                            ", e("modelRef(@%X)", x))
    S("U!                                                            ", e("modelRef2(@%X)", x))
    S("V!                                                            ", e("customBits(@%X)", x))
    S("W!      caps/constant                                         ", e("0x%X", x))
    S("X!                                                            ", e("unknown(@%X)", x))
    S("Y!      macro/subroutine                                      ", e("macro(@%X)", x); e("\n"));
    #undef S
}

int fnTreeTranslate(stTreeTranslationContext **ctx, stNodeInterpret * tree, stTreeTranslationOptions *opt)
{
    stTreeTranslationContext *const c = *ctx = (stTreeTranslationContext*)malloc(sizeof *c);
    if (!c) return -1;
    
    stTreeTranslationOptions defaultOptions;
    defaultOptions.indentationStyle = 0;
    defaultOptions.indentationSize = 2;
    defaultOptions.skipParentheses = 1;
    
    c->numTokens = 0;
    c->numBlocksAllocated = 0;
    c->indentation = 0;
    c->token = NULL;
    c->currentNode = NULL;
    c->opt = opt ? opt : &defaultOptions;
    
    fnNodeTranslateBranch(c, tree, false, true);

    return 1;
}