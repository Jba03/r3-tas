//
//  intelligence.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-06.
//

#include "intelligence.h"
#include "superobject.h"
#include "mind.h"
#include "script.h"

struct intcpa
{
    struct superobject* actor;
    /* Mind of current actor */
    struct mind* mind;
};

#define advance() intcpa_tree(h);
//
//struct script_node *intcpa_tree(struct intcpa *h)
//{
//
//}
//
//int intcpa_init(struct intcpa *h)
//{
//
//}
