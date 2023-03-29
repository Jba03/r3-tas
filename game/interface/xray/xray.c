//
//  xray.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-27.
//

#include "xray.h"
#include "stDynamics.h"
#include "stCollideObject.h"
#include "stOctree.h"
#include "stInstantiatedPhysicalObject.h"
#include "game.h"
#include "ray.h"

#include "../../library/function/fnPrimIntersections.c"

#include <stdio.h>

static const tdstVector3D xray_player_pos()
{
    const tdstMatrix4D T = fnActorGetMatrix(actor_rayman);
    return game_matrix4_position(T);
}

static const tdstVector3D xray_get_destination(struct xray* h)
{
    return vector3_new(0.0f, 0.0f, 0.0f);
}

static bool xray_player_grounded()
{
    const tdstDynamics* dynamics = fnActorGetDynamics(actor_rayman);
    return fnDynamicsCollideWith(dynamics, dynamics_obstacle_ground);
}

static tdstCollideObject* xray_nearest_platform()
{
    
    
    return NULL;
}

static tdstOctreeNode* xray_current_octree_node(struct xray* h)
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
static void xray_route_recursive_get_octrees(struct xray* h, tdstSuperObject* root)
{
    struct xray_route* route = h->current_route;
    
    if (superobject_type(root) == superobject_type_ipo)
    {
        const tdstInstantiatedPhysicalObject* ipo = superobject_data(root);
        if (!ipo) return;
        
        /* Get the reaction zone */
        const tdstCollideObject* zdr = fnIPOGetCollideObject(ipo);
        if (zdr)
        {
            const tdstOctree* octree = pointer(zdr->octree);
            if (octree)
            {
                const tdstOctreeNode* node = pointer(octree->root);
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
    memset(route->octree, 0, sizeof(tdstOctreeNode*) * XRAY_MAX_OCTREE_NODES);
    memset(route->octree_ipo, 0, sizeof(tdstSuperObject*) * XRAY_MAX_OCTREE_NODES);
    
    xray_route_recursive_get_octrees(h, father_sector);
}

static const float xray_node_distance(struct xray_node* a, struct xray_node* b)
{
    return vector3_length(vector3_sub(a->position, b->position));
}

#pragma mark - Route pointset

static const tdstSuperObject* current_sector = NULL;

static void xray_line_generate_points(struct xray *h, const tdstVector3D a, const tdstVector3D b)
{
    const tdstVector3D AB = vector3_sub(b, a);
    const tdstVector3D ABn = vector3_normalize(AB);
    const float l = vector3_length(AB);
    
    #define S 20.0f
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

static void xray_recursive_derive_pointset(struct xray* h, const tdstSuperObject* root, const tdstMatrix4D transform, struct xray_node* start, struct xray_node* end, const float z)
{
    int mesh_index = 0;
    const tdstCollideElementIndexedTriangles* mesh = NULL;
    
    const tdstMatrix4D T = matrix4_mul(fnSuperobjectGetGlobalMatrix(root), transform);
    
    if (superobject_type(root) == superobject_type_sector)
        current_sector = root;
    
    if (superobject_type(root) == superobject_type_ipo)
    {
        const tdstInstantiatedPhysicalObject* ipo = superobject_data(root);
        if (!ipo) return;
        
        printf("IPO: %s\n", fnIPOGetName(ipo));
        /* Get the reaction zone */
        const tdstCollideObject* zdr = fnIPOGetCollideObject(ipo);
        if (zdr)
        {
            while ((mesh = fnCollideObjectGetElementIndexedTriangles(zdr, mesh_index)))
            {
                printf("\tmesh %d: %d faces\n", mesh_index, host_byteorder_16(mesh->n_faces));
                const uint16* indices = (const uint16*)pointer(mesh->face_indices);
                const tdstVector3D* vertices = (const tdstVector3D*)pointer(zdr->vertices);
                const tdstVector3D* normals = (const tdstVector3D*)pointer(mesh->normals);
                
                for (int16 index = 0; index < host_byteorder_16(mesh->n_faces); index++)
                {
                    uint16 idx0 = host_byteorder_16(*(indices + index * 3 + 0));
                    uint16 idx1 = host_byteorder_16(*(indices + index * 3 + 1));
                    uint16 idx2 = host_byteorder_16(*(indices + index * 3 + 2));
                    
                    tdstVector3D A = vector3_host_byteorder(*(vertices + index * 3 + 0));
                    tdstVector3D B = vector3_host_byteorder(*(vertices + index * 3 + 1));
                    tdstVector3D C = vector3_host_byteorder(*(vertices + index * 3 + 2));
                    
                    const tdstVector4D TA = vector4_mul_matrix4(vector4_new(A.x, A.y, A.z, 1.0f), T);
                    const tdstVector4D TB = vector4_mul_matrix4(vector4_new(B.x, B.y, B.z, 1.0f), T);
                    const tdstVector4D TC = vector4_mul_matrix4(vector4_new(C.x, C.y, C.z, 1.0f), T);
                    
                    A = vector3_new(TA.x, TA.y, TA.z);
                    B = vector3_new(TB.x, TB.y, TB.z);
                    C = vector3_new(TC.x, TC.y, TC.z);
                    
                    const tdstVector3D center = vector3_new((A.x + B.x + C.x) / 3.0f, (A.y + B.y + C.y) / 3.0f, (A.z + B.z + C.z) / 3.0f);
                    tdstVector3D N = vector3_host_byteorder(*(normals + index));
                    
                    
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
tdstVector3D inters[1024];

/** Can node `a` be reached directly, by a straight line to node `b`? */
static void line_of_sight_recursive(struct xray *h, const tdstSuperObject* target, const tdstMatrix4D transform, struct xray_node *a, struct xray_node *b, bool *v)
{
    //printf("object: %X\n", offset(target));
    
    if (!target) return;
    
    const tdstMatrix4D T = matrix4_mul(fnSuperobjectGetLocalMatrix(target), transform);
    
    if (superobject_type(target) == superobject_type_ipo)
    {
        const tdstInstantiatedPhysicalObject* ipo = superobject_data(target);
        if (ipo)
        {
            /* Get the reaction zone */
            const tdstCollideObject* zdr = fnIPOGetCollideObject(ipo);
            if (zdr)
            {
                struct ray ray;
                tdstVector3D intersection_point = vector3_new(0.0f, 0.0f, 0.0f);
                if (fnCollideObjectIntersectSegment(zdr, T, a->position, b->position, &ray, &intersection_point))
                {
                    *v = false;
                    
                 
                    //                    rays[n_rays] = ray;
                    //                    inters[n_rays] = intersection_point;
                    //                    n_rays++;
                    
                    /* Check normal-offset nodes? */
//                    const tdstVector3D na = vector3_add(a->position, vector3_add(a->normal, vector3_new(0.0f, 0.0f, 0.1f)));
//                    const tdstVector3D nb = vector3_add(b->position, vector3_add(b->normal, vector3_new(0.0f, 0.0f, 0.1f)));
//                    if (fnCollideObjectIntersectSegment(zdr, T, na, nb, NULL))
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
    memset(inters, 0, sizeof(tdstVector3D) * 1024);
    n_rays = 0;
    
    /* Get the sectors to which the nodes belong. */
    const tdstSuperObject* sectorA = a->sector;
    const tdstSuperObject* sectorB = b->sector;
    
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
        line_of_sight_recursive(h, sectorA, fnSuperobjectGetGlobalMatrix(sectorA), a, b, &v);
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
void xray_analyse(struct xray* h, const tdstVector3D destination)
{
    const tdstVector3D pos = xray_player_pos();
    
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
    start.position = fnActorGetPosition(actor_find(actor_instance_name, "Rayman", dynamic_world));
    start.in_open = false;
    start.in_closed = false;
    start.score = 0.0f;
    
    struct xray_node end;
    end.position = fnActorGetPosition(actor_find(actor_instance_name, "BEN_cameleon", dynamic_world));
    end.in_open = false;
    end.in_closed = false;
    end.score = 0.0f;
    
    h->nodes[h->n_nodes++] = start;
    
    const tdstSuperObject* sectorStart = sector_by_location(father_sector, start.position);
    const tdstSuperObject* sectorEnd = sector_by_location(father_sector, end.position);
    
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

static const tdstMatrix4D xrayComputeAbsoluteMatrix(const tdstSuperObject *object, const tdstSuperObject *root, tdstMatrix4D T)
{
    tdstMatrix4D result = matrix4_mul(fnSuperobjectGetGlobalMatrix(root), T);
    if (object == root) return matrix4_mul(fnSuperobjectGetGlobalMatrix(object), result);
    superobject_for_each(root, child) xrayComputeAbsoluteMatrix(object, child, T);
    return result;
}

static void xrayDerivePointset(xray* h)
{
    
}

#define MAX_INDEX       0x0080
#define OVERFLOW_INDEX  0x8000

static void xrayOctreeNodeDerivePointset(xray* h, const tdstSuperObject *object, const tdstMatrix4D T, const tdstCollideObject* colObj, const tdstOctreeNode *octreeNode, const tdstOctree* octreeBase)
{
    /* The list begins with the number of elements, */
    uint16 numElements = host_byteorder_16(*(uint16*)pointer(octreeNode->face_indices));
    /* followed by the encoded element indices. */
    uint8* indexList = (uint8*)(((uint16*)pointer(octreeNode->face_indices)) + 1);
    printf("num elements: %d\n", numElements);
    for (unsigned int index = 0; index < numElements; index++)
    {
        int16 staticElementIndex = 0;
        int16 dataElementIndex = 0;
        
        if (*indexList < MAX_INDEX)
        {
            /* uint8, no conversion necessary */
            staticElementIndex = *(indexList++);
        }
        else
        {
            int16 data;
            data = (int16)*(indexList++);
            data <<= 8;
            data += (int16)*(indexList++);
            data &= ~OVERFLOW_INDEX;
            staticElementIndex = (data);
        }
        
        if (*indexList < MAX_INDEX)
        {
            /* uint8, no conversion necessary */
            dataElementIndex = *(indexList++);
        }
        else
        {
            int16 data;
            data = (int16)*(indexList++);
            data <<= 8;
            data += (int16)*(indexList++);
            data &= ~OVERFLOW_INDEX;
            dataElementIndex = (data);
        }
        
        uint16 elementType = host_byteorder_16(*(((uint16*)pointer(colObj->element_types) + staticElementIndex)));
        if (elementType == collide_object_indexed_triangles)
        {
            const pointer element = *((pointer*)pointer(colObj->elements) + staticElementIndex);
            tdstCollideElementIndexedTriangles* mesh = (tdstCollideElementIndexedTriangles*)pointer(element);
            
            //xrayCollideObjectDerivePointset(h, T, colObj, mesh, dataElementIndex);
            
            printf("offset: %d %d\n", staticElementIndex, dataElementIndex);
            
        }
    }
}

static void xraySourceDeriveStaticNodesRecursive(xray* h, const tdstSuperObject *object, const tdstMatrix4D T, const tdstCollideObject* colObj, const tdstOctreeNode *octreeNode, const tdstOctree* octreeBase, tdstVector3D point)
{
    if (!object) return;
    if (!octreeNode) return;
    
    const pointer* children = pointer(octreeNode->children);
    
    tdstVector3D bMin = vector3_host_byteorder(octreeNode->min);
    tdstVector3D bMax = vector3_host_byteorder(octreeNode->max);
    tdstVector4D bMinT = vector4_mul_matrix4(vector4_new(bMin.x, bMin.y, bMin.z, 1.0f), T);
    tdstVector4D bMaxT = vector4_mul_matrix4(vector4_new(bMax.x, bMax.y, bMax.z, 1.0f), T);
        
    bMin = vector3_new(bMinT.x, bMinT.y, bMinT.z);
    bMax = vector3_new(bMaxT.x, bMaxT.y, bMaxT.z);
        
    if (fnBoxBoxIntersection(bMin, bMax, point, point))
    {
        if (octreeNode->face_indices != 0x00) xrayOctreeNodeDerivePointset(h, object, T, colObj, octreeNode, octreeBase);
        if (children)
        {
            for (unsigned int n = 0; n < 8; n++)
            {
                const tdstOctreeNode* child = pointer(*(children + n));
                xraySourceDeriveStaticNodesRecursive(h, object, T, colObj, child, octreeBase, point);
            }
        }
        else
        {
            /* This is the smallest octree which the point intersects. */
            
//            xrayNode node;
//            node.position = vector3_host_byteorder(octreeNode->min);
//            h->nodes[h->n_nodes++] = node;
//
//            node.position = vector3_host_byteorder(octreeNode->max);
//            h->nodes[h->n_nodes++] = node;
            
//            int n_selected = 0;
//            tdstOctreeNode* selected[256];
//            float st[1000];
//            octree_node_select(root, selected, &n_selected, st, 5.0f);
//            for (int i = 0; i < n_selected; i++)
//                xraySourceRecordAppendOctreeNode(src, selected[i]);
        }
    }
}

static xrayNode* xrayIsSourceNodeWithinRange(xray *h, xrayNode *base, unsigned searchType, unsigned maxRange)
{
    xrayNode* iterator = base;
    while (iterator - h->sourceRecord.nodes < h->sourceRecord.numNodes && iterator - base <= maxRange)
    {
        if (iterator++->type == searchType) return iterator;
        ++iterator;
    }
    
    return NULL;
}

static void xrayTriangleDerivePointset(xray *h, tdstVector3D A, tdstVector3D B, tdstVector3D C)
{
    const tdstVector3D AC = vector3_sub(C, A);
    const tdstVector3D BC = vector3_sub(C, B);
    const tdstVector3D ACn = vector3_normalize(AC);
    const tdstVector3D BCn = vector3_normalize(BC);
    const float lAC = vector3_length(AC);
    const float lBC = vector3_length(BC);

    #define S 10.0f
    for (float dAC = 0.0f, dBC = 0.0f; dAC < lAC && dBC < lBC; dAC += lAC / S, dBC += lBC / S)
    {
        tdstVector3D iAC = vector3_add(A, vector3_mulf(ACn, dAC));
        tdstVector3D iBC = vector3_add(B, vector3_mulf(BCn, dBC));

        xray_line_generate_points(h, iAC, iBC);
    }
    #undef S
//    struct xray_node node;
//    node.position = A; h->nodes[h->n_nodes++] = node;
//    node.position = B; h->nodes[h->n_nodes++] = node;
//    node.position = C; h->nodes[h->n_nodes++] = node;
    
//    xray_line_generate_points(h, A, B);
//    xray_line_generate_points(h, B, C);
//    xray_line_generate_points(h, C, A);
}

static void xraySourceDeriveStaticNodesRecursive2(xray *h, const tdstSuperObject *object, const tdstMatrix4D T, const tdstCollideObject *collObj, xrayNode *sourceNode)
{
    const tdstCollideElementIndexedTriangles* mesh = NULL;
    int mesh_index = 0;
    float t;
    
    while ((mesh = fnCollideObjectGetElementIndexedTriangles(collObj, mesh_index)))
    {
        const uint16* indices = (const uint16*)pointer(mesh->face_indices);
        const tdstVector3D* vertices = (const tdstVector3D*)pointer(collObj->vertices);
        const tdstVector3D* normals = (const tdstVector3D*)pointer(mesh->normals);
        
        for (int16 index = 0; index < host_byteorder_16(mesh->n_faces); index++)
        {
            for (unsigned int f = 0; f < h->sourceRecord.numTaggedFaces; f++)
                if (h->sourceRecord.taggedFaces[f] == index) goto end;
            
            uint16 idx0 = host_byteorder_16(*(indices + index * 3 + 0));
            uint16 idx1 = host_byteorder_16(*(indices + index * 3 + 1));
            uint16 idx2 = host_byteorder_16(*(indices + index * 3 + 2));
            
            tdstVector3D A = vector3_host_byteorder(*(vertices + idx0));
            tdstVector3D B = vector3_host_byteorder(*(vertices + idx1));
            tdstVector3D C = vector3_host_byteorder(*(vertices + idx2));
            /* Transform the points so that they are relative to the object. */
            const tdstVector4D TA = vector4_mul_matrix4(vector4_new(A.x, A.y, A.z, 1.0f), T);
            const tdstVector4D TB = vector4_mul_matrix4(vector4_new(B.x, B.y, B.z, 1.0f), T);
            const tdstVector4D TC = vector4_mul_matrix4(vector4_new(C.x, C.y, C.z, 1.0f), T);
            /* Construct the triangle. */
            A = vector3_new(TA.x, TA.y, TA.z);
            B = vector3_new(TB.x, TB.y, TB.z);
            C = vector3_new(TC.x, TC.y, TC.z);
            
            switch (sourceNode->type)
            {
                case XRAY_NODE_TYPE_WALK:
                {
                    //sourceNode->normal = vector3_new(0.0f, 0.0f, 1.0f);
                    /* On this frame, the player is walking. Test intersection with the triangle below using a line segment. */
                    tdstVector3D origin = vector3_add(sourceNode->position, sourceNode->normal);
                    tdstVector3D direction = vector3_normalize(vector3_negate(sourceNode->normal));
                    const struct ray ray = { origin, direction };
                    
                    if (rayTriangleIntersection(ray, A, B, C, &t))
                    {
                        //printf("%f vs %f\n", t, vector3_length(sourceNode->normal));
                        /* The origin can be elevated at a max distance of 1.0u from the surface, */
                        /* so a value of 2.0u should make sure it goes at least 1.0u under the surface. */
                        /* This makes sure that the ray does not intersect such triangles we do not wish to process. */
                        if (t < 2.0f)
                        {
                            /* There is intersection. */
                            xrayTriangleDerivePointset(h, A, B, C);
                            
                            h->sourceRecord.taggedFaces[h->sourceRecord.numTaggedFaces++] = index;
                        }
                        
                        printf("n points: %d\n", h->n_nodes);
                    }
                    
                    if (xrayIsSourceNodeWithinRange(h, sourceNode, XRAY_NODE_TYPE_JUMP, 20))
                    {
                        /* We may need to check for an eventual ceiling */
                    }
                        
                    break;
                }
                    
                default:
                    break;
            }
            
        end:;
        }
        
        mesh_index++;
    }
}

static void xraySourceDeriveStaticNodes(xray* h)
{
    xraySourceRecord* record = &h->sourceRecord;
    if (!record) return;
    
    printf("derive nodes\n");
    for (unsigned int n = 0; n < record->numObjects; n++)
    {
        const tdstSuperObject* object = record->objects[n];
        if (!object) continue;
        
        /* Ignore any actors that may possibly end up in this list, since */
        /* they are dynamic, and we are yet only looking at static geometry. */
        if (superobject_type(object) != superobject_type_ipo) continue;
        
        const tdstInstantiatedPhysicalObject* ipo = superobject_data(object);
        if (!ipo) continue;
        
        const tdstCollideObject* collideObject = fnIPOGetCollideObject(ipo);
        if (!collideObject) continue;
        
        for (unsigned int i = 0; i < record->numNodes; i++)
        {
            const tdstMatrix4D T = xrayComputeAbsoluteMatrix(object, father_sector, matrix4_identity);
            xraySourceDeriveStaticNodesRecursive2(h, object, matrix4_identity, collideObject, record->nodes + i);
        }
    }
}


#pragma mark - Source record

#define XRAY_SOURCE_RECORD_INITIAL_COUNT    (1 << 10)

static void xraySourceRecordInit(xraySourceRecord *src)
{
    src->nodes = malloc(sizeof(xrayNode) * XRAY_SOURCE_RECORD_INITIAL_COUNT);
    src->objects = malloc(sizeof(tdstSuperObject*) * XRAY_SOURCE_RECORD_INITIAL_COUNT);
    src->numNodes = 0;
    src->numObjects = 0;
    src->numNodesAllocated = XRAY_SOURCE_RECORD_INITIAL_COUNT;
    src->numObjectsAllocated = XRAY_SOURCE_RECORD_INITIAL_COUNT;
}

static void xraySourceRecordAppendNode(xraySourceRecord *src, xrayNode node)
{
    if (++src->numNodes >= src->numNodesAllocated)
    {
        src->numNodesAllocated *= 2;
        src->nodes = realloc(src->nodes, sizeof(xrayNode) * src->numNodesAllocated);
    }
    
    src->nodes[src->numNodes] = node;
}

static void xraySourceRecordAppendObject(xraySourceRecord *src, tdstSuperObject* object)
{
    for (unsigned int i = 0; i < src->numObjects; i++)
        /* Avoid inserting the same object */
        if (src->objects[i] == object) return;
    
    if (++src->numObjects >= src->numObjectsAllocated)
    {
        src->numObjectsAllocated *= 2;
        src->objects = realloc(src->objects, sizeof(tdstSuperObject*) * src->numObjectsAllocated);
    }
    
    src->objects[src->numObjects] = object;
}

#pragma mark - XRAY

void xrayInitialize(xray* h)
{
    xraySourceRecordInit(&h->sourceRecord);
}

void xraySourceAddNode(xray* h, xrayNode node)
{
    if (!h->sourceRecord.locked)
    {
        xraySourceRecordAppendNode(&h->sourceRecord, node);
        if (node.superobject) xraySourceRecordAppendObject(&h->sourceRecord, node.superobject);
    }
    else
    {
        fprintf(stderr, "[xray] cannot append node: source run locked\n");
    }
}

void xraySourceFinished(xray* h)
{
    h->sourceRecord.locked = true;
    xraySourceDeriveStaticNodes(h);
}

void xrayAnalyse(xray* h)
{
    
}
