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
#include "ray.h"

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

static const float xray_node_distance(struct xray_node* a, struct xray_node* b)
{
    return vector3_length(vector3_sub(a->position, b->position));
}

#pragma mark - Route pointset

static const struct superobject* current_sector = NULL;

static void xray_line_generate_points(struct xray *h, const struct vector3 a, const struct vector3 b)
{
    const struct vector3 AB = vector3_sub(b, a);
    const struct vector3 ABn = vector3_normalize(AB);
    const float l = vector3_length(AB);
    
    #define S 2.0f
    for (float d = 0; d < l; d += l / S)
    {
        struct xray_node node;
        node.in_closed = false;
        node.in_open = false;
        node.sector = current_sector;
        node.position = vector3_add(a, vector3_mulf(ABn, d));
        
        h->nodes[h->n_nodes++] = node;
    }
    #undef S
}

static void xray_recursive_derive_pointset(struct xray* h, const struct superobject* root, const struct matrix4 transform, struct xray_node* start, struct xray_node* end, const float z)
{
    int mesh_index = 0;
    const struct collide_mesh* mesh = NULL;
    
    const struct matrix4 T = matrix4_mul(superobject_matrix_global(root), transform);
    
    if (superobject_type(root) == superobject_type_sector)
        current_sector = root;
    
    if (superobject_type(root) == superobject_type_ipo)
    {
        const struct ipo* ipo = superobject_data(root);
        if (!ipo) return;
        
        printf("IPO: %s\n", ipo_name(ipo));
        /* Get the reaction zone */
        const struct collide_object* zdr = ipo_collide_object(ipo);
        if (zdr)
        {
            while ((mesh = collide_object_mesh(zdr, mesh_index)))
            {
                printf("\tmesh %d: %d faces\n", mesh_index, host_byteorder_16(mesh->n_faces));
                const uint16* indices = (const uint16*)pointer(mesh->face_indices);
                const struct vector3* vertices = (const struct vector3*)pointer(zdr->vertices);
                const struct vector3* normals = (const struct vector3*)pointer(mesh->normals);
                
                for (int16 index = 0; index < host_byteorder_16(mesh->n_faces); index++)
                {
                    uint16 idx0 = host_byteorder_16(*(indices + index * 3 + 0));
                    uint16 idx1 = host_byteorder_16(*(indices + index * 3 + 1));
                    uint16 idx2 = host_byteorder_16(*(indices + index * 3 + 2));
                    
                    struct vector3 A = vector3_host_byteorder(*(vertices + index * 3 + 0));
                    struct vector3 B = vector3_host_byteorder(*(vertices + index * 3 + 1));
                    struct vector3 C = vector3_host_byteorder(*(vertices + index * 3 + 2));
                    
                    const struct vector4 TA = vector4_mul_matrix4(vector4_new(A.x, A.y, A.z, 1.0f), T);
                    const struct vector4 TB = vector4_mul_matrix4(vector4_new(B.x, B.y, B.z, 1.0f), T);
                    const struct vector4 TC = vector4_mul_matrix4(vector4_new(C.x, C.y, C.z, 1.0f), T);
                    
                    A = vector3_new(TA.x, TA.y, TA.z);
                    B = vector3_new(TB.x, TB.y, TB.z);
                    C = vector3_new(TC.x, TC.y, TC.z);
                    
                    const struct vector3 center = vector3_new((A.x + B.x + C.x) / 3.0f, (A.y + B.y + C.y) / 3.0f, (A.z + B.z + C.z) / 3.0f);
                    struct vector3 N = vector3_host_byteorder(*(normals + index));
                    
                    
                    //printf("normal: %.2f %.2f %.2f\n", N.x, N.y, N.z);
                    
                    struct xray_node node;
                    node.in_closed = false;
                    node.in_open = false;
                    node.superobject = root;
                    node.sector = current_sector;
                    node.normal = N;
                    
                    N = vector3_mulf(N, 2.0f);
                    
                    node.score = xray_node_distance(&node, end);
                    node.position = vector3_add(vector3_new(A.x, A.y, A.z), N); h->nodes[h->n_nodes++] = node;
                    node.position = vector3_add(vector3_new(B.x, B.y, B.z), N); h->nodes[h->n_nodes++] = node;
                    node.position = vector3_add(vector3_new(C.x, C.y, C.z), N); h->nodes[h->n_nodes++] = node;
                    
                    //xray_line_generate_points(h, A, B);
                    
                    
                    //h->nodes[h->n_nodes++] = node;
//                    for (int i = 0; i < 2; i++)
//                    {
//                        node.position = vector3_new(TP.x + sin(i) * 2, TP.y + sin(i) * 2, TP.z);
//                        h->nodes[h->n_nodes++] = node;
//                    }
                }
                
                mesh_index++;
            }
        }
    }
    
    superobject_for_each(root, so)
    {
        xray_recursive_derive_pointset(h, so, T, start, end, z);
    };
}

struct ray rays[1024];
int n_rays = 0;
struct vector3 inters[1024];

/** Can node `a` be reached directly, by a straight line to node `b`? */
static void line_of_sight_recursive(struct xray *h, const struct superobject* target, const struct matrix4 transform, struct xray_node *a, struct xray_node *b, bool *v)
{
    //printf("object: %X\n", offset(target));
    
    if (!target) return;
    
    const struct matrix4 T = matrix4_mul(superobject_matrix_local(target), transform);
    
    if (superobject_type(target) == superobject_type_ipo)
    {
        const struct ipo* ipo = superobject_data(target);
        if (ipo)
        {
            /* Get the reaction zone */
            const struct collide_object* zdr = ipo_collide_object(ipo);
            if (zdr)
            {
                struct ray ray;
                struct vector3 intersection_point = vector3_new(0.0f, 0.0f, 0.0f);
                if (collide_object_intersect_segment(zdr, T, a->position, b->position, &ray, &intersection_point))
                {
                    *v = false;
                    
                 
                    //                    rays[n_rays] = ray;
                    //                    inters[n_rays] = intersection_point;
                    //                    n_rays++;
                    
                    /* Check normal-offset nodes? */
//                    const struct vector3 na = vector3_add(a->position, vector3_add(a->normal, vector3_new(0.0f, 0.0f, 0.1f)));
//                    const struct vector3 nb = vector3_add(b->position, vector3_add(b->normal, vector3_new(0.0f, 0.0f, 0.1f)));
//                    if (collide_object_intersect_segment(zdr, T, na, nb, NULL))
//                    {
//                        *v = false;
//                    }
                }
            }
        }
    }
    
    superobject_for_each(target, child)
    {
        line_of_sight_recursive(h, child, T, a, b, v);
    };
}

/** Can node `a` be reached directly, by a straight line to node `b`? */
bool line_of_sight(struct xray *h, struct xray_node *a, struct xray_node *b)
{
    memset(rays, 0, 1024 * sizeof(struct ray));
    memset(inters, 0, sizeof(struct vector3) * 1024);
    n_rays = 0;
    
    /* Get the sectors to which the nodes belong. */
    const struct superobject* sectorA = a->sector;
    const struct superobject* sectorB = b->sector;
    
    bool v = true;
    if (sectorA != sectorB)
    {
        /* If the nodes do not belong to the same sector, assume */
        /* they are far apart and may need to cross multiple sectors. */
        line_of_sight_recursive(h, father_sector, matrix4_identity, a, b, &v);
    }
    else
    {
        /* Only check the sector where the points are. */
        //printf("sector: %X\n", offset(sectorA));
        line_of_sight_recursive(h, sectorA, superobject_matrix_global(sectorA), a, b, &v);
    }
    
    return v;
}

#pragma mark - Pathfinding

static void theta_path_reconstruct(struct xray_node *S, struct xray_node** path, unsigned *path_length)
{
    path[(*path_length)++] = S;
    
    if (S->parent != S)
    {
        theta_path_reconstruct(S->parent, path, path_length);
        //path[(*path_length)++] = ;
    }
}

#define THETA_INITIAL_SIZE  (1 << 16)

static void theta_compute_cost(struct xray *h, struct xray_node *s, struct xray_node *s2, struct xray_node* start)
{
    if (line_of_sight(h, s->parent, s2))
    {
        const float distance = xray_node_distance(s->parent, s2);
        if (s->parent->score + distance < s2->score)
        {
            s2->parent = s->parent;
            s2->score = s->parent->score + distance + xray_node_distance(s->parent, start);
            s2->GH = s2->score + s2->H;
        }
    }
    else
    {
        const float distance = xray_node_distance(s, s2);
        if (s->score + distance < s2->score)
        {
            s2->parent = s;
            s2->score = s->score + distance + xray_node_distance(s, start);
            s2->GH = s2->score + s2->H;
        }
    }
}

static void theta_update_vertex(struct xray *h,
                                struct xray_node** open,
                                unsigned long *io,
                                unsigned long *szo,
                                struct xray_node *S,
                                struct xray_node *neighbor,
                                struct xray_node *start,
                                struct xray_node *end)
{
//    const float G = S->score;
//
//    theta_compute_cost(h, S, neighbor, start);
//
//    if (neighbor->G < G)
//    {
//        /* Remove neighbor from open set */
//        memmove(open + neighbor->io, open + neighbor->io + 1, sizeof(struct xray_node*) * (THETA_INITIAL_SIZE - neighbor->io));
//        //(*io)--;
//        neighbor->in_open = true;
//        neighbor->io = *io;
//        neighbor->score += xray_node_distance(neighbor, end);
//        open[*io] = neighbor;
//    }
    

    if (line_of_sight(h, S->parent, neighbor))
    {
        printf("line of sight: %d\n", *io);
        /* If the parent of S and neighbor are in line of sight, */
        /* use the path between S and neighbor, ignoring S completely. */
        const float d = xray_node_distance(S->parent, neighbor);
        if (S->parent->score + d < neighbor->score)
        {
            neighbor->score = xray_node_distance(S->parent, start) + S->parent->score + d;
            neighbor->parent = S->parent;

            if (neighbor->in_open)
            {
                /* Remove neighbor from open set */
                memmove(open + neighbor->io, open + neighbor->io + 1, sizeof(struct xray_node*) * (THETA_INITIAL_SIZE - neighbor->io));
                (*io)--;
                neighbor->in_open = false;
            }

            //neighbor->score += heuristic();
            /* Add the neighbor to the open set */
            neighbor->in_open = true;
            neighbor->io = *io;
            //neighbor->score += xray_node_distance(neighbor, end);
            open[(*io)++] = neighbor;
        }
    }
    else
    {
        const float d = xray_node_distance(S, neighbor);
        //printf("distance: %f\n", d);
        if (S->score + d < neighbor->score)
        {
            neighbor->score = xray_node_distance(S, start) + S->score + d;
            neighbor->parent = S;

            if (neighbor->in_open)
            {
                /* Remove neighbor from open set */
                memmove(open + neighbor->io, open + neighbor->io + 1, sizeof(struct xray_node*) * (THETA_INITIAL_SIZE - neighbor->io));
                (*io)--;
                neighbor->in_open = false;
            }

            //neighbor->score += 4.0; //heuristic();
            /* Add the neighbor to the open set */
            neighbor->in_open = true;
            neighbor->io = *io;
            //neighbor->score += xray_node_distance(neighbor, end);
            open[(*io)++] = neighbor;
        }
    }
}

static void theta(struct xray *h, struct xray_node* start, struct xray_node* end, struct xray_node** path, unsigned *path_length)
{
    unsigned long szo = sizeof(struct xray_node*) * THETA_INITIAL_SIZE;
    unsigned long szc = sizeof(struct xray_node*) * THETA_INITIAL_SIZE;
    unsigned long io = 0, ic = 0;
    
    /* Allocate open and closed sets. */
    struct xray_node** open = malloc(szo);
    struct xray_node** closed = malloc(szc);
    
    start->score = xray_node_distance(start, end);
    start->parent = start;
    start->in_open = true;
    start->in_closed = false;
    start->io = 0;
    start->ic = 0;
    
    open[io++] = start;
    
    while (0 != io)
    {
        struct xray_node* S = open[--io];
        
        if (S == end)
        {
            printf("End reached\n");
            return theta_path_reconstruct(S, path, path_length);
        }
        
        S->in_closed = true;
        closed[ic++] = S;
        
        for (unsigned n = 0; n < S->n_neighbors; n++)
        {
            struct xray_node* neighbor = S->neighbor[n];
            if (!neighbor->in_closed)
            {
                if (!neighbor->in_open)
                {
                    neighbor->score = INFINITY;
                    neighbor->parent = NULL;
                }
                
                theta_update_vertex(h, open, &io, &szo, S, neighbor, start, end);
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

static int neighbors = 0;

static void xray_node_find_neighbors(struct xray *h, struct xray_node* node)
{
    if (node->n_neighbors == 0)
    {
        float radius = 10.0f;
        //while (node->n_neighbors < 5)
        {
            for (int i = 0; i < h->n_nodes; i++)
            {
                struct xray_node* cmp = &h->nodes[i];
                if (cmp != node)
                {
                    if (xray_node_distance(node, cmp) <= radius)
                    {
                        //printf("distance: %f\n", theta_node_distance(node, cmp));
                        node->neighbor[node->n_neighbors++] = &h->nodes[i];
                        //                node->neighbor[node->n_neighbors++] = &h->nodes[i + 2];
                        //                node->neighbor[node->n_neighbors++] = &h->nodes[i + 3];
                        //neighbors++;
                    }
                }
            }
            
            radius *= 2.0f;
        }
    }
}

void xray_init(struct xray* h)
{
    struct xray_node start;
    start.position = actor_position(actor_find(actor_instance_name, "Rayman", dynamic_world));
    start.in_open = false;
    start.in_closed = false;
    start.score = 0.0f;
    
    struct xray_node end;
    end.position = actor_position(actor_find(actor_instance_name, "BEN_cameleon", dynamic_world));
    end.in_open = false;
    end.in_closed = false;
    end.score = 0.0f;
    
    h->nodes[h->n_nodes++] = start;
    
    const struct superobject* sectorStart = sector_by_location(father_sector, start.position);
    const struct superobject* sectorEnd = sector_by_location(father_sector, end.position);
    
    xray_recursive_derive_pointset(h, sectorStart, matrix4_identity, &start, &end, 0.75f);
    //if (sectorStart != sectorEnd) xray_derive_pointset(h, sectorEnd, 0.75f);
    
    printf("n points: %d\n", h->n_nodes);
    
    h->nodes[h->n_nodes++] = end;
    
    for (int i = 0; i < h->n_nodes; i++)
    {
        h->nodes[i].n_neighbors = 0;
        xray_node_find_neighbors(h, &h->nodes[i]);
        //printf("n neighbors: %d\n", neighbors);
    }
    
    printf("start: %d neighbors\n", h->nodes[0].n_neighbors);
    
    memset(h->path, 0, sizeof(struct xray_node*) * 10000);
    
    
    unsigned length = 0;
    theta(h, &h->nodes[0], &h->nodes[h->n_nodes-1], h->path, &length);
    
    printf("length :%d\n", length);
}
