//
//  intelligence.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-06.
//

#include "stIntelligence.h"
#include "stSuperObject.h"
#include "stMind.h"
#include "stTreeInterpret.h"

struct intcpa
{
    tdstSuperObject* actor;
    /* Mind of current actor */
    tdstMind* mind;
};

#define advance() intcpa_tree(h);
//
//tdstNodeInterpret *intcpa_tree(struct intcpa *h)
//{
//
//}
//
//int intcpa_init(struct intcpa *h)
//{
//
//}
