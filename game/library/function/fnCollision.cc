/***************************************************
 * This file is not part of the structure library. *
 ***************************************************/

#include "stVector3D.hh"
#include "fnPrimIntersections.c"

//struct CollisionContext
//{
//    struct Sphere sphere;
//    struct Triangle triangle;
//    
//    tdstVector3D movementStart;
//    tdstVector3D movementDirection;
//    tdstVector3D movementEnd;
//};
//
//static bool fnSphereTriangleCollision(struct CollisionContext *c)
//{
//    const float plane = -vector3_dot(c->triangle.N, c->triangle.A);
//    /* Enlarge sphere */
//    
//    tdstVector3D TNorm = vector3_mulf(c->triangle.N, c->sphere.radius);
//    tdstVector3D SegmentStart = vector3_add(c->sphere.center, TNorm);
//    tdstVector3D SegmentExtent = vector3_mulf(TNorm, -2.0f);
//    
//    if (fnSegmentTriangleIntersection(SegmentStart, SegmentExtent, c->triangle.A, c->triangle.B, c->triangle.C, c->triangle.N, plane))
//    {
//        tdstVector3D movement;
//        
//        /* use initial sphere */
//        TNorm = vector3_mulf(c->triangle.N, c->sphere.radius);
//        SegmentExtent = vector3_mulf(TNorm, -2.0f);
//        movement = vector3_negate(c->movementDirection);
//        SegmentStart = vector3_add(c->movementEnd, TNorm); /* Segment start point */
//        
////        if (fnSegmentTriangleIntersection(SegmentStart, SegmentExtent, c->triangle.A, c->triangle.B, c->triangle.C, c->triangle.N, plane))
////        {
////            tdstVector3D t0a = vector3_sub(c->triangle.A, c->movementStart);
////            tdstVector3D t1a = vector3_sub(c->triangle.A, c->movementEnd);
////            float dot = vector3_dot(t0a, c->triangle.N);
////            float d = dot + c->sphere.radius;
////
////            if (d <= 0.0f)
////                return false;
////
////            tdstVector3D TDist = vector3_mulf(c->triangle.N, d);
////            /* matrix mul, probably 1:1 regardless */
////
////            /* Hit */
////            tdstVector3D THit = vector3_mulf(c->triangle.N, dot);
////            THit = vector3_add(THit, c->movementStart);
////
////            if (!fnPointInTriangle(THit, c->triangle.A, c->triangle.B, c->triangle.C, c->triangle.N))
////                return false;
////
////            /* There is collision */
////
////        }
////        else
////        {
////
////        }
//        
//        /* Collision */
//        return true;
//    }
//    
//    /* No collision */
//    return false;
//}
