//
//  xray.h
//  r3-tas
//
//  Created by Jba03 on 2023-02-27.
//

#ifndef xray_h
#define xray_h

#include "structure.h"
#include "stSuperObject.h"
#include "stEngineObject.h"
#include "stSector.h"
#include "stOctree.h"

#define XRAY_MAX_OBJECTS   256
#define XRAY_MAX_ROUTES     16
#define XRAY_MAX_OCTREE_NODES   2048
#define XRAY_MAX_THETA_NODES    2048

typedef struct xray xray;
typedef struct xray_node xrayNode;
typedef struct xraySourceRecord xraySourceRecord;

#define XRAY_NODE_TYPE_WALK         0
#define XRAY_NODE_TYPE_ROLL         1
#define XRAY_NODE_TYPE_JUMP         2
#define XRAY_NODE_TYPE_FALL         3
#define XRAY_NODE_TYPE_HOVER        4
#define XRAY_NODE_TYPE_LEDGEGRAB    5

struct xray_node {
    
    unsigned type;
    /* node's position */
    tdstVector3D position;
    /* normal of the node */
    tdstVector3D normal;
    /* for transformation of the node */
    tdstSuperObject* superobject;
    /* the sector this node belongs to */
    tdstSuperObject* sector;
    
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

struct xraySourceRecord {
#define XRAY_MAX_TAGGED_FACES   2048
    /* List of recorded nodes */
    xrayNode* nodes;
    /* List of objects collided with */
    const tdstSuperObject** objects;
    /* List of tagged faces */
    int16 taggedFaces[XRAY_MAX_TAGGED_FACES];
    
    unsigned int numNodes;
    unsigned int numNodesAllocated;
    unsigned int numObjects;
    unsigned int numObjectsAllocated;
    unsigned int numTaggedFaces;
    unsigned int locked;
};

struct xray {
    int n_routes;
    struct xray_route* routes[XRAY_MAX_ROUTES];
    
    int n_points;
    int n_lines;
    tdstVector3D pointset[65536 * 4];
    
    int n_nodes;
    struct xray_node nodes[200000];
        
    struct xray_node* path[10000];
    
    struct xray_route* current_route;
    
    
    struct xraySourceRecord sourceRecord;
    
    
    
};

struct xray_route
{
    /* source point */
    tdstVector3D source;
    /* destination point */
    tdstVector3D destination;
    
    /* number of octree nodes */
    int n_octree_nodes;
    /* list of route octree nodes */
    const tdstOctreeNode* octree[XRAY_MAX_OCTREE_NODES];
    /* list of ipos that contain above octrees at the same indices */
    const tdstSuperObject* octree_ipo[XRAY_MAX_OCTREE_NODES];
    

    
    /* current target, null if none */
    //tdstSuperObject* target;
};

struct xray_output
{
    bool jump;
    bool hover;
};

void xrayInitialize(xray* h);

void xraySourceAddNode(xray* h, xrayNode node);

void xraySourceFinished(xray* h);

/** xray_analyse: analyse the scene and generate an optimal route to the specified destination. */
void xrayAnalyse(xray* h);


void xray_frame(xray* h, struct xray_output* output);

#endif /* xray_h */
