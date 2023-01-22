/* intcpa: CPA script interpreter  */
/* Created by Jba03 on 2022-12-11. */

#ifndef intcpa_h
#define intcpa_h

#include "script.h"
#include "dsg.h"
#include "structure.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#define INTCPA


#define intcpa_n_functions 446
#define intcpa_n_conditions 168
#define intcpa_rstack_size 64

#define intcpa_true     0xFFFFFFFF
#define intcpa_false    0x00000000

#define intcpa_tag_skip 0x534B4950 // Better make it uncommon

struct intcpa_param
{
    /* mirror: mirror the program to an internal */
    /* buffer to avoid tampering platform memory */
    uint8_t mirror;
    /* nowrite: disable writing of dsgvars */
    uint8_t nowrite;
    /* convert: convert to native byteorder before interpreting */
    uint8_t convert;
    /* actor_self: address of actor whose AI contains this script */
    uint32_t actor_self;
};

struct intcpa;

#include "inttables.h"

typedef void (*intcpa_userfunc)(struct intcpa* intr);

struct intcpa
{
    struct script_node* original_tree;
    /* script node tree */
    struct script_node* tree;
    /* currently interpreted node */
    struct script_node* current;
    /* in assign mode? */
    bool assign;
    /* global frame counter randomizer */
    unsigned randomizer;
    /* is the interpreter finished? */
    bool finished;
    /* return stack */
    struct script_node* rstack[intcpa_rstack_size];
    unsigned rsp;
    /* current macro, if any */
    struct macro* macro;
    
    /* user callbacks */
    intcpa_userfunc function[intcpa_n_functions];
    intcpa_userfunc condition[intcpa_n_conditions];
    struct intcpa_dsg dsg;
    
    /* interpreter options */
    struct intcpa_param i_opt;
};

#pragma mark - Interpreter

#define IsEndOfTree(node) (node->type == script_node_type_end_macro)
#define fetch() (++intr->current)
#define param(node) (node->param)
#define op() intcpa_advance(intr)

/* External functions */
#define extern_function(fn) ({ if(intr->function.fn) intr->function.fn(intr); })
#define extern_condition(fn) ({ if(intr->condition.fn) intr->condition.fn(intr); })

#define lval(nd)    *((int32_t*)&nd->param)
#define ulval(nd)   *((uint32_t*)&nd->param)
#define rval(nd)    host_byteorder_f32((nd)->param)

static inline struct script_node* intcpa_advance(struct intcpa* intr);
static inline struct script_node* intcpa_current(struct intcpa* intr);
static inline void intcpa_open(struct intcpa* intr, struct script_node* tree, struct intcpa_param* param);
static inline void intcpa_close(struct intcpa* intr);

// Find next node whose depth is lower or same as that of the current.
static inline struct script_node* intcpa_find_next(struct intcpa* intr)
{
    struct script_node* test = intr->current;
    struct script_node* next = intr->current + 1;
    while (!IsEndOfTree(next) && !(next->depth <= test->depth)) next++;
    return next; // The node will be found at prefetch.
}

// Seek to the next node whose depth is lower or same as that of the current.
static inline void intcpa_seek_next(struct intcpa* intr)
{
    intr->current = intcpa_find_next(intr);
}

#pragma mark - Keyword

static inline struct script_node* intcpa_keyword(struct intcpa* intr)
{
    struct script_node* keyword = intr->current - 1;
    
    if (keyword->param >= 0 && keyword->param <= 13) // If, IfNot, IfX, IfNotX
    {
        bool x = true;
        // After the keyword comes the keyword's condition, which does not
        // necessarily need to be a condition node, but can also be dsgvars or
        // operators, assuming the result can be treated as a condition.
        struct script_node *condition = intcpa_advance(intr);
        // Parity for framerule-dependent conditional keywords
        unsigned par = intr->randomizer % (1 << (keyword->param - 1));
        
        if (keyword->param == 0) x = condition->param != 0; // If
        if (keyword->param == 1) x = condition->param == 0; // IfNot
        if (keyword->param >= 2 && keyword->param <= 7) x = par == 0; // IfX
        if (keyword->param >= 8 && keyword->param <= 13) x = par != 0; // IfNotX
        
        if (x) // Condition is true
        {
            intr->current = keyword;
            // Seek to `Then`
            intcpa_seek_next(intr);
            // Find `Else`, if any
            struct script_node* else_node = intcpa_find_next(intr);
            if (else_node->type == script_node_type_keyword && else_node->param == 17)
                // Change the parameter of the else statement, so we can skip it later.
                // It is possible that the script node structure reserves some field for
                // just this purpose, where the current padding bytes now reside.
                else_node->param = intcpa_tag_skip;
        }
        else // Condition is false
        {
            intr->current = keyword;
            // Seek to `Then`
            intcpa_seek_next(intr);
            // Seek to `Else`
            intcpa_seek_next(intr);
        }
    }
    
    if (keyword->param == intcpa_tag_skip)
    {
        intr->current = keyword;
        intcpa_seek_next(intr);
    }
    
    return keyword;
}

#pragma mark - Condition

static inline struct script_node* intcpa_condition(struct intcpa* intr)
{
    struct script_node* condition = intr->current - 1;
    
    #pragma mark Boolean condition
    if (condition->param >= 0 && condition->param <= 3)
    {
        if (ulval(condition) == 0) condition->param = lval(op()) == lval(op()); // And
        if (ulval(condition) == 1) condition->param = lval(op()) || lval(op()); // Or
        if (ulval(condition) == 2) condition->param =              !lval(op()); // Not
        if (ulval(condition) == 3) condition->param = lval(op())  ^ lval(op()); // XOr
    }

    #pragma mark Comparison condition
    if (condition->param >= 4 && condition->param <= 9)
    {
        struct script_node* A = op();
        struct script_node* B = op();
        
        float x = 0.0f;
        float y = 0.0f;
        
        // Convert the types to floating point, so that they can be compared
        // in a single statement. Don't know how this affects precision though.
        if (A->type == 12) x = (float)ulval(A);
        if (A->type == 13) x = rval(A);
        if (B->type == 12) y = (float)ulval(A);
        if (B->type == 13) y = rval(A);
        
        printf("compare: %f(%X, %d) {%d} %f(%X, %d)\n", x, A->param, A->type, condition->param, y, A->param, B->type);
        
        if (ulval(condition) == 4) condition->param = x == y;
        if (ulval(condition) == 5) condition->param = x != y;
        if (ulval(condition) == 6) condition->param = x  < y;
        if (ulval(condition) == 7) condition->param = x  > y;
        if (ulval(condition) == 8) condition->param = x <= y;
        if (ulval(condition) == 9) condition->param = x >= y;
    }
    
    // Call extern condition
    if (condition->param < intcpa_n_conditions)
    {
        if (intr->condition[condition->param])
        {
            // Hooray
            (*intr->condition[condition->param])(intr);
        }
        else
        {
            // Skip the condition
            intr->current = condition;
            intcpa_seek_next(intr);
        }
    }
    
    if (condition->param > 0) condition->param = intcpa_true;

    condition->param = 0;
    
    return condition;
}

#pragma mark - Operator

static inline struct script_node* intcpa_operator(struct intcpa* intr)
{
    struct script_node* op = intr->current - 1;
    
    #pragma mark Add, subtract, multiply, divide
    if (op->param <= 3)
    {
        struct script_node* A = op();
        struct script_node* B = op();
        
        #define arithmeticOP(t,a) \
        if (op->param == t && A->type == 12 && B->type == 12) op->param = lval(A) a lval(B); \
        if (op->param == t && A->type == 12 && B->type == 13) op->param = lval(A) a rval(B); \
        if (op->param == t && A->type == 13 && B->type == 12) op->param = rval(A) a lval(B); \
        if (op->param == t && A->type == 13 && B->type == 13) op->param = rval(A) a rval(B); \

        arithmeticOP(0, +)
        arithmeticOP(1, -)
        arithmeticOP(2, *)
        arithmeticOP(3, /)
    }
    
    #pragma mark Negate
    if (op->param == 4)
    {
        struct script_node* A = op();
        if (A->type == 12) op->param = -lval(A);
        if (A->type == 13) *((float*)&op->param) = -rval(A);
    }
    
    #pragma mark Modulo
    if (op->param == 5)
    {
        struct script_node* A = op();
        struct script_node* B = op();
        if (A->type == 12 && B->type == 12) op->param = lval(A) % lval(B);
    }
    
    #pragma mark Affect and assign
    if (op->param >= 6 && op->param <= 11)
    {
        struct script_node* A = op();
        struct script_node* B = op();
    }
    
    #pragma mark Assign
    if (op->param == 12)
    {
        struct script_node* A = op();
        struct script_node* B = op();
        
        if (intr->dsg.Write)
        {
//            void (*Read)(struct intcpa* intr, pointer actor, unsigned var, uint32_t* result);
//            void (*Write)(struct intcpa* intr, pointer actor, unsigned var, uint32_t store);
            //intr->dsg.Write()
        }
    }
    
    if (op->param > 12) // Skip
    {
        intcpa_seek_next(intr);
    }
    
    return op;
}

#pragma mark - Function

static inline struct script_node* intcpa_function(struct intcpa* intr)
{
    struct script_node* function = intr->current - 1;
    
    if (function->param < intcpa_n_functions)
    {
        if (intr->function[function->param])
        {
            (*intr->function[function->param])(intr);
        }
        else
        {
            intr->current = function;
            intcpa_seek_next(intr);
        }
    }
    
    return function;
}

#pragma mark - Reference

static inline struct script_node* intcpa_dsgvarref(struct intcpa* intr)
{
    struct script_node* dsgvar = intr->current - 1;
    
    uint32_t type = 0;
    void* data = NULL;
    
    if (intr->dsg.Read && !intr->assign) // Make sure it is a read operation
    {
        intr->dsg.Read(intr, intr->i_opt.actor_self, ulval(dsgvar), &type, &data);

        printf("data: %X\n", offset(data));
        if (data)
        {
            switch (type)
            {
                case dsgvar_type_boolean:
                    dsgvar->type = 12; // Constant
                    dsgvar->param = *(uint8_t*)data;
                    break;

                case dsgvar_type_byte:
                    dsgvar->type = 12; // Constant
                    dsgvar->param = *(int8_t*)data;
                    break;

                case dsgvar_type_ubyte:
                    dsgvar->type = 12; // Constant
                    dsgvar->param = *(uint8_t*)data;
                    break;

                case dsgvar_type_short:
                    dsgvar->type = 12; // Constant
                    dsgvar->param = host_byteorder_16(*(int16_t*)data);
                    break;

                case dsgvar_type_ushort:
                    dsgvar->type = 12; // Constant
                    dsgvar->param = host_byteorder_16(*(uint16_t*)data);
                    break;

                case dsgvar_type_int:
                    dsgvar->type = 12; // Constant
                    dsgvar->param = host_byteorder_32(*(int32_t*)data);
                    break;

                case dsgvar_type_uint:
                    dsgvar->type = 12; // Constant
                    dsgvar->param = host_byteorder_32(*(uint32_t*)data);
                    break;

                case dsgvar_type_float: {
                    dsgvar->type = 13; // Real
                    uint32_t intrm = host_byteorder_32(*(uint32_t*)data);
                    *((float*)&dsgvar->param) = *((float*)&intrm);
                    break;
                }

                case dsgvar_type_actor:
                    dsgvar->param = host_byteorder_32(*(pointer*)data);
                    break;
            }
        }
        
        //printf("read type: %s\n", dsgvar_typenames[type]);
    }
    
    return dsgvar;
}

#pragma mark - Vector

static inline struct script_node* intcpa_vector(struct intcpa* intr)
{
    struct script_node* vector = intr->current - 1;
    // Assume each vector has only three components.
    struct script_node* nx = op();
    struct script_node* ny = op();
    struct script_node* nz = op();
    
    return vector;
}

#pragma mark - Subroutine

static inline struct script_node* intcpa_subroutine(struct intcpa* intr)
{
    struct script_node* subroutine = intr->current - 1;
    
    struct macro* macro = (struct macro*)pointer(host_byteorder_32(subroutine->param));
    // Fetch initial tree
    struct script_node* tree_initial = (struct script_node*)doublepointer(macro->script_initial);
    
    if (macro && tree_initial)
    {
        // Push the original tree onto the stack
        intr->rstack[intr->rsp++] = intr->original_tree;
        // Push the tree root onto the stack
        intr->rstack[intr->rsp++] = intr->tree;
        // Push the macro onto the stack
        intr->rstack[intr->rsp++] = (struct script_node*)macro;
        // Push the current node onto the stack
        intr->rstack[intr->rsp++] = intr->current;
        
        intr->macro = macro;
        
        printf("Push to macro call stack (rsp=%d)\n", intr->rsp);
        
        intcpa_open(intr, tree_initial, &intr->i_opt);
    }
    
    return subroutine;
}

#pragma mark - Endtree

static inline struct script_node* intcpa_endtree(struct intcpa* intr)
{
    struct script_node* endtree = intr->current - 1;
    // Close the current script and free it if necessary
    //intcpa_close(intr);
    
    if (intr->rsp >= 2)
    {
        intcpa_close(intr);
        
        // Return to the current node
        intr->current = intr->rstack[--intr->rsp];
        // Pop tree off the stack
        intr->macro = (struct macro*)intr->rstack[--intr->rsp];
        // Pop tree off the stack
        intr->tree = intr->rstack[--intr->rsp];
        // Pop original tree off the stack
        intr->original_tree = intr->rstack[--intr->rsp];
        // Did we leave macro mode?
        if (intr->rsp == 0) intr->macro = NULL;
        
        printf("Pop from macro call stack (rsp=%d)\n", intr->rsp);
    }
    else
    {
        // Stack empty: program finished.
        intr->current--;
        intr->finished = true;
    }
    
    return endtree;
}

INTCPA static inline struct script_node* intcpa_advance(struct intcpa* intr)
{
    if (intr->finished) return intr->current - 1;
    
    printf("advance: %X!\n", intr->current->type);
    
    struct script_node* branch_root = intr->current;
    struct script_node* branch_entry = fetch();
    
    switch (branch_root->type)
    {
        #define t(y) return intcpa_##y(intr); break;
        case script_node_type_keyword: t(keyword)
        case script_node_type_condition: t(condition)
        case script_node_type_operator: t(operator)
        case script_node_type_function: t(function)
            
        case script_node_type_dsgvarref: t(dsgvarref)
        case script_node_type_dsgvarref2: t(dsgvarref)
            
        case script_node_type_vector: t(vector)
            
        case script_node_type_end_macro: t(endtree)
            
            
        case script_node_type_subroutine: t(subroutine)
            
        default: break;
        #undef t
    }
    
    /* Seek to next branch */
    //intcpa_seek_next(intr);
    //intr->current++;
    
    //return intr->current - 1;
    
    return intr->current - 1;
}

INTCPA static inline struct script_node* intcpa_current(struct intcpa* intr)
{
    return intcpa_advance(intr) - 1;
};

INTCPA static inline void intcpa_open(struct intcpa* intr, struct script_node* tree, struct intcpa_param* param)
{
    intr->original_tree = tree;
    
    if (intr->i_opt.mirror)
    {
        /* Copy the original tree into an internal buffer. */
        unsigned int L = tree_length(tree) * sizeof(struct script_node);
        intr->tree = (struct script_node*)malloc(L);
        memcpy(intr->tree, tree, L);
    }
    else
    {
        /* Interpret tree directly from memory. */
        intr->tree = tree;
    }
    
    // Convert node byteorder
    struct script_node* cc = intr->tree;
    while (!IsEndOfTree(cc))
    {
        cc->param = host_byteorder_32(cc->param);
        ++cc;
    }
    
    intr->current = intr->tree;
}

INTCPA static inline void intcpa_close(struct intcpa* intr)
{
    if (intr->i_opt.mirror) free(intr->tree);
    intr->tree = NULL;
    intr->current = NULL;
}

#pragma mark - Interface

INTCPA static struct intcpa* intcpa_interpreter_create(struct intcpa_param* param, struct script* scpt)
{
    struct script_node* original_tree = (struct script_node*)pointer(scpt->tree);
    if (!original_tree)
        return NULL;
    
    printf("script: %X\n", offset(scpt));
    
    struct intcpa* intr = (struct intcpa*)malloc(sizeof *intr);
    intr->assign = false;
    intr->finished = false;
    intr->rsp = 0;
    intr->macro = NULL;
    
    /* Copy parameters */
    memcpy(&intr->i_opt, param, sizeof(struct intcpa_param));
    
    /* Clear tables */
    memset(&intr->condition, 0, sizeof(intcpa_userfunc) * intcpa_n_conditions);
    memset(&intr->function, 0, sizeof(intcpa_userfunc) * intcpa_n_functions);
    memset(&intr->dsg, 0, sizeof intr->dsg);
    memset(&intr->rstack, 0, sizeof(struct script_node*) * intcpa_rstack_size);
    
    intcpa_open(intr, original_tree, param);
    
    return intr;
}

INTCPA static inline unsigned intcpa_interpreter_pc(struct intcpa* intr)
{
    return (unsigned)(intr->current - intr->tree);
}

#define intcpa_interpreter_step(intr) intcpa_advance(intr)

#endif /* intcpa_h */
