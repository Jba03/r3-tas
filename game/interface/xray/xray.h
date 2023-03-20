//
//  xray.h
//  r3-tas
//
//  Created by Jba03 on 2023-02-27.
//

#ifndef xray_h
#define xray_h

#include "structure.h"
#include "superobject.h"
#include "actor.h"
#include "sector.h"
#include "octree.h"

#define XRAY_MAX_OBJECTS   256
#define XRAY_MAX_ROUTES     16
#define XRAY_MAX_OCTREE_NODES   2048
#define XRAY_MAX_THETA_NODES    2048

struct xray_node
{
    /* node's position */
    struct vector3 position;
    /* normal of the node */
    struct vector3 normal;
    /* for transformation of the node */
    struct superobject* superobject;
    /* the sector this node belongs to */
    struct superobject* sector;
    
    /* node parent */
    struct xray_node* parent;
    /* immediate neighbors */
    
    int n_neighbors;
    struct xray_node* neighbor[1024*2];
    
    bool in_open;
    bool in_closed;
    
    unsigned long io;
    unsigned long ic;
    
    float score;
    
    float G;
    float H;
    float GH;
};

struct xray_route
{
    /* source point */
    struct vector3 source;
    /* destination point */
    struct vector3 destination;
    
    /* number of octree nodes */
    int n_octree_nodes;
    /* list of route octree nodes */
    const struct octree_node* octree[XRAY_MAX_OCTREE_NODES];
    /* list of ipos that contain above octrees at the same indices */
    const struct superobject* octree_ipo[XRAY_MAX_OCTREE_NODES];
    

    
    /* current target, null if none */
    //struct superobject* target;
};

struct xray_line
{
    struct vector4 a;
    struct vector4 b;
};

struct xray
{
    int n_routes;
    struct xray_route* routes[XRAY_MAX_ROUTES];
    
    int n_points;
    int n_lines;
    struct vector3 pointset[65536 * 4];
    struct xray_line lines[10000];
    
    int n_nodes;
    struct xray_node nodes[200000];
        
    struct xray_node* path[10000];
    
    struct xray_route* current_route;
};

struct xray_output
{
    bool jump;
    bool hover;
};

void xray_init(struct xray* h);

/** xray_analyse: analyse the scene and generate an optimal route to the specified destination. */
void xray_analyse(struct xray* h, const struct vector3 destination);

void xray_frame(struct xray* h, struct xray_output* output);

#endif /* xray_h */
