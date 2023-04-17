//
//  fnTreeUtils.c
//  r3-tas
//
//  Created by Jba03 on 2023-04-17.
//

#include "memory.h"
#include "tables.h"
#include "stTreeInterpret.h"

bool fnIsEndOfTree(tdstNodeInterpret *tree)
{
    return (tree->type == 8 || tree->depth <= 0);
}

int fnTreeGetLength(const tdstNodeInterpret *tree)
{
    if (!tree) return -1;
    const tdstNodeInterpret *first = tree;
    const tdstNodeInterpret *advance = tree;
    
    do advance++;
    while (advance->type != script_node_type_end_macro && advance->depth >= 1);
    
    return (unsigned int)(advance - first) + 1;
}

void fnTreePrintNode(tdstNodeInterpret *node)
{
    uint32 param = node->param;
    printf("Node [%s: %d]\n", R3NodeTypeTable[node->type], param);
}

/* Duplicate a node tree */
tdstNodeInterpret *fnTreeDuplicate(const tdstNodeInterpret *tree)
{
    unsigned length = fnTreeGetLength(tree) * sizeof(tdstNodeInterpret);
    tdstNodeInterpret *copy = (tdstNodeInterpret*)malloc(length);
    memcpy(copy, tree, length);
    
    return copy;
}

void fnTreeSwapByteOrder(tdstNodeInterpret *tree)
{
    while (!fnIsEndOfTree(tree))
    {
        tree->param = host_byteorder_32(tree->param);
        tree++;
    }
}

tdstNodeInterpret *fnMacroGetCurrentTree(tdstMacro *macro)
{
    if (!macro) return NULL;
    tdstTreeInterpret *tree = (tdstTreeInterpret*)pointer(macro->script_current);
    tdstNodeInterpret *node = (tdstNodeInterpret*)pointer(tree->tree);
    return node;
}

const char* fnMacroGetName(const tdstMacro *macro)
{
    if (!macro) return NULL;
    const char* name = (const char*)memchr(macro->name, ':', 0x100);
    return name ? name + 1 : NULL;
}
