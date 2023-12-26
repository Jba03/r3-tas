#include "solver.hh"
#include "memory.hh"

#include "stInstantiatedPhysicalObject.hh"
#include "stPhysicalObject.hh"
#include "stCollideObject.hh"
#include "stCollideElementIndexedTriangles.hh"
#include "stDynamics.hh"

#include "fnPrimIntersections.c"

#define MAX_NUM_TRIANGLES   256
/* Constants derived from dynamics */
#define MAX_SLOPE 0.707  //M_SQRT1_2
#define MIN_SLOPE -0.707 //-M_SQRT1_2

static const tdstVector3D WORLD_UP = { 0.0f, 0.0f, 1.0f };

bool sphereTriangleIntersection(struct Sphere *sphere, struct Triangle *tri)
{
    const tdstVector3D A = vector3_sub(tri->A, sphere->center);
    const tdstVector3D B = vector3_sub(tri->B, sphere->center);
    const tdstVector3D C = vector3_sub(tri->C, sphere->center);
    const float rr = sphere->radius * sphere->radius;
    
    const tdstVector3D V = vector3_cross(vector3_sub(B, A), vector3_sub(C, A));
    const float d = vector3_dot(A, V);
    const float e = vector3_dot(V, V);
    const bool sep1 = d * d > rr * e;
    
    const float aa = vector3_dot(A, A);
    const float ab = vector3_dot(A, B);
    const float ac = vector3_dot(A, C);
    const float bb = vector3_dot(B, B);
    const float bc = vector3_dot(B, C);
    const float cc = vector3_dot(C, C);
    
    const bool sep2 = (aa > rr) & (ab > aa) && (ac > aa);
    const bool sep3 = (bb > rr) & (ab > bb) && (bc > bb);
    const bool sep4 = (cc > rr) & (ac > cc) && (bc > cc);
    
    const tdstVector3D AB = vector3_sub(B, A);
    const tdstVector3D BC = vector3_sub(C, B);
    const tdstVector3D CA = vector3_sub(A, C);
    
    const float d1 = ab - aa;
    const float d2 = bc - bb;
    const float d3 = ac - cc;
    
    const float e1 = vector3_dot(AB, AB);
    const float e2 = vector3_dot(BC, BC);
    const float e3 = vector3_dot(CA, CA);
    
    const tdstVector3D Q1 = vector3_sub(vector3_mulf(A, e1), vector3_mulf(AB, d1));
    const tdstVector3D Q2 = vector3_sub(vector3_mulf(B, e2), vector3_mulf(BC, d2));
    const tdstVector3D Q3 = vector3_sub(vector3_mulf(C, e3), vector3_mulf(CA, d3));
    
    const tdstVector3D QC = vector3_sub(vector3_mulf(C, e1), Q1);
    const tdstVector3D QA = vector3_sub(vector3_mulf(A, e2), Q2);
    const tdstVector3D QB = vector3_sub(vector3_mulf(B, e3), Q3);
    
    const bool sep5 = (vector3_dot(Q1, Q1) > rr * e1 * e1) && (vector3_dot(Q1, QC) > 0);
    const bool sep6 = (vector3_dot(Q2, Q2) > rr * e2 * e2) && (vector3_dot(Q2, QA) > 0);
    const bool sep7 = (vector3_dot(Q3, Q3) > rr * e3 * e3) && (vector3_dot(Q3, QB) > 0);
    
    return !(sep1 | sep2 | sep3 | sep4 | sep5 | sep6 | sep7);
}

static bool isGround(const tdstVector3D normal)
{
    const float dot = vector3_dot(normal, WORLD_UP);
    return (dot >= MAX_SLOPE);
}

static bool isWall(const tdstVector3D normal)
{
    const float dot = vector3_dot(normal, WORLD_UP);
    return (dot > MIN_SLOPE) && (dot < MAX_SLOPE);
}

static void
collideElementSphereIntersection(const tdstCollideObject *obj, const tdstMatrix4D T, const tdstCollideElementIndexedTriangles *m, struct Sphere *sph, unsigned *numTrisOut, struct Triangle *out)
{
    const tdstVector3D* vertices = (const tdstVector3D*)pointer(obj->vertices);
    const tdstVector3D* normals = (const tdstVector3D*)pointer(m->normals);
    const uint16* indices = (const uint16*)pointer(m->faceIndices);
        
    for (int16 index = 0; index < host_byteorder_16(m->numFaces); index++)
    {
        uint16 idx0 = host_byteorder_16(*(indices + index * 3 + 0));
        uint16 idx1 = host_byteorder_16(*(indices + index * 3 + 1));
        uint16 idx2 = host_byteorder_16(*(indices + index * 3 + 2));
            
        const tdstVector3D Pa = vector3_host_byteorder(*(vertices + idx0));
        const tdstVector3D Pb = vector3_host_byteorder(*(vertices + idx1));
        const tdstVector3D Pc = vector3_host_byteorder(*(vertices + idx2));
        const tdstVector3D Na = vector3_host_byteorder(*(normals + index / 3));
            
        const tdstVector4D TpA = vector4_mul_matrix4(vector4_new(Pa.x, Pa.y, Pa.z, 1.0f), T);
        const tdstVector4D TpB = vector4_mul_matrix4(vector4_new(Pb.x, Pb.y, Pb.z, 1.0f), T);
        const tdstVector4D TpC = vector4_mul_matrix4(vector4_new(Pc.x, Pc.y, Pc.z, 1.0f), T);
        
        struct Triangle t;
        t.A = *(tdstVector3D*)&TpA;
        t.B = *(tdstVector3D*)&TpB;
        t.C = *(tdstVector3D*)&TpC;
        t.N = Na;
        
        if (sphereTriangleIntersection(sph, &t))
            out[(*numTrisOut)++] = t;
    }
}

#pragma mark - Static geometry



void solveGeometry(tdstSuperObject *g, struct GeometrySolver *solver)
{
    tdstVector3D start = solver->start;
    tdstVector3D end = solver->end;
    //solver->numPoints = 0;
    //solver->numTriangles = 0;
    
    /* Calculate the angle from start to end vertex */
    const float angle = atan2f(start.y - end.y, start.x - end.x);
    
    /* Only check IPOs, which have collision. */
    if (superobject_type(g) == superobject_type_ipo)
    {
        tdstInstantiatedPhysicalObject *ipo = fnSuperobjectGetData(g);
        tdstCollideObject *collideObject = fnIPOGetCollideObject(ipo);
        
        if (collideObject)
        {
            const float bsphR = host_byteorder_f32(collideObject->boundingSphereRadius);
            const tdstVector3D bsphC = vector3_host_byteorder(*(tdstVector3D*)&collideObject->boundingSpherePosition);
            /* Check if any of the start or end points lie within the bounding sphere. */
            if (fnPointInSphere(start, bsphC, bsphR) || fnPointInSphere(end, bsphC, bsphR))
            {
                int index = 0;
                const tdstCollideElementIndexedTriangles *m = NULL;
                /* For each collideable mesh */
                while ((m = fnCollideObjectGetElementIndexedTriangles(collideObject, index)))
                {
                    struct Sphere sphere;
                    sphere.center = start;
                    sphere.radius = 0.9f;
                    
                    tdstVector3D savedPosition = sphere.center;
                    for (unsigned int i = 0; i < 30; i++)
                    {
                        /* Initial distance */
                        float d = vector3_length(vector3_sub(savedPosition, end));
                        /* Optimal normal length */
                        float n = 1.0f;
                        
                        float optimal = angle;
                        
                        #define ANGLE (M_PI / 4.0f)
                        for (float π = -ANGLE; π < ANGLE; π += 0.1f)
                        {
                            unsigned numTris = 0;
                            struct Triangle triangles[MAX_NUM_TRIANGLES];
                            /* Collide the geometry with the sphere */
                            collideElementSphereIntersection(collideObject, fnSuperobjectGetGlobalMatrix(g), m, &sphere, &numTris, triangles);
                            
                            /* Calculate the resulting normal of all triangles collided with */
                            tdstVector3D resultingNormal = vector3_new(0.0f, 0.0f, 0.0f);
                            tdstVector3D centerPoint = vector3_new(0.0f, 0.0f, 0.0f);
                            for (unsigned int n = 0; n < numTris; n++)
                            {
                                struct Triangle t = triangles[n];
                                resultingNormal = vector3_add(resultingNormal, t.N);
                                
                                //tdstVector3D orientation = vector3_sub
                                //centerPoint = vector3_add(centerPoint, vector3_new((t.A.x + t.B.x + t.C.x) / 3.0f, (t.A.y + t.B.y + t.C.y) / 3.0f, (t.A.z + t.B.z + t.C.z) / 3.0f));
                            }
                            //centerPoint = vector3_mulf(centerPoint, 1.0f / (float)numTris);
                            resultingNormal = vector3_normalize(resultingNormal);
                            
//                            if (!isGround(resultingNormal) && isWall(resultingNormal))
//                                for (unsigned int n = 0; n < numTris; n++) solver->triangles[solver->numTriangles++] = triangles[n];
                            
                            /* Determine the 2D angle of the normal relative to the (projected) triangle center */
                            const float nA = atan2f(resultingNormal.y - centerPoint.y, resultingNormal.x + centerPoint.x);
                            
                            
                            /* The best angle is the  */
                            //solver->points[solver->numPoints++] = sphere.center;
                            
                            /* Calculate the new position */
                            const float x = cos(π + angle);
                            const float y = sin(π + angle);
                            tdstVector3D newPosition = sphere.center = vector3_sub(savedPosition, vector3_new(x, y, 0.0f));
                            
                            
                            float dd = vector3_length(vector3_sub(sphere.center, end));
                            //float dn = vector3_length(resultingNormal, resultingNormal);
                            if (dd < d && (nA < π + angle) && isGround(resultingNormal) && !isWall(resultingNormal))
                            {
                                d = dd;
                                //n = dn;
                                optimal = angle + π;
                            }
                            
                            
                            //solver->points[solver->numPoints] = newPosition;
                            //solver->points[solver->numPoints++].z += 2.0f;
                            
                            {
                                /* For each triangle we collide with, calculate the normal */
                                //solver->triangles[solver->numTriangles++] = triangles[i];
                            }
                        }
                        
                        
                        
                        solver->points[solver->numPoints++] = sphere.center;
                        
                        /* While on ground, we can move at max 0.25u/s. Use 0.3f = (ZDM size / 3) */
                        #define STEP 0.3f
                        /* Move the sphere in the optimal angle */
                        const float x = cos(optimal) * STEP;
                        const float y = sin(optimal) * STEP;
                        savedPosition = vector3_sub(savedPosition, vector3_new(x, y, 0.0f));
                        
                        //printf("num collided: %d\n", numTris);
                    }
                        
                    index++;
                }
                
                //printf("Intersects %s\n", fnIPOGetName(ipo));
            }
            //
            //        /* Calculate the angle from start to end vertex */
            //        const float angle = atan2f(start.y - end.y, start.x - end.x);
            //        const int iterations = 1;
            //
            //#define STEP 0.1f
            //        for (unsigned int i = 0; i < iterations; i++)
            //        {
            //#define ANGLE_LIMIT (M_PI / 4.0f)
            //            for (float π = -ANGLE_LIMIT; π < ANGLE_LIMIT; π += 0.1f)
            //            {
            //                const float x = cos(π + angle);
            //                const float y = sin(π + angle);
            //                tdstVector3D v = vector3_new(start.x - x, start.y - y, 1.3f);
            //
            //                solver->points[solver->numPoints++] = v;
            //            }
            //
            //            start = vector3_sub(start, vector3_new(cos(angle) * STEP, sin(angle) * STEP, 0.0f)); //vector3_new(start.x - x, start.y - y, 1.3f);
            //
            //
            //        }
        }
    }
    
    //printf("angle: %f\n", angle);
    
    /* Solve geometry recursively */
    superobject_for_each(g, child) solveGeometry(child, solver);
}
