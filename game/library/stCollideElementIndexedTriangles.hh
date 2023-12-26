#ifndef stCollideElementIndexedTriangles_h
#define stCollideElementIndexedTriangles_h

#include "library.hh"

typedef struct stElementIndexedTrianglesVisual stElementIndexedTrianglesVisual;
typedef struct stCollideElementIndexedTriangles stCollideElementIndexedTriangles;

struct stElementIndexedTrianglesVisual
{
    readonly pointer<> visualMaterial;
    readonly int16 numFaces;
    readonly int16 numUVs;
    readonly int16 numUVStages;
    readonly padding(2)
    readonly pointer<> faceIndices;
    readonly pointer<> faceUVIndices;
    readonly pointer<> faceNormals;
    readonly pointer<> UVElements;
    readonly pointer<> edges;
    readonly pointer<> adjacentFaces;
    readonly pointer<> thisIndexList;
    readonly int16 numUsedIndices;
    readonly int16 boundingBoxIndex;
    readonly uint32 displayList;
    readonly pointer<> unknown;
    readonly uint8 portalVisibility;
    readonly padding(3)
    readonly uint32 vao[4];
};

struct stCollideElementIndexedTriangles
{
    readonly pointer<> material;
    readonly pointer<> faceIndices;
    readonly pointer<stVector3D> normals;
    readonly int16 numFaces;
    readonly int16 aabbIndex;
    readonly pointer<> geometryVisualization; /* above struct */
    readonly pointer<> faceEdges;
    readonly pointer<> edgeNormals;
    readonly pointer<> edgeCoefficients;
    readonly int16 numEdges;
    readonly padding(2)
};

#endif /* stCollideElementIndexedTriangles_h */
