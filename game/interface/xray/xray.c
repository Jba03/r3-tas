//
//  xray.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-27.
//

#include "xray.h"
#include "dynamics.h"
#include "collide_object.h"
#include "octree.h"
#include "ipo.h"
#include "game.h"

#include <stdio.h>

static const struct vector3 xray_player_pos()
{
    const struct matrix4 T = actor_matrix(actor_rayman);
    return game_matrix4_position(T);
}

static const struct vector3 xray_get_destination(struct xray* h)
{
    return vector3_new(0.0f, 0.0f, 0.0f);
}

static bool xray_player_grounded()
{
    const struct dynamics* dynamics = actor_dynamics(actor_rayman);
    return dynamics_collide_with(dynamics, dynamics_obstacle_ground);
}

static struct collide_object* xray_nearest_platform()
{
    
    
    return NULL;
}

static struct octree_node* xray_current_octree_node(struct xray* h)
{
    
    
    return NULL;
}

static void xray_cornercut(struct xray* h)
{
    
}

static void xray_output(struct xray* h, struct xray_output* out)
{
    
}

#pragma mark - Route common

static struct xray_route* xray_route_alloc(struct xray* h)
{
    struct xray_route* new = malloc(sizeof *new);
    h->current_route = new;
    h->routes[h->n_routes++] = new;
    return new;
}
 
#pragma mark - Route octree

/* xray_recursive_find_octrees: find appropriate octree nodes */
static void xray_route_recursive_get_octrees(struct xray* h, struct superobject* root)
{
    struct xray_route* route = h->current_route;
    
    if (superobject_type(root) == superobject_type_ipo)
    {
        const struct ipo* ipo = superobject_data(root);
        if (!ipo) return;
        
        /* Get the reaction zone */
        const struct collide_object* zdr = ipo_collide_object(ipo);
        if (zdr)
        {
            const struct octree* octree = pointer(zdr->octree);
            if (octree)
            {
                const struct octree_node* node = pointer(octree->root);
                if (node)
                {
                    route->octree[route->n_octree_nodes] = node;
                    route->octree_ipo[route->n_octree_nodes] = root;
                    route->n_octree_nodes++;
                }
            }
        }
    }
    
    superobject_for_each(root, so)
    {
        xray_route_recursive_get_octrees(h, so);
    };
}

static void xray_route_get_octrees(struct xray* h)
{
    struct xray_route* route = h->current_route;
    
    route->n_octree_nodes = 0;
    memset(route->octree, 0, sizeof(struct octree_node*) * XRAY_MAX_OCTREE_NODES);
    memset(route->octree_ipo, 0, sizeof(struct superobject*) * XRAY_MAX_OCTREE_NODES);
    
    xray_route_recursive_get_octrees(h, father_sector);
}

#pragma mark - Route pointset

static void xray_recursive_derive_pointset(struct xray* h, struct superobject* root, const struct matrix4 transform, const float z)
{
    int mesh_index = 0;
    const struct collide_mesh* mesh = NULL;
    
    const struct matrix4 T = matrix4_mul(superobject_matrix_global(root), transform);
    
    if (superobject_type(root) == superobject_type_ipo)
    {
        const struct ipo* ipo = superobject_data(root);
        if (!ipo) return;
        
        /* Get the reaction zone */
        const struct collide_object* zdr = ipo_collide_object(ipo);
        if (zdr)
        {
            while ((mesh = collide_object_mesh(zdr, mesh_index)))
            {
                const uint16* indices = (const uint16*)pointer(mesh->face_indices);
                const struct vector3* vertices = (const struct vector3*)pointer(zdr->vertices);
                
                for (int16 index = 0; index < host_byteorder_16(mesh->n_faces) * 3; index++)
                {
                    uint16 idx = host_byteorder_16(*(indices + index));
                    
                    const struct vector3 v = vector3_host_byteorder(*(vertices + idx));
                    const struct vector4 result = vector4_mul_matrix4(vector4_new(v.x, v.y, v.z, 1.0f), T);
                    
                    struct xray_node node;
                    node.position = vector3_new(result.x, result.y, result.z);
                    node.superobject = root;
                    
                    h->nodes[h->n_nodes++] = node;
                }
                
                mesh_index++;
            }
        }
    }
    
    superobject_for_each(root, so)
    {
        xray_recursive_derive_pointset(h, so, T, z);
    };
}

static void xray_derive_pointset(struct xray* h, const float z)
{
//    struct xray_route* route = h->current_route;
//    if (route->n_octree_nodes <= 0)
//    {
//        fprintf(stderr, "[xray] failed to derive pointset: no octrees loaded!");
//    }
    
    h->n_lines = 0;
    
    xray_recursive_derive_pointset(h, father_sector, matrix4_identity, z);
}

#pragma mark - Route pathfinding

static void xray_route_theta_vertex_update(struct xray* h, struct xray_node* s, struct xray_node* neighbor)
{
    if ( /* line of sight */ 1)
    {
        
    }
    else
    {
        
    }
}

static void xray_route_theta(struct xray* h)
{
    struct xray_route* route = h->current_route;
    
    memset(route->open_set, 0, sizeof(struct xray_node) * XRAY_MAX_THETA_NODES);
    memset(route->closed_set, 0, sizeof(struct xray_node) * XRAY_MAX_THETA_NODES);
    
    /* Initialize first node. */
    route->open_set[0].position = route->source;
    route->open_set[0].parent = &route->open_set[0];
    route->open_set[0].cost = 0.0f;
    route->open_set[0].in_open = true;
    route->n_open_set_nodes++;
    
    while (route->n_open_set_nodes != 0)
    {
        struct xray_node* s = &route->open_set[route->n_open_set_nodes--];
        /* Check if we are at the destination node */
        if (s->position.x == route->destination.x && s->position.y == route->destination.y && s->position.z == route->destination.z)
        {
            // Reconstruct
        }
        
        s->in_closed = true;
        route->closed_set[route->n_closed_set_nodes] = *s;
        
        for (unsigned n = 0; n < 26; n++)
        {
            struct xray_node* neighbor = s->neighbors[n];
            if (!neighbor->in_closed)
            {
                if (!neighbor->in_open)
                {
                    neighbor->cost = INFINITY;
                    neighbor->parent = NULL;
                }
                
                // Update vertex
            }
        }
    }
}

#pragma mark - Route generation

/* Generate a route that is walkable (rolling included). */
static void xray_gen_route_walk(struct xray* h)
{
    struct xray_route* route = xray_route_alloc(h);
    
    xray_route_get_octrees(h);
}

/* Generate a route that is jumpable. */
static void xray_gen_route_jump(struct xray* h)
{
    struct xray_route* route = xray_route_alloc(h);
}

/* Generate a route that is hoverable. */
static void xray_gen_route_hover(struct xray* h)
{
    struct xray_route* route = xray_route_alloc(h);
}

/* Generate a route that is swimmable. */
static void xray_gen_route_swim(struct xray* h)
{
    struct xray_route* route = xray_route_alloc(h);
}

#pragma mark - xray

/** xray_analyse: analyse the scene and generate an optimal route to the specified destination. */
void xray_analyse(struct xray* h, const struct vector3 destination)
{
    const struct vector3 pos = xray_player_pos();
    
}

void xray_frame(struct xray* h, struct xray_output* output)
{
    if (xray_player_grounded(h))
    {
        
        
        /* Player is walking on ground */
//        if (h->target)
//        {
//
//        }
//        else
//        {
//
//        }
    }
    else
    {
        
    }
}

void xray_init(struct xray* h)
{
    xray_derive_pointset(h, 0.75f);
}
