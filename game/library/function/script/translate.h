//
//  translate.h
//  r3-tas-memorymap
//
//  Created by Jba03 on 2022-12-24.
//

#ifndef translate_h
#define translate_h

#include "tables.h"
#include "script.h"
#include <string.h>

#define TOKEN_MAX_LENGTH 256

struct translation_token
{
    char string[TOKEN_MAX_LENGTH];
    struct script_node* node;
    struct translation_token* next;
    int offset; /* Offset in tree */
};

struct translation
{
    struct script_node* original_tree;
    struct script_node* tree;
    struct script_node* current;
    
    unsigned n_tokens;
    struct translation_token token[4096];
    
    unsigned indent;
};

#define IsEndOfTree(Node) (Node->type == script_node_type_end_macro)

#define i_emit(Node, ...) \
{ \
    struct translation_token* tok = &t->token[t->n_tokens]; \
    memset(tok->string, 0, TOKEN_MAX_LENGTH); \
    sprintf(tok->string, __VA_ARGS__); \
    tok->node = Node; \
    tok->offset = 0; \
    if (Node) tok->offset = (int)(((struct script_node*)Node) - t->tree); \
    t->n_tokens++; \
}

#define emit(...) i_emit(t->current, __VA_ARGS__)
#define emit2(...) i_emit(NULL, __VA_ARGS__)

#define param() host_byteorder_32(t->current->param)

static inline void translate_node(struct translation* t);
static inline void translate_childnode(struct translation* t, int child);
static inline void translate_branch(struct translation* t, bool args);

#pragma mark - Keyword

static inline void translate_keyword(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    if (param == 0) { /* If */
        emit("if ")
        //emit2("(")
        translate_childnode(t, 0);
        //emit2(")")
        if (t->current->depth == 1) emit2("\n")
    } else if (param == 1) { /* IfNot */
        emit("if ")
        emit2("!(")
        translate_childnode(t, 0);
        emit2(")")
        if (t->current->depth == 1) emit2("\n")
    } else if (param >= 2 && param <= 7) { /* If{X} */
        emit("if ")
        emit("framerule")
        emit2("(%d)", 1 << (param - 1));
        emit2(" && ")
        translate_childnode(t, 0);
        //emit2(")")
    } else if (param >= 8 && param <= 13) { /* IfNot{X} */
        emit("if randomizer %% %d != 0 && (%s)", 1 << (param - 1), "");
    } else if (param == 14) { /* IfDebug */
        emit("if #debug && (%s)", "");
    } else if (param == 15) { /* IfNotU64 */
        emit("IfNotU64 (%s)", "");
    } else if (param == 16) { /* Then */
        emit2("%s", "{") t->indent++;
        emit2("\n")
        translate_branch(t, false);
        emit2("%*s", --t->indent * 4+1, "}")
    } else if (param == 17) { /* Else */
        //emit2("\n")
        emit("else")
        emit2("\n")
        emit2("%*s", t->indent * 4+1, "{") t->indent++;
        emit2("\n")
        translate_branch(t, false);
        emit2("%*s", --t->indent * 4+1, "}")
    } else if (param == 18) {
        /* goto */
    } else if (param == 19) {
        emit("self")
    } else if (param == 20) {
        emit("main_actor")
    }
}

#pragma mark - Condition

static inline void translate_condition(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    const char* condition = script_condition_table[param];
    
    switch (param)
    {
        case 0: translate_childnode(t, 0); emit(" && "); translate_childnode(t, 1); break; /* And */
        case 1: translate_childnode(t, 0); emit(" || "); translate_childnode(t, 1); break; /* Or */
        case 2: emit2("!("); translate_childnode(t, 0); emit2(")") break; /* Not */
        case 3: translate_childnode(t, 0); emit(" ^ "); translate_childnode(t, 1); break; /* XOr */
        case 4: translate_childnode(t, 0); emit(" == "); translate_childnode(t, 1); break; /* Eq */
        case 5: translate_childnode(t, 0); emit(" != "); translate_childnode(t, 1); break; /* NEq */
        case 6: translate_childnode(t, 0); emit(" < "); translate_childnode(t, 1); break; /* Lt */
        case 7: translate_childnode(t, 0); emit(" > "); translate_childnode(t, 1); break; /* Gt */
        case 8: translate_childnode(t, 0); emit(" <= "); translate_childnode(t, 1); break; /* Le */
        case 9: translate_childnode(t, 0); emit(" >= "); translate_childnode(t, 1); break; /* Ge */
            
        case 45: emit("PressedBut") emit2("(") translate_childnode(t, 0); emit2(")") break; /* JustPressedBut */
            
        default: emit("%s", condition); emit2("("); translate_childnode(t, 0); emit2(")");
    }
}

#pragma mark - Operator

static inline void translate_operator(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    switch (param)
    {
        case 0: emit("("); translate_childnode(t, 0); emit(" + "); translate_childnode(t, 1); emit(")"); break; /* add */
        case 1: emit("("); translate_childnode(t, 0); emit(" - "); translate_childnode(t, 1); emit(")"); break; /* subtract */
        case 2: emit("("); translate_childnode(t, 0); emit(" * "); translate_childnode(t, 1); emit(")"); break; /* multiply */
        case 3: emit("("); translate_childnode(t, 0); emit(" / "); translate_childnode(t, 1); emit(")"); break; /* divide */
        case 4: emit("-("); translate_childnode(t, 0); emit(")"); break; /* negate */
        case 5: translate_childnode(t, 0); emit(" %% "); translate_childnode(t, 1); break; /* modulo */
        case 6: translate_childnode(t, 0); emit(" += "); translate_childnode(t, 1); emit2(";"); emit2("\n"); break; /* add and assign */
        case 7: translate_childnode(t, 0); emit(" -= "); translate_childnode(t, 1); emit2(";"); emit2("\n"); break; /* subtract and assign */
        case 8: translate_childnode(t, 0); emit(" *= "); translate_childnode(t, 1); emit2(";"); emit2("\n"); break; /* multiply and assign */
        case 9: translate_childnode(t, 0); emit(" /= "); translate_childnode(t, 1); emit2(";"); emit2("\n"); break; /* divide and assign */
        case 10: translate_childnode(t, 0); emit("++;"); emit2("\n"); break; /* increment */
        case 11: translate_childnode(t, 0); emit("--;"); emit2("\n"); break; /* decrement */
        case 12: translate_childnode(t, 0); emit(" = "); translate_childnode(t, 1); emit2(";"); emit2("\n"); break; /* assign */
        case 13: translate_childnode(t, 0); emit("."); translate_childnode(t, 1); break; /* dot, field access */
        case 14: translate_childnode(t, 0); emit(".X"); break; /* vector component X */
        case 15: translate_childnode(t, 0); emit(".Y"); break; /* vector component Y */
        case 16: translate_childnode(t, 0); emit(".Z"); break; /* vector component Z */
        case 17: emit("("); translate_childnode(t, 0); emit(" + "); translate_childnode(t, 1); emit(")"); break; /* add vectors */
        case 18: emit("("); translate_childnode(t, 0); emit(" - "); translate_childnode(t, 1); emit(")"); break; /* subtract vectors */
        case 19: emit("-"); translate_childnode(t, 0); break; /* negate vector */
        case 20: emit("("); translate_childnode(t, 0); emit(" * "); translate_childnode(t, 1); emit(")"); break; /* multiply vector by scalar */
        case 21: emit("("); translate_childnode(t, 0); emit(" / "); translate_childnode(t, 1); emit(")"); break; /* divide vector by scalar */
        case 22: translate_childnode(t, 0); emit(".X = "); translate_childnode(t, 1); break; /* assign to vector component X */
        case 23: translate_childnode(t, 0); emit(".Y = "); translate_childnode(t, 1); break; /* assign to vector component Y */
        case 24: translate_childnode(t, 0); emit(".Z = "); translate_childnode(t, 1); break; /* assign to vector component Z */
        case 25: translate_childnode(t, 0); emit("."); translate_childnode(t, 1); break; /* ultra */ break;
        case 26: emit("(("); translate_childnode(t, 0); emit(")"); emit("(") translate_childnode(t, 1); emit("))") break; /* modelcast */
        case 27: translate_childnode(t, 0); emit("["); translate_childnode(t, 1); emit("]"); break;
    }
}

#pragma mark - Function

static inline void translate_function(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    const char* function = script_function_table[param];
    
//    if (strcmp(function, "TernOp") == 0) /* Ternary operation */
//    {
//        emit2("(")
//        translate_childnode(t, 0);
//        emit2(" ? ")
//        translate_childnode(t, 1);
//        emit2(" : ")
//        translate_childnode(t, 2);
//        emit2(")")
//        return;
//    }
    
    emit("%s", function)
    emit2("(")
    translate_branch(t, true);
    emit2(")")
}

#pragma mark - Procedure

static inline void translate_procedure(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    const char* proc = script_procedure_table[param];
    emit("%s", proc)
    emit2("(")
    translate_branch(t, true);
    emit2(");")
    emit2("\n")
}

#pragma mark - MetaAction

static inline void translate_meta_action(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    const char* meta_action = script_meta_action_table[param];
    emit("%s", meta_action)
    emit2("(")
    translate_branch(t, true);
    emit2(");")
}

#pragma mark - Field

static inline void translate_field(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    const char* field = script_field_table[param];
    emit("%s", field)
}

#pragma mark - DsgVar

static inline void translate_dsgvar(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    emit("DsgVar_%d", param)
}

#pragma mark - Constant

static inline void translate_constant(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    emit("%i", param)
}

#pragma mark - Real

static inline void translate_real(struct translation* t)
{
    uint32_t intrm = host_byteorder_32(t->current->param);
    float value = *((float*)&intrm);
    emit("%.5gf", value)
}

#pragma mark - Button

static inline void translate_button(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    emit("%X", param)
}

#pragma mark - Vector

static inline void translate_vector(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    emit("Vector") emit2("(")
    translate_branch(t, true);
    emit2(")")
}

#pragma mark - String

static inline void translate_string(struct translation* t)
{
    const char* str = (const char*)pointer(t->current->param);
    emit("%s", str);
}

#pragma mark - Reference

static inline void translate_reference(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    switch (t->current->type)
    {
        case script_node_type_lipssynchroref: emit("lipsyncref(@%X)", param); break;
        case script_node_type_familyref: emit("familyref(@%X)", param); break;
        case script_node_type_actorref: emit("actor(@%X)", param); break;
        case script_node_type_actionref: emit("action(@%X)", param); break;
        case script_node_type_superobjectref: emit("superobject(@%X)", param); break;
        case script_node_type_solinksref: emit("solinks(@%X)", param); break;
        case script_node_type_waypointref: emit("waypoint(@%X)", param); break;
        case script_node_type_textref: emit("text(@%X)", param); break;
        case script_node_type_behaviorref: emit("behavior(@%X)", param); break;
        case script_node_type_moduleref: emit("moduleref(@%X)", param); break;
        case script_node_type_soundeventref: emit("soundevent(@%X)", param); break;
        case script_node_type_objecttableref: emit("objecttable(@%X)", param); break;
        case script_node_type_gamematerialref: emit("gamematerial(@%X)", param); break;
        case script_node_type_visualmaterial: emit("visualmaterial(@%X)", param); break;
        case script_node_type_particlegenerator: emit("particlegenerator(@%X)", param); break;
        case script_node_type_modelref: emit("modelref(@%X)", param); break;
        case script_node_type_modelref2: emit("modelref2(@%X)", param); break;
    }
}

#pragma mark - Custom bits

static inline void translate_custombits(struct translation* t)
{
    emit("%d", host_byteorder_32(t->current->param));
}

#pragma mark - Caps

static inline void translate_caps(struct translation* t)
{
    emit("0x%X", host_byteorder_32(t->current->param));
}


#pragma mark - Subroutine

static inline void translate_subroutine(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    emit("macro(@%X)", param)
    emit2("\n")
}

#pragma mark - Translation

static inline void seektonextofdepth(struct translation* t)
{
    unsigned depth = t->current->depth;
    if IsEndOfTree(t->current++) return;
    while (!IsEndOfTree(t->current) && t->current->depth != depth)
        t->current++;
}

static inline void translate_node(struct translation* t)
{
    switch (t->current->type)
    {
        #define n(x) translate_##x(t); break;
        case script_node_type_keyword: n(keyword)
        case script_node_type_condition: n(condition)
        case script_node_type_operator: n(operator)
        case script_node_type_function: n(function)
        case script_node_type_procedure: n(procedure)
        case script_node_type_meta_action: n(meta_action)
        case script_node_type_begin_macro: break;
        case script_node_type_begin_macro2: break;
        case script_node_type_end_macro: break; /* Endtree */
        case script_node_type_field: n(field)
        case script_node_type_dsgvarref: n(dsgvar)
        case script_node_type_dsgvarref2: n(dsgvar)
        case script_node_type_constant: n(constant)
        case script_node_type_real: n(real)
        case script_node_type_button: n(button)
        case script_node_type_constant_vector: n(vector)
        case script_node_type_vector: n(vector)
        case script_node_type_mask: break;
        case script_node_type_moduleref: break;
        case script_node_type_dsgvarid: break;
            
        case script_node_type_string: n(string)
            
        case script_node_type_lipssynchroref: n(reference)
        case script_node_type_familyref: n(reference)
        case script_node_type_actorref: n(reference)
        case script_node_type_actionref: n(reference)
        case script_node_type_superobjectref: n(reference)
        case script_node_type_solinksref: n(reference)
        case script_node_type_waypointref: n(reference)
        case script_node_type_textref: n(reference)
        case script_node_type_behaviorref: n(reference)
        case script_node_type_moduleref2: n(reference)
        case script_node_type_soundeventref: n(reference)
        case script_node_type_objecttableref: n(reference)
        case script_node_type_gamematerialref: n(reference)
        case script_node_type_visualmaterial: n(reference)
        case script_node_type_particlegenerator: n(reference)
        case script_node_type_modelref: n(reference)
        case script_node_type_modelref2: n(reference)
            
        case script_node_type_custombits: n(custombits)
        case script_node_type_caps: n(caps);
            
        case script_node_type_subroutine: n(subroutine)
        #undef y
    }
    
    seektonextofdepth(t);
}

static inline void translate_childnode(struct translation* t, int child)
{
    unsigned min = t->current->depth;
    unsigned occ = 0;
    
    struct script_node* orig = t->current;
    struct script_node* node = orig + 1;
    while (!IsEndOfTree(node) && node->depth > min)
    {
        if (node->depth == min + 1 && occ++ == child)
        {
            t->current = node;
            translate_node(t);
            break;
        }
        node++;
    }
    
    t->current = orig;
}

static inline void translate_branch(struct translation* t, bool args)
{
    unsigned depth = t->current->depth + 1;
    
    struct script_node* orig = t->current;
    struct script_node* node = orig + 1;
    struct script_node* last = NULL;
    while (!IsEndOfTree(node) && node->depth >= depth)
    {
        if (node->depth == depth)
        {
            t->current = node;
            if (!args) emit2("%*s", t->indent * 4, ""); /* Indent */
            translate_node(t);
            
            /* Avoid emitting newline twice */
            if (node->type != script_node_type_operator
            &&  node->type != script_node_type_procedure
            &&  node->type != script_node_type_subroutine && !args) emit2("\n");
            /* Argument separator */
            if (args) emit2(", ");
            
            last = node;
        }
        
        node++;
    }
    
    /* If this was the last node of an argument list, remove the comma. */
    if (last && args) memset(t->token[t->n_tokens--].string, 0, TOKEN_MAX_LENGTH);
    
    t->current = orig;
}

static inline struct translation* script_translate(struct script_node* tree)
{
    if (!tree) return NULL;
//    struct script_node* tree = (struct script_node*)pointer(script->tree);
//    if (!tree) return NULL;
    
    unsigned N = 0;
    struct script_node* dup = tree;
    while (!IsEndOfTree(dup++)) N++;
    
    /* Duplicate the tree, so platform memory stays unmodified. */
    dup = (struct script_node*)malloc(sizeof *tree * (N+1));
    memcpy(dup, tree, sizeof *tree * (N+1));
    
    struct translation* t = (struct translation*)malloc(sizeof *t);
    t->original_tree = tree;
    t->current = t->tree = dup;
    t->n_tokens = 0;
    t->indent = 0;
    
    while (!IsEndOfTree(t->current))
        if (t->current->depth == 1)
            translate_node(t);

    return t;
}

static inline void translation_destroy(struct translation* t)
{
    if (t)
    {
        free(t->tree);
        free(t);
    }
}

#endif /* translate_h */
