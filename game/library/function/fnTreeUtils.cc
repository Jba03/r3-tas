//
//  fnTreeUtils.c
//  r3-tas
//
//  Created by Jba03 on 2023-04-17.
//

#include "memory.hh"
#include "tables.hh"
#include "stTreeInterpret.hh"

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

void fnTreeInsertNode(tdstNodeInterpret **root, tdstNodeInterpret *parent, tdstNodeInterpret *insert)
{
    unsigned newLength = fnTreeGetLength(*root) + 1;
    *root = realloc(*root, sizeof(tdstNodeInterpret) * newLength);
    
    tdstNodeInterpret *iterator = *root;
    /* Seek to the end of the tree */
    while (!fnIsEndOfTree(iterator)) iterator++;
    /* Shift all nodes prior to the parent backwards */
    while (iterator != parent) *(iterator + 1) = *iterator;
    
    *iterator = *insert;
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
