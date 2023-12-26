//
//  fnTreeUtils.c
//  r3-tas
//
//  Created by Jba03 on 2023-04-17.
//

#include "memory.hh"
#include "tables.hh"
#include "stTreeInterpret.hh"

bool fnIsEndOfTree(stNodeInterpret *tree)
{
    return (tree->type == 8 || tree->depth <= 0);
}

int fnTreeGetLength(const stNodeInterpret *tree)
{
    if (!tree) return -1;
    const stNodeInterpret *first = tree;
    const stNodeInterpret *advance = tree;
    
    do advance++;
    while (advance->type != script_node_type_end_macro && advance->depth >= 1);
    
    return (unsigned int)(advance - first) + 1;
}

void fnTreePrintNode(stNodeInterpret *node)
{
    uint32 param = node->param;
    printf("Node [%s: %d]\n", R3NodeTypeTable[node->type], param);
}

/* Duplicate a node tree */
stNodeInterpret *fnTreeDuplicate(const stNodeInterpret *tree)
{
    unsigned length = fnTreeGetLength(tree) * sizeof(stNodeInterpret);
    stNodeInterpret *copy = (stNodeInterpret*)malloc(length);
    memcpy(copy, tree, length);
    
    return copy;
}

void fnTreeInsertNode(stNodeInterpret **root, stNodeInterpret *parent, stNodeInterpret *insert)
{
    unsigned newLength = fnTreeGetLength(*root) + 1;
    *root = realloc(*root, sizeof(stNodeInterpret) * newLength);
    
    stNodeInterpret *iterator = *root;
    /* Seek to the end of the tree */
    while (!fnIsEndOfTree(iterator)) iterator++;
    /* Shift all nodes prior to the parent backwards */
    while (iterator != parent) *(iterator + 1) = *iterator;
    
    *iterator = *insert;
}

void fnTreeSwapByteOrder(stNodeInterpret *tree)
{
    while (!fnIsEndOfTree(tree))
    {
        tree->param = host_byteorder_32(tree->param);
        tree++;
    }
}

stNodeInterpret *fnMacroGetCurrentTree(stMacro *macro)
{
    if (!macro) return NULL;
    stTreeInterpret *tree = (stTreeInterpret*)pointer(macro->script_current);
    stNodeInterpret *node = (stNodeInterpret*)pointer(tree->tree);
    return node;
}

const char* fnMacroGetName(const stMacro *macro)
{
    if (!macro) return NULL;
    const char* name = (const char*)memchr(macro->name, ':', 0x100);
    return name ? name + 1 : NULL;
}
