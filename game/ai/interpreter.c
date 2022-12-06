//
//  interpreter.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-29.
//

#include "interpreter.h"
#include "tables.h"

#define read() ((struct ScriptNode*)array_get(i->script->nodes, i->pc++))
#define childnode(n) ((struct ScriptNode*)array_get(i->current->children, n))

//#define to_string(v) ((v)->string)
#define to_real(v) ((float)(v)->param)
#define to_long(v) ((int32_t)(v)->param)
#define to_ulong(v) ((uint32_t)(v)->param)
//#define to_actor(v) ((v)->actor) index pointer tables for these
//#define to_superobject(v) ((v)->superobject)
//#define to_behavior(v) ((v)->superobject)
//#define to_ai_model(v) ((v)->superobject)

#define rval(v) ((v)->value.v_float)
#define lval(v) ((v)->value.v_long)
#define ulval(v) ((v)->value.v_ulong)

struct Vector3 vector3val(struct ScriptNode* node)
{
    struct Vector3 v;
    return v;
}

static struct ScriptNode * op(struct ScriptInterpreter* i);

static void jump(int depth, struct ScriptInterpreter* i)
{
    do
        i->current = array_get(i->script->nodes, ++i->pc);
    while (i->current->depth != depth && i->current->depth != 0 /*&& i->pc < array_element_count(i->script->nodes)*/);
    
    info(BOLD COLOR_PINK "Jump to depth: %d (PC = %d)\n", depth, i->pc);
}

static void keyword(struct ScriptInterpreter* i)
{
    struct ScriptNode* keyword = i->current;
    
    info(COLOR_RED "Keyword: %s (@%d)\n", script_keyword_table[keyword->param], keyword->depth);
    
    switch (keyword->param)
    {
        case 0: /* If */
        {
            /* Condition */
            struct ScriptNode* condition = op(i);
            /* Decrement PC to ensure we find the next statement */
            i->pc--;
            /* Jump to `then` */
            jump(keyword->depth, i);
            /* If condition was false, go to `else`,
             * or otherwise next statement at the same depth. */
            if (ulval(condition) == 0) jump(keyword->depth, i);
            break;
        }
            
//        case 1: /* If Not */
//        {
//            /* Condition */
//            struct ScriptNode* condition = op(i);
//            /* Jump to `then` if false */
//            if (ulval(condition) == 0) jump(keyword->depth, i);
//            /* If condition was true, go to `else`,
//             * or otherwise next statement at the same depth. */
//            if (ulval(condition) == 0) jump(keyword->depth, i);
//        }
            
        /* If Not */
        //case 1: op(i); lval(keyword) = lval(keyword) != 0 ? 0 : 1; break; /* If not */
        default: break;
    }
//
//    if (lval(i->current) != 0) /* True */
//    {
//        /* Jump to `then` */
//        jump(keyword->depth, i);
//    }
//    else /* False */
//    {
//        /* Skip `then` */
//        jump(keyword->depth, i);
//        /* Also skip `else` */
//        jump(keyword->depth, i);
//    }
}

#pragma mark - Conditional operations

//"Cond_And", "Cond_Or", "Cond_Not", "Cond_XOR", "Cond_Equal", "Cond_Different", "Cond_Lesser",
//"Cond_Greater", "Cond_LesserOrEqual", "Cond_GreaterOrEqual", "CollidePersoZDDNoWithPerso",
//"CollideModuleZDDNoWithPerso", "CollidePersoAllZDDWithPerso", "CollidePersoZDDWithAnyPerso",

static bool condition(struct ScriptInterpreter* i);

static bool condition_boolean(struct ScriptInterpreter* i, struct ScriptNode* A, struct ScriptNode* B) {
    if (A->type == B->type && A->type == NODE_TYPE_CONSTANT) {
        if (i->current->param == 0 && (lval(A) && lval(B)) /* And */) return true;
        if (i->current->param == 1 && (lval(A) || lval(B)) /* Or  */) return true;
        if (i->current->param == 2 && (lval(A) ==   false) /* Not */) return true;
        if (i->current->param == 3 && (lval(A)  ^ lval(B)) /* XOR */) return true;
    }
    return false;
}

/**/
static bool condition_compare(struct ScriptInterpreter* i, struct ScriptNode* A, struct ScriptNode* B) {
    if (A->type == B->type) {
        
        /* Define numeric conditions. */
        #define P(T,c,x) { if (A->param == T && (x##val(A) c x##val(B))) return true; };
        #define nc(t) P(4,==,t) P(5,!=,t) P(6,<,t) P(7,>,t) P(8,<=,t) P(9,>=,t)
        if (A->type == NODE_TYPE_REAL) nc(r);
        if (A->type == NODE_TYPE_CONSTANT) nc(l);
        
        if (A->type == NODE_TYPE_CONSTANT_VECTOR || A->type == NODE_TYPE_VECTOR) {
            
        }
        
        if (A->type == NODE_TYPE_CONDITION) {
            /* The condition contains another condition, A. */
            i->current = A;
            return condition(i);
        }
        
    } else {
        warning("attempt to compare nodes of different types. condition defaults to false.");
    }
    
    return false;
}

static bool condition(struct ScriptInterpreter* i)
{
    struct ScriptNode* A = childnode(0);
    struct ScriptNode* B = childnode(1);

    info(COLOR_CYAN "Condition param: %X\n", i->current->param);
    
    struct ScriptNode* condition = i->current;
    
    bool cond = false;
    int type = condition->param;
    
    if (type >= 0 && type <= 3) /* Boolean */
        cond = condition_boolean(i, A, B);
    else if (type >= 4 && type <= 8) /* Comparison */
        cond = condition_compare(i, A, B);
    else if (type >= 9 && type <= 13); /* ZDD */
    else if (type >= 14 && type <= 25); /* ZDE */
    else if (type >= 26 && type <= 32); /* ZDM */
    
    /* The condition node itself takes on the resulting value of its condition.
     * As such, conditions are in a way evaluated upwards the root condition node.  */
    ulval(condition) = cond ? 1 : 0;
    
    info(COLOR_RED "Condition: %s (%s, @%d)\n", script_condition_table[i->current->param], cond ? "true": "false", i->current->depth);
    
    return cond;
}

static void subroutine(struct ScriptInterpreter* i)
{
    address offset = i->current->param;
    /* add macro to list and run it */
}

static void vector(struct ScriptInterpreter* i)
{
    
}

//    Keyword (1)
//        Condition (2)
//            Condition (3)
//            Condition (3)
//                Caps (4)
//    Keyword (1)
//        Keyword (2)
//            Condition (3)
//                Condition (4)
//                    Condition (5)
//                        Condition (6)
//                            ActionRef (7)
//                    Condition (5)
//                        DsgVarRef (6)
//                Condition (4)
//                    DsgVarRef (5)
//        Keyword (2)
//            Keyword (3)
//                Condition (4)
//                    ActionRef (5)
//            Keyword (3)
//                Operator (4)
//                    DsgVarRef (5)
//                    Constant (5)
//            Procedure (3)
//                ActionRef (4)
//        Keyword (2)
//            Keyword (3)
//                DsgVarRef (4)
//            Keyword (3)
//                Operator (4)
//                    DsgVarRef (5)
//                    Function (5)
//                        Function (6)
//                            Constant (7)
//                            Constant (7)
//                        Real (6)
//                        Real (6)
//                        Real (6)
//                Operator (4)
//                    DsgVarRef (5)
//                    Function (5)
//                        Operator (6)
//                            DsgVarRef (7)
//                            DsgVarRef (7)
//                        Real (6)
//                        Real (6)
//                Keyword (4)
//                    Condition (5)
//                        DsgVarRef (6)
//                        Real (6)
//                Keyword (4)
//                    Procedure (5)
//                        ActionRef (6)
//                Keyword (4)
//                    Keyword (5)
//                        Condition (6)
//                            DsgVarRef (7)
//                            Real (7)
//                    Keyword (5)
//                        Procedure (6)
//                            ActionRef (7)
//                    Keyword (5)
//                        Procedure (6)
//                            ActionRef (7)
//    EndTree (0)


static struct ScriptNode* op(struct ScriptInterpreter* i)
{
    if (i->pc >= array_element_count(i->script->nodes) - 1) return NULL;
    
    i->current = read();
    
    //info("ptr: %p", next);
    
    info(COLOR_RED "Tree: %s(%d) %d children\n", script_nodetype_table[i->current->type], i->current->type, array_element_count(i->current->children));
    
    switch (i->current->type)
    {
        #define b(y) y(i); break;
        case NODE_TYPE_KEYWORD: b(keyword)
        case NODE_TYPE_CONDITION: b(condition)
        case NODE_TYPE_OPERATOR: break;
        case NODE_TYPE_FUNCTION: break;
        case NODE_TYPE_PROCEDURE: break;
        case NODE_TYPE_META_ACTION: break;
        case NODE_TYPE_BEGIN_MACRO: break;
        case NODE_TYPE_BEGIN_MACRO2: break;
        case NODE_TYPE_END_MACRO: break;
        case NODE_TYPE_FIELD: break;
        case NODE_TYPE_DSGVARREF: break;
        case NODE_TYPE_DSGVARREF2: break;
        case NODE_TYPE_CONSTANT: break;
        case NODE_TYPE_REAL: break;
        case NODE_TYPE_BUTTON: break;
        case NODE_TYPE_CONSTANT_VECTOR:
        case NODE_TYPE_VECTOR: b(vector)
        case NODE_TYPE_MASK: break;
        case NODE_TYPE_MODULEREF: break;
        case NODE_TYPE_DSGVARID: break;
        case NODE_TYPE_STRING: break;
        case NODE_TYPE_LIPSSYNCHROREF: break;
        case NODE_TYPE_FAMILYREF: break;
        case NODE_TYPE_ACTORREF: break;
        case NODE_TYPE_ACTIONREF: break;
        case NODE_TYPE_SUPEROBJECTREF: break;
        case NODE_TYPE_UNKNOWN: break;
        case NODE_TYPE_WAYPOINTREF: break;
        case NODE_TYPE_TEXTREF: break;
        case NODE_TYPE_COMPORTREF: break;
        case NODE_TYPE_MODULEREF2: break;
        case NODE_TYPE_SOUNDEVENTREF: break;
        case NODE_TYPE_OBJECTTABLEREF: break;
        case NODE_TYPE_GAMEMATERIALREF: break;
        case NODE_TYPE_VISUALMATERIAL: break;
        case NODE_TYPE_PARTICLEGENERATOR: break;
        case NODE_TYPE_MODELREF: break;
        case NODE_TYPE_MODELREF2: break;
        case NODE_TYPE_CUSTOMBITS: break;
        case NODE_TYPE_CAPS: break;
        case NODE_TYPE_UNKNOWN2: break;
        case NODE_TYPE_SUBROUTINE: break;
        case NODE_TYPE_NULL: break;
        case NODE_TYPE_NULL2: break;
        case NODE_TYPE_GRAPHREF: break;
        #undef b
    }
    
    return i->current;
}

int script_interpreter_create(struct ScriptInterpreter** intr,
                              struct Script* scpt,
                              struct ScriptInterpreterOptions* opt,
                              struct Engine* engine)
{
    if (!engine) return -1;
    if (!scpt) return -1;
    
    *intr = malloc(sizeof(struct ScriptInterpreter));
    (*intr)->script = scpt;
    (*intr)->pc = 0;
    
    return 0;
}

void script_interpreter_step(struct ScriptInterpreter* intr)
{
    info("\n\n\n\n");
    info(COLOR_RED "step (PC = %d)\n", intr->pc);
    for(int i = 0; i < array_element_count(intr->script->nodes); i++)
    {
        struct ScriptNode* node = array_get(intr->script->nodes, i);
        
        char extra[32];
        memset(extra, 0, 32);
        
        #define inRadius() ((i > intr->pc - 2) && (i < intr->pc + 2))
        if (node->type == NODE_TYPE_KEYWORD && inRadius()) sprintf(extra, "(%s)", script_keyword_table[node->param]);
        if (node->type == NODE_TYPE_CONDITION && inRadius()) sprintf(extra, "(%s)", script_condition_table[node->param]);
        if ((node->type == NODE_TYPE_DSGVARREF || node->type == NODE_TYPE_DSGVARREF2) && inRadius()) sprintf(extra, "(DsgVar_%d)", node->param);
        
        info(BOLD "%s" "%*s" COLOR_PINK "\t%s\n", i == intr->pc ? COLOR_CYAN "->   " : COLOR_GREEN,
             node->depth * 4 + 10 - (i == intr->pc ? 5 : 0),
             script_nodetype_table[node->type],
             extra);
    }
    
    op(intr);
}
