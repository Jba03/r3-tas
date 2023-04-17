#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "tables.h"
#include "memory.h"
#include "stTreeInterpret.h"

#include "stDsg.h"

#define IsEndOfTree(Node) (Node->type == script_node_type_end_macro)

#define fetch() (++c->frame->current)


#pragma mark - Interpreter

static tdstTreeInterpretFrame *fnInterpretMakeFrame(const tdstNodeInterpret *tree)
{
    tdstTreeInterpretFrame *frame = (tdstTreeInterpretFrame*)malloc(sizeof *frame);
    frame->duplicateTree = fnTreeDuplicate(tree);
    frame->originalTree = tree;
    frame->current = frame->duplicateTree - 1;
    frame->name = "New Frame";
    
    // Swap tree byteorder
    fnTreeSwapByteOrder(frame->duplicateTree);
    
    return frame;
}

static void fnInterpretPushFrame(tdstTreeInterpretContext *c, tdstTreeInterpretFrame *frame)
{
    if (c->currentFrameIndex + 1 > INTERPRETER_FRAME_COUNT)
    {
        fprintf(stderr, "cannot push context frame \"%s\": max frame count reached\n", frame->name);
        return;
    }
    
    c->frame = frame;
    c->frameStack[++c->currentFrameIndex] = frame;
    printf("PUSH [%s] (%d)\n", frame->name, c->currentFrameIndex);
}

static tdstTreeInterpretFrame *fnInterpretPopFrame(tdstTreeInterpretContext *c)
{
    printf("POP [%s] (%d)\n", c->frame->name, c->currentFrameIndex-1);
    
    if (c->currentFrameIndex - 1 < 0)
    {
        fprintf(stderr, "last context frame popped; program finished.\n");
        c->finished = true;
        return c->frame = c->frameStack[0];
    }
    
    return c->frame = c->frameStack[--c->currentFrameIndex];
}

static void fnTreeInterpreterDefaultOptions(tdstTreeInterpretOptions *opt)
{
}

static tdstNodeInterpret *fnTreeFindNextOfDepth(tdstTreeInterpretContext *c, unsigned depth)
{
    tdstNodeInterpret *next = c->frame->current + 1;
    while (!IsEndOfTree(next) && next->depth > depth) ++next;
    return next;
}

static void fnTreeSeekDepth(tdstTreeInterpretContext *c, unsigned depth)
{
    c->frame->current = fnTreeFindNextOfDepth(c, depth) - 1;
}

#define op() fnTreeInterpret(c)
#define seeknext() { c->frame->current = current; fnTreeSeekNext(c); }

#define lval(nd)    *((int32*)&nd->param)
#define ulval(nd)   *((uint32*)&nd->param)
#define rval(nd)    *((float*)&nd->param)

static tdstNodeInterpret *fnTreeInterpret(tdstTreeInterpretContext *c);

static tdstNodeInterpret *fnTreeInterpretException(tdstTreeInterpretContext *c, tdstNodeInterpret *node, const char* fmt, ...)
{
    long pc = node - c->frame->duplicateTree + 1;
    
    va_list args;
    va_start(args, fmt);
    printf("exception (PC=%ld): ", pc);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
    
    return c->frame->current;
}

static const uint32 fnTreeInterpretGetGlobalRandomizer(tdstTreeInterpretContext *c)
{
    return c->globals ? c->globals->randomizer : 0;
}

static tdstNodeInterpret *fnTreeInterpretKeyword(tdstTreeInterpretContext *c)
{
    tdstNodeInterpret *keyword = c->frame->current;
    
    if (ulval(keyword) >= 0 && ulval(keyword) <= 13) // If, IfNot, IfX, IfNotX
    {
        bool x = true;
        // Fetch condition (which is not always a condition!)
        tdstNodeInterpret *condition = op();
        // For framerule-dependent conditional keywords
        unsigned p = fnTreeInterpretGetGlobalRandomizer(c) % (1 << (ulval(keyword) - 1));
        
        if (ulval(keyword) == 0) x = ulval(condition) == 1; // If
        if (ulval(keyword) == 1) x = ulval(condition) == 0; // IfNot
        if (ulval(keyword) >= 2 && ulval(keyword) <=  7) x = p == 0; // IfX
        if (ulval(keyword) >= 8 && ulval(keyword) <= 13) x = p != 0; // IfNotX
        
        // After the condition is evaluated, a `Then` statement should always
        // follow, with the exception of a by the user badly designed program.
        // TODO: Check for this exception?
        
        // Find `Then`
        c->frame->current = keyword;
        tdstNodeInterpret *Then = fnTreeFindNextOfDepth(c, keyword->depth);
        if (Then->type != 0 || Then->param != 16)
            return fnTreeInterpretException(c, Then, "expected 'then' node");
        c->frame->current = Then - 1;
        
        if (x) // Condition is true
        {
            fetch();
            // Find else statement, if it exists.
            tdstNodeInterpret *Else = fnTreeFindNextOfDepth(c, Then->depth);
            if (Else->type == 0 && Else->param == 17)
            {
                Else->param = INTERPRETER_TAG_SKIP;
            }
        }
        else // Condition is false
        {
            fetch();
            fnTreeSeekDepth(c, Then->depth);
        }
    }
    
    if (ulval(keyword) == INTERPRETER_TAG_SKIP)
    {
        fetch();
        fnTreeSeekDepth(c, keyword->depth);
    }
    
    return keyword;
}

static tdstNodeInterpret *fnTreeInterpretCondition(tdstTreeInterpretContext *c)
{
    tdstNodeInterpret *condition = c->frame->current;
    
    #pragma mark Boolean condition
    if (ulval(condition) >= 0 && ulval(condition) <= 3)
    {
        tdstNodeInterpret *A = op();
        tdstNodeInterpret *B = op();
        
//        if ((ulval(A) != 12 || ulval(B) != 12) && (ulval(A) != 1 || ulval(B) != 1))
//            return fnTreeInterpretException(c, condition, "boolean condition on non-integer value");
        
        if (ulval(condition) == 0) ulval(condition) = ulval(op()) == ulval(op()); // And
        if (ulval(condition) == 1) ulval(condition) = ulval(op()) || ulval(op()); // Or
        if (ulval(condition) == 2) ulval(condition) =               !ulval(op()); // Not
        if (ulval(condition) == 3) ulval(condition) = ulval(op())  ^ ulval(op()); // XOr
    }
    
    #pragma mark Comparison condition
    if (ulval(condition) >= 4 && ulval(condition) <= 9)
    {
        tdstNodeInterpret *A = op();
        tdstNodeInterpret *B = op();
        
//        if (ulval(A) != 12 || ulval(A) != 13 || ulval(B) != 12 || ulval(B) != 13)
//            return fnTreeInterpretException(c, condition, "cannot perform comparison on non-numeric operand");
        
        float x = 0.0f;
        float y = 0.0f;
        
        // Convert the types to floating point, so that they can be compared
        // in a single statement. Don't know how this affects precision though.
        if (A->type == 12) x = (float)ulval(A);
        if (A->type == 13) x = rval(A);
        if (B->type == 12) y = (float)ulval(A);
        if (B->type == 13) y = rval(A);
               
        printf("compare: %f(%X, %d) {%d} %f(%X, %d)\n", x, A->param, A->type, condition->param, y, A->param, B->type);
               
        if (ulval(condition) == 4) ulval(condition) = x == y;
        if (ulval(condition) == 5) ulval(condition) = x != y;
        if (ulval(condition) == 6) ulval(condition) = x  < y;
        if (ulval(condition) == 7) ulval(condition) = x  > y;
        if (ulval(condition) == 8) ulval(condition) = x <= y;
        if (ulval(condition) == 9) ulval(condition) = x >= y;
        
        //ulval(condition) = 1;
    }
    
    #pragma mark Extern condition
    if (ulval(condition) >= 10)
    {
        if (c->condition) c->condition(c);
        
        c->frame->current = condition;
        // Seek to the next statement
        fnTreeSeekDepth(c, condition->depth);
    }
    
    return condition;
}

static tdstNodeInterpret *fnTreeInterpretOperator(tdstTreeInterpretContext *c)
{
    tdstNodeInterpret *op = c->frame->current;
    
    #pragma mark Add, subtract, multiply, divide
    if (ulval(op) <= 3)
    {
        tdstNodeInterpret *A = op();
        tdstNodeInterpret *B = op();
        
//        #define arithmeticOP(t,a) \
//        if (ulval(op) == t && A->type == 12 && B->type == 12) op->param = lval(A) a lval(B); \
//        if (ulval(op) == t && A->type == 12 && B->type == 13) op->param = lval(A) a rval(B); \
//        if (ulval(op) == t && A->type == 13 && B->type == 12) op->param = rval(A) a lval(B); \
//        if (ulval(op) == t && A->type == 13 && B->type == 13) op->param = rval(A) a rval(B); \
//
//        arithmeticOP(0, +)
//        arithmeticOP(1, -)
//        arithmeticOP(2, *)
//        arithmeticOP(3, /)
    }
    
    #pragma mark Negate
    if (op->param == 4)
    {
        tdstNodeInterpret *A = op();
        if (A->type == 12) lval(op) = -lval(A);
        if (A->type == 13) *((float*)&op->param) = -rval(A);
    }
    
    #pragma mark Modulo
    if (op->param == 5)
    {
        tdstNodeInterpret *A = op();
        tdstNodeInterpret *B = op();
        if (A->type == 12 && B->type == 12) lval(op) = lval(A) % lval(B);
    }
    
    #pragma mark Affect and assign
    if (op->param >= 6 && op->param <= 9)
    {
        tdstNodeInterpret *A = op();
        tdstNodeInterpret *B = op();
    }
    
    #pragma mark Increment and decrement
    if (op->param >= 10 && op->param <= 11)
    {
        tdstNodeInterpret *A = op();
    }
    
    #pragma mark Assign
    if (op->param == 12)
    {
        
    }
    
    #pragma mark Dot
    if (op->param == 13)
    {
        if (op()->type != script_node_type_actorref)
            return fnTreeInterpretException(c, op, "attempt to access data of invalid operand (not an actor reference)");
        
        tdstNodeInterpret *B = op();
        lval(op) = lval(B);
        
        c->actorReference = NULL;
    }
    
    // Vector negation operator
    if (op->param == 19)
    {
        tdstNodeInterpret *A = op();
        
        if (A->type != script_node_type_vector && A->type != script_node_type_constant_vector && A->type != script_node_type_function)
            return fnTreeInterpretException(c, op, "attempt to perform vector negation on non-vector operand");
    }
    
    return op;
}

static tdstNodeInterpret *fnTreeInterpretFunction(tdstTreeInterpretContext *c)
{
    tdstNodeInterpret *function = c->frame->current;
    if (function->type == 3 && c->function) c->function(c);
    if (function->type == 4 && c->procedure) c->procedure(c);
    
    c->frame->current = function;
    // Seek to the next statement
    fnTreeSeekDepth(c, function->depth);
    
    return function;
}

static void fnTreeInterpretConvertDsgVar(tdstNodeInterpret *dsgVarRef, tdstEngineObject *actor)
{
    uint8 type = 0;
    void* data = fnActorGetDsgVar(actor, ulval(dsgVarRef), &type);
    if (!data) return;
    
    printf("reading dsgvar_%d of \"%s\"\n", ulval(dsgVarRef), fnActorGetName(actor_instance_name, actor, objectType));
    
    if (type <= 6) dsgVarRef->type = 12; // Constant
    if (type == 7) dsgVarRef->type = 13; // Real
    
    switch (type)
    {
        case dsgvar_type_boolean:   ulval(dsgVarRef) = *(uint8 *)data; break;
        case dsgvar_type_byte:       lval(dsgVarRef) = *(int8  *)data; break;
        case dsgvar_type_ubyte:     ulval(dsgVarRef) = *(uint8 *)data; break;
        case dsgvar_type_short:      lval(dsgVarRef) = *(int16 *)data; break;
        case dsgvar_type_ushort:    ulval(dsgVarRef) = *(uint16*)data; break;
        case dsgvar_type_int:        lval(dsgVarRef) = *(int32 *)data; break;
        case dsgvar_type_uint:      ulval(dsgVarRef) = *(uint32*)data; break;
        case dsgvar_type_float:      rval(dsgVarRef) = game_byteorder_f32(host_byteorder_32(*(float32*)data)); break;
    }
    
    ulval(dsgVarRef) = host_byteorder_32(ulval(dsgVarRef));
}

static tdstNodeInterpret *fnTreeInterpretDsgVarRef(tdstTreeInterpretContext *c)
{
    tdstNodeInterpret *dsgVarRef = c->frame->current;
    if (!c->isAssignment && (c->dsgAccessMode & INTERPRETER_DSG_READ))
    {
        if (!c->actorReference && c->owner)
            fnTreeInterpretConvertDsgVar(dsgVarRef, c->owner);
        else if (c->actorReference)
            fnTreeInterpretConvertDsgVar(dsgVarRef, c->actorReference);
    }
    else if (c->isAssignment && (c->dsgAccessMode & INTERPRETER_DSG_WRITE))
    {
        
    }
    
    return dsgVarRef;
}

static tdstNodeInterpret *fnTreeInterpretActorRef(tdstTreeInterpretContext *c)
{
    tdstNodeInterpret *actorReference = c->frame->current;
    uint32 offset = game_byteorder_32(actorReference->param);
    
    tdstEngineObject *actor = (tdstEngineObject*)pointer(offset);
    // Null check necessary?
    c->actorReference = actor;
    
    
    return actorReference;
}

static tdstNodeInterpret *fnTreeInterpretSubroutine(tdstTreeInterpretContext *c)
{
    tdstNodeInterpret *subroutine = c->frame->current;

    tdstMacro *macro = (tdstMacro*)pointer(swap32(subroutine->param));
    tdstNodeInterpret *tree = fnMacroGetCurrentTree(macro);
    tdstTreeInterpretFrame *frame = fnInterpretMakeFrame(tree);
    frame->name = fnMacroGetName(macro);
    fnInterpretPushFrame(c, frame);
    
    return subroutine;
}
    
static tdstNodeInterpret *fnTreeInterpret(tdstTreeInterpretContext *c)
{
    if (c->finished) return c->frame->current - 1;
    
    tdstNodeInterpret *current = fetch();
    //fnTreePrintNode(current);
    switch (current->type)
    {
        case script_node_type_keyword: fnTreeInterpretKeyword(c); break;
        case script_node_type_condition: fnTreeInterpretCondition(c); break;
        case script_node_type_operator: fnTreeInterpretOperator(c); break;
        case script_node_type_function:
        case script_node_type_procedure: fnTreeInterpretFunction(c); break;
            
        case script_node_type_end_macro: fnInterpretPopFrame(c); break;
        //case script_node_type_dsgvarref:
        case script_node_type_dsgvarref2: fnTreeInterpretDsgVarRef(c); break;
            
        case script_node_type_actorref: fnTreeInterpretActorRef(c); break;
            
        case script_node_type_subroutine: fnTreeInterpretSubroutine(c); break;
    }
    
//    #define i(t, code) if (type == t) { code; }
//    #define u(t, m, code) if (type == t && param < 37) { if (m[param] == '!') { code; } }
//    u(0, "!!!!!!!!!!!!!!!!                     ", A = op()) // If (%){X}, If !(%){X}
//    //u(0, 0b00111111111111000000000000000000000000000000, y = c->randomizer % (1 << (param - 1))) // If (%), If !(%)
//    //u(0, 0b00111111000000000000000000000000000000000000, condition &= y == 0) // If  (%)
//    //u(0, 0b00000000111111000000000000000000000000000000, condition &= y != 0) // If !(%)
//    u(0, "!!!!!!!!!!!!!!!!                     ", if (!ulval(A)) seeknext() /* Then -> Else */)
//    i(0, if (param == INTERPRETER_TAG_SKIP) { seeknext(); })
//    i(0, if(param==16) { A = fnTreeFindNext(c); if (A->type == 0 && ulval(A) == 17) A->param = game_byteorder_32(INTERPRETER_TAG_SKIP); })
//    u(0, "                !                    ", ) /* Then */
//    u(0, "                 !                   ", ) /* Else */
//
//    #pragma mark Condition
//    i(1, uint8 v = param >= 10; if(v) { call(condition); seeknext(); goto ret; }) // Conditions which are not addressed here
//    u(1, "!!!!!!!!!!                           ", A = op())
//    u(1, "!! !!!!!!!                           ", B = op())
//    u(1, "!                                    ", current->param = ulval(A) && ulval(B)) // Cond_And
//    u(1, " !                                   ", current->param = ulval(A) || ulval(B)) // Cond_Or
//    u(1, "  !                                  ", current->param =            !ulval(A)) // Cond_Not
//    u(1, "   !                                 ", current->param = ulval(A)  ^ ulval(B)) // Cond_XOR
//    u(1, "    !                                ", current->param =  zval(A) ==  zval(B)) // Cond_Equal
//    u(1, "     !                               ", current->param =  zval(A) !=  zval(B)) // Cond_Different
//    u(1, "      !                              ", current->param =  zval(A)  <  zval(B)) // Cond_Lesser
//    u(1, "       !                             ", current->param =  zval(A)  >  zval(B)) // Cond_Greater
//    u(1, "        !                            ", current->param =  zval(A) <=  zval(B)) // Cond_LesserOrEqual
//    u(1, "         !                           ", current->param =  zval(A) >=  zval(B)) // Cond_GreaterOrEqual
//    u(1, "!!!!!!!!!!                           ", current->param = ulval(current))
//    #pragma mark Operator
//    u(2, "!!!!!!!!!!!!!                        ", A = op())
//    u(2, "!!!! !      !                        ", B = op())
//    u(2, "!                                    ", current->param = zval(A) + zval(B))
//    u(2, " !                                   ", current->param = zval(A) - zval(B))
//    u(2, "  !                                  ", current->param = zval(A) * zval(B))
//    u(2, "   !                                 ", current->param = zval(A) / zval(B))
//    u(2, "    !                                ", current->param =          -zval(A))
//    u(2, "     !                               ", current->param = lval(A) % lval(B))
//    u(2, "      !                              ", A->param       = zval(A) + zval(B))
//    u(2, "       !                             ", A->param       = zval(A) - zval(B))
//    u(2, "        !                            ", A->param       = zval(A) * zval(B))
//    u(2, "         !                           ", A->param       = zval(A) / zval(B))
//    u(2, "          !                          ", A->param       = zval(A) + 1)
//    u(2, "           !                         ", A->param       = zval(A) - 1)
//    i(2, if (A) { if (A->type == 13) A->param = game_byteorder_f32(A->param); })
//    i(2, if (B) { if (B->type == 13) { A->type = 13; A->param = game_byteorder_f32(A->param); }  }) // B exists -> A exists
//    u(2, "!!!!!!!!!!!!!                        ", A->param = ulval(A); current->param = ulval(current))
//
//    /* FUNCTION, PROCEDURE */
//    i(3, call(function); seeknext(); goto ret)
//    i(4, call(procedure); seeknext(); goto ret)
//
//    /* ENDMACRO */
//    i(8 || (depth <= 0), fnInterpretPopFrame(c))
//
//    /* SUBROUTINE */
//    if (type == 41)
//    {
//        tdstMacro *macro = (tdstMacro*)pointer(current->param);
//        tdstNodeInterpret *tree = fnMacroGetCurrentTree(macro);
//        tdstTreeInterpretFrame *frame = fnInterpretMakeFrame(tree);
//        frame->name = fnMacroGetName(macro);
//
//        fnInterpretPushFrame(c, frame);
//    }
//
//    #undef u
    
    return current;
}

int fnTreeInterpreterInit(tdstTreeInterpretContext **ctx, const tdstNodeInterpret *tree, const tdstTreeInterpretOptions *opt)
{
    tdstTreeInterpretContext *c = *ctx = (tdstTreeInterpretContext*)malloc(sizeof(*c));
    
    c->frame = NULL;
    c->currentFrameIndex = -1;
    c->globals = NULL;
    c->isAssignment = false;
    c->actorReference = NULL;
    c->owner = NULL;
    
    c->dsgAccessMode = INTERPRETER_DSG_READ;
    
    tdstTreeInterpretFrame *frame = fnInterpretMakeFrame(tree);
    fnInterpretPushFrame(c, frame);
    
    return 1;
}

void fnTreeInterpreterStep(tdstTreeInterpretContext *c)
{
    fnTreeInterpret(c);
}

long fnTreeInterpreterGetPC(tdstTreeInterpretContext *c)
{
    tdstTreeInterpretFrame *frame = c->frame;
    return frame->current - frame->duplicateTree + 1;
}
