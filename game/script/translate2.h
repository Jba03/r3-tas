//
//  translate.h
//  r3-tas-memorymap
//
//  Created by Jba03 on 2022-12-24.
//

#ifndef translate_h
#define translate_h

#include "script.h"

#define TOKEN_MAX_LENGTH 256

struct translation_token
{
    char string[TOKEN_MAX_LENGTH];
    struct script_node* node;
    struct translation_token* next;
};

struct translation
{
    struct script_node* tree;
    struct script_node* current;
    
    unsigned n_tokens;
    struct translation_token token[1024];
    
    unsigned indent;
};

#define IsEndOfTree(node) (node->type == script_node_type_end_macro)

#define i_emit(Node, ...) \
{ \
    struct translation_token* tok = &t->token[t->n_tokens]; \
    memset(tok->string, 0, TOKEN_MAX_LENGTH); \
    sprintf(tok->string, __VA_ARGS__); \
    tok->node = Node; \
    t->n_tokens++; \
}

#define emit(...) i_emit(t->current, __VA_ARGS__)
#define emit2(...) i_emit(NULL, __VA_ARGS__)

#define param() host_byteorder_32(t->current->param)

static inline void translate_node(struct translation* t);
static inline void translate_childnode(struct translation* t, int child);
static inline void translate_children(struct translation* t);

//static inline const struct script_node* childnode(int depth)
//{
//
//}

#pragma mark - Keyword

static inline void translate_keyword(struct translation* t)
{
    uint32_t param = host_byteorder_32(t->current->param);
    if (param == 0) { /* If */
        emit("if ")
        emit2("(")
        //translate_childnode(t, 0);
        emit2(")")
        emit2("\n")
    } else if (param == 1) { /* IfNot */
        emit("if !(%s)", "hi")
        emit2("\n")
    } else if (param >= 2 && param <= 7) { /* If{X} */
        emit("if randomizer %% %d == 0 && (%s)", 1 << (param - 1), "");
    } else if (param >= 8 && param <= 13) { /* IfNot{X} */
        emit("if randomizer %% %d != 0 && (%s)", 1 << (param - 1), "");
    } else if (param == 14) { /* IfDebug */
        emit("if #debug && (%s)", "");
    } else if (param == 15) { /* IfNotU64 */
        emit("IfNotU64 (%s)", "");
    } else if (param == 16) { /* Then */
        emit2("%*s", t->indent++, "{")
        //translate_branches()
        emit2("%*s", --t->indent, "}")
    } else if (param == 17) { /* Else */
        emit("else")
        emit2("\n")
        emit2("%*s", t->indent++, "{")
        //translate_branches()
        emit2("%*s", --t->indent, "}")
    }
}

#pragma mark - Translation

static inline void translate_node(struct translation* t)
{
    switch (t->current++->type)
    {
        #define n(x) translate_##x(t); break;
        case script_node_type_keyword: n(keyword)
        #undef y
    }
}

static inline void translate_childnode(struct translation* t, int child)
{
    unsigned min = t->current->depth;
    unsigned occ = 0;
    t->current++;
    
    while (!IsEndOfTree(t->current) && t->current->depth > min)
        if (t->current->depth == min + 1) /* This is a childnode */
            if (occ++ == child) translate_node(t);
}

static inline struct translation* script_translate(struct script* script)
{
    if (!script) return NULL;
    struct script_node* tree = (struct script_node*)pointer(script->tree);
    if (!tree) return NULL;
    
    struct script_node* dup = tree;
    while (!IsEndOfTree(dup++));
    /* Duplicate the tree, so platform memory stays unmodified. */
    dup = (struct script_node*)malloc(sizeof *tree * (dup - tree));
    memcpy(dup, tree, sizeof *tree * (dup - tree));
    
    struct translation* t = (struct translation*)malloc(sizeof *t);
    t->current = t->tree = dup;
    t->n_tokens = 0;
    t->indent = 0;
    
    while (!IsEndOfTree(t->current))
        translate_node(t);
    
    
    return t;
}

#endif /* translate_h */
