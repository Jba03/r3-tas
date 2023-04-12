//
//  fnTreeInterpret.c
//  r3-tas-memorymap
//
//  Created by Jba03 on 2023-04-02.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stTreeInterpret.h"

typedef struct stTreeInterpreterOptions {
    bool mirrorTree;
    bool writeDSG;
} tdstTreeInterpreterOptions;

typedef struct stTreeInterpreterContext {
    const tdstNodeInterpret *originalTree;
    const tdstNodeInterpret *mirroredTree;
    tdstNodeInterpret *current;
    tdstTreeInterpreterOptions opt;
    
    uint32 randomizer;
} tdstTreeInterpreterContext;

static void fnTreeInterpreterDefaultOptions(tdstTreeInterpreterOptions *opt)
{
    opt->mirrorTree = true;
    opt->writeDSG = false;
}


static tdstNodeInterpret *fnTreeInterpret(tdstTreeInterpreterContext *c)
{
    tdstNodeInterpret* current = c->current;
    
    uint32 param = host_byteorder_32(current->param);
    
    tdstTreeInterpret *node = NULL;
    uint32 x, y;
    bool b;
    
//    #define u(t,m,code) if (t == current->type) { code; }
//    u(0, "!!!!!!!!!!!!!!                          ", b = fnTreeInterpret(c)->param)
//    u(0, "!!                                      ", b = fnTreeInterpret(c)->param)
//    u(0, "  !!!!!!!!!!!!                          ", y = c->randomizer % (1 << (param - 1)))
//    u(0, "  !!!!!!                                ", b &= y == 0) // IfX
//    u(0, "        !!!!!!                          ", b &= y != 0) // IfNotX
//    u(0, "!!!!!!!!!!!!!!                          ", fnTreeSeekNext(c); if (!b) fnTreeSeekNext(c))
//    #undef u
    
    return current;
}

static int fnTreeInterpreterInit(tdstTreeInterpreterContext **ctx, const tdstNodeInterpret *tree, const tdstTreeInterpreterOptions *opt)
{
    tdstTreeInterpreterContext *c = *ctx = (tdstTreeInterpreterContext*)malloc(sizeof(*c));
    c->originalTree = tree;
    
    if (opt)
        c->opt = *opt;
    else
        fnTreeInterpreterDefaultOptions(&c->opt);
    
    
    
    
    return 1;
}
