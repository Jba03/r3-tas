//
//  octree.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-04.
//

#include "octree.h"
#include "intersect.h"
#include "game.h"

static void octree_node_select(struct octree_node* node,
                            struct octree_node** selected,
                            int *n_selected,
                            float *st,
                            float t)
{
    int index = *n_selected;
    while (0 < index && t < st[index - 1])
    {
        if (index < OCTREE_MAX_SELECTED_NODES)
        {
            selected[index] = selected[index - 1];
            st[index] = st[index - 1];
        }
        --index;
    }
    
    if (index < OCTREE_MAX_SELECTED_NODES)
    {
        selected[index] = node;
        st[index] = t;
        if (*n_selected < OCTREE_MAX_SELECTED_NODES) (*n_selected)++;
    }
}

/* octree_traverse_line_segment: traverse an octree with a line segment. AB is a vector from point A.  */
void octree_traverse_line_segment(const struct octree_node* node,
                                  const struct matrix4 octree_transform,
                                  const struct vector3 A,
                                  const struct vector3 AB,
                                  struct octree_node** selected,
                                  int *n_selected,
                                  float *st)
{
    if (!node) return;
    
    float t;
    
    struct vector3 min = vector3_host_byteorder(node->min);
    struct vector3 max = vector3_host_byteorder(node->max);
    struct vector4 min4 = vector4_mul_matrix4(vector4_new(min.x, min.y, min.z, 1.0f), octree_transform);
    struct vector4 max4 = vector4_mul_matrix4(vector4_new(max.x, max.y, max.z, 1.0f), octree_transform);
    min = vector3_new(min4.x, min4.y, min4.z);
    max = vector3_new(max4.x, max4.y, max4.z);
    
    if (intersect_box_line_segment(min, max, A, AB, &t))
    {
        const pointer* childlist = (const pointer*)pointer(node->children);
        if (childlist)
        {
            for (int i = 0; i < 8; i++)
            {
                const struct octree_node* node = (const struct octree_node*)pointer(*(childlist + i));
                octree_traverse_line_segment(node, octree_transform, A, AB, selected, n_selected, st);
            }
        }
        else
        {
            const pointer* face_indices = pointer(node->face_indices);
            if (face_indices) octree_node_select((struct octree_node*)node, selected, n_selected, st, t);
        }
    }
}
