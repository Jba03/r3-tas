//
//  script.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-27.
//

#include "script.h"
#include "tables.h"

#pragma mark - Translation

#define BUFFER_SIZE 8192
#define SCRIPT_MAX_DEPTH 256

struct Translation {
    char* buf;
    char* ptr;
    struct ScriptNode* current;
    int indent;
    int line;
    int col;
};

#define append(...) t->ptr += sprintf(t->ptr, __VA_ARGS__)
#define cmp(a, b, ...) if (!strcmp(a,b)) { t->ptr += sprintf(t->ptr, __VA_ARGS__); return; }
#define first_child ((struct ScriptNode*)array_get(t->current->children, 0))
#define second_child ((struct ScriptNode*)array_get(t->current->children, 1))
#define third_child ((struct ScriptNode*)array_get(t->current->children, 2))

static const char* tostring(struct ScriptNode* node);
static const char* tostring_all(struct ScriptNode* node, bool separate_arguments);

static int indent = 0;

static void keyword(struct Translation* t)
{
    if (t->current->param >= script_keyword_table_length)
    {
        error("keyword index out of table range\n");
        return;
    }
    
    const char* keyword = script_keyword_table[t->current->param];
    
    int imod = -1, nmod = -1;
    
    cmp(keyword, "If",          "if %s",              tostring(first_child))
    cmp(keyword, "IfNot",       "if !(%s)",           tostring(first_child))
    cmp(keyword, "Then",        "{\n%s%*s}",            tostring_all(t->current, false), indent * 4, "")
    cmp(keyword, "Else",        "else\n%*s{\n%s%*s}\n", indent * 4, "", tostring_all(t->current, false), indent * 4, "")
    cmp(keyword, "IfDebug",     "if (debug && %s)",     tostring(first_child))
    cmp(keyword, "IfNotU64",    "if (!u64) {\n%s\n}\n", tostring_all(t->current, false))
    if (sscanf(keyword, "If%d", &imod)    > 0) append("if (randomizer %% %d == 0 && (%s))", imod, tostring(first_child));
    if (sscanf(keyword, "IfNot%d", &nmod) > 0) append("if (randomizer %% %d != 0 && (%s))", nmod, tostring(first_child));
        t->ptr += sprintf(t->ptr, "%s", keyword);
}

static void condition(struct Translation* t)
{
    const char* condition = script_condition_table[t->current->param];
    
    cmp(condition, "Cond_And",              "%s && %s", tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_Or",               "%s || %s", tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_Not",              "!(%s)",    tostring(first_child)                        )
    cmp(condition, "Cond_XOR",              "%s ^ %s",  tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_Equal",            "%s == %s", tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_Different",        "%s != %s", tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_Lesser",           "%s < %s",  tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_Greater",          "%s > %s",  tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_LesserOrEqual",    "%s <= %s", tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_GreaterOrEqual",   "%s >= %s", tostring(first_child), tostring(second_child))
    
    t->ptr += sprintf(t->ptr, "%s(%s)", condition, tostring(first_child));
}

static void constant(struct Translation* t)
{
    t->ptr += sprintf(t->ptr, "%i", t->current->param);
}

static void real(struct Translation* t)
{
    t->ptr += sprintf(t->ptr, "%f", (float)t->current->param);
}

static void vector(struct Translation* t)
{
    /* do all script vectors have 3 components? */
    t->ptr += sprintf(t->ptr, "(%s, %s, %s)", tostring(first_child), tostring(second_child), tostring(third_child));
}

static void operator(struct Translation* t)
{
    const char* operator = script_operator_table[t->current->param];
    
    cmp(operator, "Operator_Plus",              "(%s %c %s)",   tostring(first_child), first_child->param >= 0 ? '+' : ' ', tostring(second_child)) /* add */
    cmp(operator, "Operator_Minus",             "(%s - %s)",    tostring(first_child), tostring(second_child)) /* subtract */
    cmp(operator, "Operator_UnaryMinus",        "-%s",          tostring(first_child)                        ) /* negate */
    cmp(operator, "Operator_Mul",               "(%s * %s)",    tostring(first_child), tostring(second_child)) /* multiply */
    cmp(operator, "Operator_Div",               "(%s / %s)",    tostring(first_child), tostring(second_child)) /* divide */
    cmp(operator, "Operator_Mod",               "(%s %% %s)",   tostring(first_child), tostring(second_child)) /* modulo */
    cmp(operator, "Operator_Affect",            "%s = %s;",     tostring(first_child), tostring(second_child)) /* assign */
    cmp(operator, "Operator_PlusAffect",        "%s += %s;",    tostring(first_child), tostring(second_child)) /* add and assign */
    cmp(operator, "Operator_PlusPlusAffect",    "%s++;",        tostring(first_child)                        ) /* increment */
    cmp(operator, "Operator_MinusAffect",       "%s -= %s",     tostring(first_child), tostring(second_child)) /* subtract and assign */
    cmp(operator, "Operator_MinusMinusAffect",  "%s--;",        tostring(first_child)                        ) /* decrement */
    cmp(operator, "Operator_Dot",               "%s.%s",        tostring(first_child), tostring(second_child)) /* field access */
    cmp(operator, ".X",                         "%s.x",         tostring(first_child)                        ) /* vector component X */
    cmp(operator, ".Y",                         "%s.y",         tostring(first_child)                        ) /* vector component Y */
    cmp(operator, ".Z",                         "%s.z",         tostring(first_child)                        ) /* vector component Z */
    cmp(operator, ".X:=",                       "%s.x = %s;",   tostring(first_child), tostring(second_child)) /* assign to vector component X */
    cmp(operator, ".Y:=",                       "%s.y = %s;",   tostring(first_child), tostring(second_child)) /* assign to vector component Y */
    cmp(operator, ".Z:=",                       "%s.z = %s;",   tostring(first_child), tostring(second_child)) /* assign to vector component Z */
    cmp(operator, "Operator_VectorPlusVector",  "%s + %s",      tostring(first_child), tostring(second_child))
    cmp(operator, "Operator_VectorMinusVector", "%s - %s",      tostring(first_child), tostring(second_child))
    cmp(operator, "Operator_VectorMulScalar",   "%s * %s",      tostring(first_child), tostring(second_child))
    cmp(operator, "Operator_VectorDivScalar",   "%s / %s",      tostring(first_child), tostring(second_child))
    cmp(operator, "Operator_VectorUnaryMinus",  "- %s",         tostring(first_child)                        )
    cmp(operator, "Operator_Ultra",             "%s.%s",        tostring(first_child), tostring(second_child))
    cmp(operator, "Operator_Array",             "%s[%s]",       tostring(first_child), tostring(second_child))
    /* type cast - maybe emit this as two consecutive tokens, so that the original type can also be viewed */
    cmp(operator, "Operator_ModelCast",         "((%s)(%s))",   tostring(first_child), tostring(second_child))
    
    append("(%X)(", t->current->param);
    unsigned N = array_element_count(t->current->children);
    for (unsigned i = 0; i < N; i++)
    {
        struct ScriptNode* childnode = array_get(t->current->children, i);
        append("%s", tostring(childnode));
        if (i != N - 1) append("%s", ", ");
    }
    append(");");
}

static void field(struct Translation* t)
{
    const char* field = script_field_table[t->current->param];
    
    if (first_child == NULL)
    {
        append("%s", field);
        return;
    }
    
    append("%s(", field); /* open parameter list */
    
    unsigned N = array_element_count(t->current->children);
    for (unsigned i = 0; i < N; i++)
    {
        struct ScriptNode* childnode = array_get(t->current->children, i);
        append("%s", tostring(childnode));
        if (i != N - 1) append("%s", ", ");
    }
    
    append(")"); /* close parameter list */
}

static void actor_reference(struct Translation* t)
{
    t->ptr += sprintf(t->ptr, "actor(@%X)", t->current->param);
}

static void dsgvar_reference(struct Translation* t)
{
    t->ptr += sprintf(t->ptr, "DsgVar_%d", t->current->param);
}

static void superobject_reference(struct Translation* t)
{
    t->ptr += sprintf(t->ptr, "SuperObject(@%X)", t->current->param);
}

static void function(struct Translation* t)
{
    const char* function = script_function_table[t->current->param];
    
    const unsigned N = array_element_count(t->current->children);
    t->ptr += sprintf(t->ptr, "%s(%s)", function, N > 0 ? tostring_all(t->current, true) : "");
}

static void procedure(struct Translation* t)
{
    const char* proc = script_procedure_table[t->current->param];
    cmp(proc, "Proc_Loop", "for (int i = 0; i < %s; i++) {\n", tostring_all(first_child, false))
    cmp(proc, "Proc_EndLoop", "}\n")
    cmp(proc, "Proc_Break", "break;\n")
    
    if (t->current->children)
    {
        append("%s(", proc); /* open parameter list */
        
        unsigned N = array_element_count(t->current->children);
        for (unsigned i = 0; i < N; i++)
        {
            struct ScriptNode* childnode = array_get(t->current->children, i);
            append("%s", tostring(childnode));
            if (i != N - 1) append("%s", ", ");
        }
        
        append(")"); /* close parameter list */
    }
}

static void macro(struct Translation* t)
{
    t->ptr += sprintf(t->ptr, "macro(@%X)", t->current->param);
}

static const char* tostring(struct ScriptNode* node)
{
    if (!node) return "NULL";
    
    struct Translation t;
    t.current = node;
    t.buf = malloc(BUFFER_SIZE * 10);
    t.ptr = t.buf;
    
    memset(t.buf, 0, BUFFER_SIZE * 10);
    
    switch (node->type)
    {
        case NODE_TYPE_KEYWORD: keyword(&t); break;
        case NODE_TYPE_CONDITION: condition(&t); break;
        case NODE_TYPE_OPERATOR: operator(&t); break;
        case NODE_TYPE_FUNCTION: function(&t); break;
        case NODE_TYPE_PROCEDURE: procedure(&t); break;
        case NODE_TYPE_META_ACTION: return "MetaAction";
        case NODE_TYPE_BEGIN_MACRO: return "Macro";
        case NODE_TYPE_BEGIN_MACRO2: return "Macro2";
        case NODE_TYPE_END_MACRO: return "EndMacro";
        case NODE_TYPE_FIELD: field(&t); break;
        case NODE_TYPE_DSGVARREF: dsgvar_reference(&t); break;
        case NODE_TYPE_DSGVARREF2: dsgvar_reference(&t); break;
        case NODE_TYPE_CONSTANT: constant(&t); break;
        case NODE_TYPE_REAL: real(&t); break;
        case NODE_TYPE_BUTTON: return "Button";
        case NODE_TYPE_CONSTANT_VECTOR:
        case NODE_TYPE_VECTOR: vector(&t); break;
        case NODE_TYPE_MASK: return "Mask";
        case NODE_TYPE_MODULEREF: return "ModuleRef";
        case NODE_TYPE_DSGVARID: return "DsgVarID";
        case NODE_TYPE_STRING: return "String";
        case NODE_TYPE_LIPSSYNCHROREF: return "LipsSynchroRef";
        case NODE_TYPE_FAMILYREF: return "FamilyRef";
        case NODE_TYPE_ACTORREF: actor_reference(&t); break;
        case NODE_TYPE_ACTIONREF: return "ActionRef";
        case NODE_TYPE_SUPEROBJECTREF: superobject_reference(&t); break;
        case NODE_TYPE_UNKNOWN:return "Unk";
        case NODE_TYPE_WAYPOINTREF: return "WaypointRef";
        case NODE_TYPE_TEXTREF: return "TextRef";
        case NODE_TYPE_COMPORTREF: return "ComportRef";
        case NODE_TYPE_MODULEREF2:return "ModuleRef2";
        case NODE_TYPE_SOUNDEVENTREF:return "SoundEventRef";
        case NODE_TYPE_OBJECTTABLEREF:return "ObjectTableRef";
        case NODE_TYPE_GAMEMATERIALREF: return "GameMaterialRef";
        case NODE_TYPE_VISUALMATERIAL: return "VisualMaterial";
        case NODE_TYPE_PARTICLEGENERATOR:return "ParticleGenerator";
        case NODE_TYPE_MODELREF:return "ModelRef";
        case NODE_TYPE_MODELREF2: return "ModelRef2";
        case NODE_TYPE_CUSTOMBITS: return "CustomBits";
        case NODE_TYPE_CAPS: return "Caps";
        case NODE_TYPE_UNKNOWN2: return "Unk2";
        case NODE_TYPE_SUBROUTINE: macro(&t); break;
        case NODE_TYPE_NULL: return "NULL";
        case NODE_TYPE_NULL2: return "NULL.2";
        case NODE_TYPE_GRAPHREF:return "GraphRef";
 
            
        default:
            //warning("[script-translation] invalid node type %X\n", node->type);
            t.ptr += sprintf(t.ptr, "Not yet read.");
            break;
    }
    
    return t.buf;
}

static const char* tostring_all(struct ScriptNode* parent, bool sep)
{
    char buf[BUFFER_SIZE * 10];
    char* ptr = &buf[0];
    
    if (!sep) indent++;
    
    unsigned count = array_element_count(parent->children);
    for (unsigned n = 0; n < count; n++)
    {
        struct ScriptNode* childnode = array_get(parent->children, n);
        if (childnode->depth == parent->depth + 1)
        {
            if (sep)
            {
                ptr += sprintf(ptr, "%s", tostring(childnode));
                if (n != count - 1) ptr += sprintf(ptr, "%s", ", ");
            }
            else
            {
                for (int i = 0; i < indent * 4; i++)
                    ptr += sprintf(ptr, " ");
                
                ptr += sprintf(ptr, "%s\n", tostring(childnode));
            }
        }
    }
    
    if (!sep) indent--;
    
    return strdup(buf);
}

SCRIPT const char* script_translate(struct Script* scpt)
{
    if (!scpt->nodes) return "error: invalid script nodes";
    
    int saved = log_indent;
    log_indent = 0;
    
    indent = 0;
    
    int n = 0;
    int last_depth = 1;
    struct ScriptNode* tree_depth[SCRIPT_MAX_DEPTH];
    memset(tree_depth, 0, sizeof(struct ScriptNode*) * SCRIPT_MAX_DEPTH);
    
    while (1)
    {
        //info("array_get (%d): %d\n", array_element_count(scpt->nodes), n);
        struct ScriptNode* node = array_get(scpt->nodes, n);
        
        node->children = array_create();
        
//        for(int i = 0; i < (node->depth - 1) * 4; i++) printf(" ");
//            info(BOLD COLOR_GREEN "%s (%d)\n", script_nodetype_table[node->type], node->depth);

        if (node->depth > last_depth && node->depth != 0) /* New child node */
            tree_depth[last_depth] = array_get(scpt->nodes, n - 1);
        else if (node->depth == last_depth) /* Parameter or component of parent */
            last_depth--;
        else if (node->depth < last_depth)
            last_depth = node->depth - 1;

        if (node->depth <= 0) /* End of file. */
            break;

        struct ScriptNode* parent = tree_depth[last_depth];
        if (parent)
            array_push(parent->children, node);

        last_depth = node->depth;
        
        n++;
    }
    
    char buf[BUFFER_SIZE];
    char* ptr = buf;
    
    /* Iterate over the top nodes only - the rest of
     * the node tree will be translated recursively. */
    for (unsigned n = 0; n < array_element_count(scpt->nodes); n++)
    {
        struct ScriptNode* node = array_get(scpt->nodes, n);
        if (node)
            if (node->depth == 1)
                //printf("%s\n", script_nodetype_table[node->type]);
                //info(BOLD COLOR_PINK "%s\n", tostring(node));
                ptr += sprintf(ptr, "%s\n", tostring(node));
    }
    
    log_indent = saved;
    
    return strdup(buf);
}

#pragma mark - Script

SCRIPT static struct ScriptNode* scriptnode_read(struct Stream* stream)
{
    struct ScriptNode* node = malloc(sizeof *node);
    node->param = read32();
    advance(3); /* TODO: read */
    node->type = read8();
    advance(2);
    node->depth = read8();
    advance(1);
    
    //if (node->type < script_nodetype_table_length)
        //info("Script node: %s\n", script_nodetype_table[node->type]);
    
    node->children = NULL;
    node->line = -1;
    node->col = -1;
    
    return node;
}

SCRIPT struct Script* script_read(const address addr)
{
    if (addr == 0x00) return NULL;
    
    struct Script* script = malloc(sizeof *script);
    script->offset = addr;
    script->ptr_scriptnodes_offset = memory.read_32(addr) & 0xFFFFFFF;
    script->nodes = array_create();
    
    struct Stream* stream = stream_open(script->ptr_scriptnodes_offset);
    while (1)
    {
        struct ScriptNode* node = scriptnode_read(stream);
        array_push(script->nodes, node);
        if (node->depth == 0) break;
    }
    stream_close(stream);
    
    return script;
}
