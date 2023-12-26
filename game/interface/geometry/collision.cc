#include "game.hh"
#include "memory.hh"
#include "stCollideObject.hh"
#include "fnPrimIntersections.c"
#include "solver.hh"
#include "stDynamics.hh"
#include "stInstantiatedPhysicalObject.hh"

#define ZDM_RADIUS  0.9f
#define MAX_SLOPE M_SQRT1_2
#define MIN_SLOPE -M_SQRT1_2

static void AddCollision(struct CollisionRecord *record, struct Collision c)
{
    if (record->numCollisions + 1 < MAX_COLLISIONS)
    {
        if (c.staticType == COLLISION_TYPE_H_EDGE || c.staticType == COLLISION_TYPE_L_EDGE)
        {
        }
        
        for (unsigned int i = 0; i < record->numCollisions; i++)
        {
            struct Collision *coll = &record->collisions[i];
            
            if (c.normal.x < coll->normal.x)
            {
                
            }
        }
        
        record->collisions[record->numCollisions++] = c;
    }
}

static bool CollideSphereWithTriangle(const float radius,
                                      const float radiusL,
                                      const tdstVector3D T0,
                                      const tdstVector3D T0L,
                                      const tdstVector3D Dir,
                                      const tdstVector3D T1,
                                      const tdstVector3D TA,
                                      const tdstVector3D TB,
                                      const tdstVector3D TC,
                                      const tdstVector3D TN,
                                      const tdstMatrix4D StaticMatrix,
                                      struct CollisionRecord *record)
{
    const float plane = -vector3_dot(TN, TA);
    
    tdstVector3D TNorm = vector3_mulf(TN, radiusL);
    tdstVector3D SegmentStart = vector3_add(T0L, TNorm);
    tdstVector3D SegmentExtent = vector3_mulf(TNorm, -2.0f);

    if (fnSegmentTriangleIntersection(SegmentStart, SegmentExtent, TA, TB, TC, TN, plane))
    {
        TNorm = vector3_mulf(TN, radius);
        SegmentExtent = vector3_mulf(TNorm, -2.0f);
        SegmentStart = vector3_add(T1, TNorm);
        tdstVector3D Movement = vector3_negate(Dir);
        
        

        if (fnSegmentTriangleIntersection(SegmentStart, SegmentExtent, TA, TB, TC, TN, plane))
        {
            tdstVector3D T0A = vector3_sub(TA, T0);
            tdstVector3D T1A = vector3_sub(TA, T1);
            float dot = vector3_dot(T0A, TN);
            float d = dot + radius;
            
            if (d <= 0.0f)
                return false;
            
            tdstVector4D TNd = vector4_mulf(vector4_new(TN.x, TN.y, TN.z, 1.0f), d);
            TNd.w = 1.0f; /* Make sure w = 1.0f */
            TNd = vector4_mul_matrix4(TNd, StaticMatrix);
            
            /* Hit */
            tdstVector3D THit = vector3_mulf(TN, dot);
            THit = vector3_add(THit, T0);
            
            if (!fnPointInTriangle(THit, TA, TB, TC, TN))
                return false;
            
            tdstVector4D hit = vector4_mul_matrix4(vector4_new(THit.x, THit.y, THit.z, 1.0f), StaticMatrix);
            tdstVector4D TNorm2 = vector4_mul_matrix4(vector4_new(TN.x, TN.y, TN.z, 1.0f), StaticMatrix);
            tdstVector4D Move = vector4_mul_matrix4(vector4_new(Movement.x, Movement.y, Movement.z, 1.0f), StaticMatrix);
            tdstVector4D End = vector4_mul_matrix4(vector4_new(T0.x, T0.y, T0.z, 1.0f), StaticMatrix);
            
            /* There is collision */
            struct Collision collision;
            collision.dynamicType = COLLISION_TYPE_SPHERE;
            collision.staticType = COLLISION_TYPE_TRIANGLE;
            collision.contact = vector3_new(hit.x, hit.y, hit.z);
            collision.normal = vector3_new(TNorm2.x, TNorm2.y, TNorm2.z);
            collision.translation = vector3_new(TNd.x, TNd.y, TNd.z);
            collision.movement = vector3_new(Move.x, Move.y, Move.z);
            collision.endpos = vector3_new(End.x, End.y, End.z);
            collision.triangle = (struct Triangle){ TA, TB, TC, TN };
            
            AddCollision(record, collision);
        }
        else
        {
            printf("Movement collision?\n");
        }

        /* Collision */
        return true;
    }

    /* No collision */
    return false;
}

void PrintMatrix(const tdstMatrix4D m)
{
    printf("%.2f  %.2f  %.2f  %.2f\n", m.m00, m.m01, m.m02, m.m03);
    printf("%.2f  %.2f  %.2f  %.2f\n", m.m10, m.m11, m.m12, m.m13);
    printf("%.2f  %.2f  %.2f  %.2f\n", m.m20, m.m21, m.m22, m.m23);
    printf("%.2f  %.2f  %.2f  %.2f\n", m.m30, m.m31, m.m32, m.m33);
    printf("\n");
}

bool
CollideSphereWithGeometry(const float dynamicSphereRadius,
                          const tdstMatrix4D dynamicSphereStart,
                          const tdstMatrix4D dynamicSphereEnd,
                          const tdstCollideObject *staticObject,
                          const tdstMatrix4D staticObjectMatrix,
                          struct CollisionRecord *record)
{
    const tdstMatrix4D staticObjectMatrixInv = matrix4_inverse(staticObjectMatrix);
    const tdstMatrix4D T0Matrix = matrix4_mul(staticObjectMatrixInv, dynamicSphereEnd);
    const tdstMatrix4D T1Matrix = matrix4_mul(staticObjectMatrixInv, dynamicSphereStart);
    
    /* Compute the maximum scale of the collide object. */
    const tdstVector3D vL1 = { staticObjectMatrix.m00, staticObjectMatrix.m01, staticObjectMatrix.m02 };
    const tdstVector3D vL2 = { staticObjectMatrix.m10, staticObjectMatrix.m11, staticObjectMatrix.m12 };
    const tdstVector3D vL3 = { staticObjectMatrix.m20, staticObjectMatrix.m21, staticObjectMatrix.m22 };
    const float l1 = vector3_dot(vL1, vL1);
    const float l2 = vector3_dot(vL2, vL2);
    const float l3 = vector3_dot(vL3, vL3);
    const float maxScale = 1.0f; //sqrt(max(max(l1, l2), l3)); /* 1.0 if the matrix contains no rotation component */
    
    tdstVector4D center = { dynamicSphereStart.m03, dynamicSphereStart.m13, dynamicSphereStart.m23, 1.0f };
    float radius = dynamicSphereRadius / maxScale;
    
    tdstVector4D T0 = vector4_mul_matrix4(center, T0Matrix);
    tdstVector4D T1 = vector4_mul_matrix4(center, T1Matrix);
    tdstVector4D dV = vector4_sub(T1, T0);
    tdstVector3D direction = vector3_new(dV.x, dV.y, dV.z);
    
    tdstVector4D T0L = T0;
    float radiusL = radius;
    
//    printf("T0:\n");
//    PrintMatrix(T0Matrix);
    
    
    /* Enlarge the sphere if necessary */
    const float sqd = vector3_dot(direction, direction);
    if (sqd >= 1e-3)
    {
        const float d = sqrtf(sqd);
        const float r = (d + radius * 2.0f) / 2.0f;
        const float c = (r - radius) / d;
        T0L = vector4_mulf(T0, c);
        radiusL = r;
    }
    
    const tdstVector3D* vertices = (const tdstVector3D*)pointer(staticObject->vertices);
    
    bool coll = false;
    
    int elementIndex = 0;
    const tdstCollideElementIndexedTriangles *element;
    while ((element = fnCollideObjectGetElementIndexedTriangles(staticObject, elementIndex)))
    {
        const tdstVector3D* normals = pointer(element->normals);
        const uint16* indices = pointer(element->faceIndices);
            
        for (int16 index = 0; index < host_byteorder_16(element->numFaces); index++)
        {
            uint16 idx0 = host_byteorder_16(*(indices + index * 3 + 0));
            uint16 idx1 = host_byteorder_16(*(indices + index * 3 + 1));
            uint16 idx2 = host_byteorder_16(*(indices + index * 3 + 2));
                
            const tdstVector3D Pa = vector3_host_byteorder(*(vertices + idx0));
            const tdstVector3D Pb = vector3_host_byteorder(*(vertices + idx1));
            const tdstVector3D Pc = vector3_host_byteorder(*(vertices + idx2));
            const tdstVector3D Na = vector3_host_byteorder(*(normals + index));
                
            const tdstVector4D TpA = vector4_mul_matrix4(vector4_new(Pa.x, Pa.y, Pa.z, 1.0f), staticObjectMatrix);
            const tdstVector4D TpB = vector4_mul_matrix4(vector4_new(Pb.x, Pb.y, Pb.z, 1.0f), staticObjectMatrix);
            const tdstVector4D TpC = vector4_mul_matrix4(vector4_new(Pc.x, Pc.y, Pc.z, 1.0f), staticObjectMatrix);
            
            const tdstVector3D T03 = vector3_new(T0.x, T0.y, T0.z);
            const tdstVector3D T0L3 = vector3_new(T0L.x, T0L.y, T0L.z);
            const tdstVector3D Dir3 = vector3_new(direction.x, direction.y, direction.z);
            const tdstVector3D T13 = vector3_new(T1.x, T1.y, T1.z);
            const tdstVector3D TA = vector3_new(TpA.x, TpA.y, TpA.z);
            const tdstVector3D TB = vector3_new(TpB.x, TpB.y, TpB.z);
            const tdstVector3D TC = vector3_new(TpC.x, TpC.y, TpC.z);
            
            if (CollideSphereWithTriangle(radius, radiusL, T03, T0L3, Dir3, T13, TA, TB, TC, Na, staticObjectMatrix, record))
                coll = true;
        }
        
        elementIndex++;
    }
    
    return coll;
}

void CollideSphereWithWorld(const float dynamicSphereRadius,
                            const tdstMatrix4D dynamicSphereStart,
                            const tdstMatrix4D dynamicSphereEnd,
                            const tdstSuperObject *root,
                            const tdstMatrix4D transform,
                            struct CollisionRecord *record)
{
    if (!root) return;
    
    /* Calculate the new transformation matrix */
    const tdstMatrix4D T = matrix4_mul(fnSuperobjectGetGlobalMatrix(root), transform);
    
    if (superobject_type(root) == superobject_type_ipo)
    {
        const tdstInstantiatedPhysicalObject* ipo = (const tdstInstantiatedPhysicalObject*)superobject_data(root);
        if (ipo)
        {
            const tdstCollideObject* zdr = fnIPOGetCollideObject(ipo);
            if (zdr)
            {
                CollideSphereWithGeometry(dynamicSphereRadius, dynamicSphereStart, dynamicSphereEnd, zdr, T, record);
            }
        }
    }
    
    superobject_for_each(root, child) CollideSphereWithWorld(dynamicSphereRadius, dynamicSphereStart, dynamicSphereEnd, child, T, record);
}

static tdstVector3D VectorRotation(tdstVector3D v, tdstMatrix4D R)
{
    R.m30 = R.m31 = R.m32 = 0.0f;
    R.m33 = 1.0f;
    tdstVector4D v2 = vector4_mul_matrix4(vector4_new(v.x, v.y, v.z, 1.0f), R);
    return vector3_new(v2.x, v2.y, v2.z);
}

static void ProposeSpeed(tdstDynamics *d, tdstMatrix4D *mat, tdstMatrix4D *matInv, float t)
{
    tdstMatrix4D current = (d->base.currentTransform.matrix);
    tdstMatrix4D previous = (d->base.previousTransform.matrix);
    tdstVector3D move = vector3_mulf(d->base.proposedSpeed, t);
    
    tdstVector3D currentPos = game_matrix4_position(current);
    tdstVector3D previousPos = game_matrix4_position(previous);
    tdstVector3D translation = vector3_sub(currentPos, previousPos);
    
    tdstMatrix4D inv;
    if (vector3_length(translation) > 0)
    {
        *matInv = matrix4_inverse(*mat);
        translation = VectorRotation(translation, *matInv);
    }
    
    translation = move;
    if (vector3_length(translation) > 0)
        translation = VectorRotation(translation, *mat);
    
    tdstVector3D *pos = (tdstVector3D*)&d->base.currentTransform.matrix.m30;
    *pos = vector3_add(previousPos, translation);
}

static float ScaleConversion (float v)
{
    float t;
    if (v >= 1.0f) return v;
    if (v > 0.0f)
    {
        t = 1.0f - v;
        t = t * t;
        t = t * t;
        t = 1.0f - t;
    }
    else
        t = 0.0f;
    return t;
}

static void Inertia(tdstDynamics *d, tdstMatrix4D *mat, tdstMatrix4D *matInv, float t)
{
    float x = ScaleConversion(0.0f);
    float y = ScaleConversion(0.0f);
    float z = ScaleConversion(0.0f);
}

static void Gravity(tdstDynamics *d, float t)
{
    float gravity = 9.81f;
    tdstMatrix4D current = (d->base.currentTransform.matrix);
    tdstMatrix4D previous = (d->base.previousTransform.matrix);
    tdstVector3D currentPos = game_matrix4_position(current);
    tdstVector3D previousPos = game_matrix4_position(previous);
    
    tdstVector3D speed = d->base.previousSpeed;
    
    gravity = gravity * -1.0f;
    gravity = gravity * t;
    gravity = gravity + speed.z;
    gravity = gravity * t;
    
    tdstVector3D *pos = (tdstVector3D*)&d->base.currentTransform.matrix.m30;
    pos->z = previousPos.z + gravity;
}

static void SpeedCalculation(tdstDynamics *d, float t)
{
    tdstVector3D currentPos = game_matrix4_position(d->base.currentTransform.matrix);
    tdstVector3D previousPos = game_matrix4_position(d->base.previousTransform.matrix);
    d->base.previousSpeed = vector3_mulf(vector3_sub(currentPos, previousPos), 1.0f / t);
}

static void SpeedAdjustment(tdstDynamics *d, float t)
{
    tdstVector3D currentPos = game_matrix4_position(d->base.currentTransform.matrix);
    tdstVector3D previousPos = game_matrix4_position(d->base.previousTransform.matrix);
    tdstVector3D translation = vector3_sub(currentPos, previousPos);
    if (vector3_length(translation) > 0.0f)
        translation = vector3_normalize(translation);
    
    float dot = vector3_dot(translation, d->base.previousSpeed);
    translation = vector3_mulf(translation, dot * t);
    
    tdstVector3D *pos = (tdstVector3D*)&d->base.currentTransform.matrix.m30;
    *pos = vector3_add(previousPos, translation);
}

static bool Collision(tdstDynamics *d, tdstMatrix4D start, tdstMatrix4D end)
{
    float radius = ZDM_RADIUS;
    
    //printf("collision reached\n");
    
    tdstVector3D up = vector3_new(0.0f, 0.0f, 1.0f);
    
    
    printf("start:\n");
    PrintMatrix(start);
    printf("end:\n");
    PrintMatrix(end);
    
    struct CollisionRecord record = {};
    CollideSphereWithWorld(radius, start, end, father_sector, matrix4_identity, &record);
    printf("num coll: %d\n", record.numCollisions);
    if (record.numCollisions > 0)
    {
        tdstVector3D currentPos = game_matrix4_position(d->base.currentTransform.matrix);
        tdstVector3D previousPos = game_matrix4_position(d->base.previousTransform.matrix);
        tdstVector3D translation = vector3_sub(currentPos, previousPos);
        
        /* Compute the final translation */
        for (unsigned int i = 0; i < record.numCollisions; i++)
        {
            struct Collision *c = &record.collisions[i];
            /* TODO: Check rate */
            float rate = 0.5f; // default ground rate?
            if (rate > 1.0f)
            {
                
            }
            else if (rate >= 0.0f)
            {
                c->translation = vector3_mulf(c->translation, 1.0f - rate);
            }
        }
        
        long obstacle = 2;
        float previousRate = 2.0f;
        for (unsigned int i = 0; i < record.numCollisions; i++)
        {
            struct Collision *c = &record.collisions[i];
            if (c)
            {
                if (i < obstacle || c->rate == previousRate)
                {
                    tdstVector3D normal = c->normal;
                    previousRate = c->rate;
                    
                    /* Check if we collided with a ground or a wall */
                    float dot = vector3_dot(normal, up);
                    if (dot > MAX_SLOPE)
                    {
                        printf("ground\n");
                    }
                    else if (dot > MIN_SLOPE)
                    {
                        printf("wall\n");
                    }
                }
            }
        }
        
    }
    
    printf("\n");
    
    return record.numCollisions > 0;
}

static void BaseObstacleManagement(tdstDynamics *d, float dt)
{
    tdstMatrix4D T_current = d->base.currentTransform.matrix;
    tdstMatrix4D T_previous = d->base.previousTransform.matrix;
    printf("obs manag\n");
    if (Collision(d, T_previous, T_current))
    {

    }
}

static void GroundMovement(tdstDynamics *d, float dt)
{
    float minSlope      = -*(float*)&d->base.slopeLimit;
    float maxSlope      =  *(float*)&d->base.slopeLimit;
    float wallLimit     = -*(float*)&d->base.slopeCosine;
    float groundLimit   =  *(float*)&d->base.slopeCosine;
    
    tdstVector3D currentPos = game_matrix4_position(d->base.currentTransform.matrix);
    tdstVector3D previousPos = game_matrix4_position(d->base.previousTransform.matrix);
    
    tdstVector3D vertical = vector3_new(0.0f, 0.0f, 1.0f);
    tdstVector3D adjust = vector3_new(0.0f, 0.0f, 0.0f);
    tdstVector3D normal = vector3_new(0.0f, 0.0f, 0.0f);
    tdstVector3D push = vector3_new(0.0f, 0.0f, 0.0f);
    tdstVector3D move = vector3_sub(currentPos, previousPos);
    tdstVector3D normalMove, tangentMove;
    tdstVector3D normalizedTangentMove = vector3_new(0.0f, 0.0f, 0.0f);
    tdstVector3D absoluteTangentMove = vector3_new(0.0f, 0.0f, 0.0f);
    
    normal = vertical;
    
    float dot = vector3_dot(move, normal);
    normalMove = vector3_mulf(normal, dot);
    tangentMove = vector3_sub(move, normalMove);
    float normTangentMove = vector3_length(tangentMove);
    
    if (normTangentMove > 1e-3)
        normalizedTangentMove = vector3_mulf(tangentMove, 1.0f / normTangentMove);
    
    bool highSpeed = normTangentMove > ZDM_RADIUS;
    absoluteTangentMove = tangentMove;
    
    //printf("normtang, normMovelen: %f %f\n", normTangentMove, vector3_length(normalMove));
//    printf("mov: %.2f %.2f %.2f\n", move.x, move.y, move.z);
//    //printf("prv: %.2f %.2f %.2f\n", previousPos.x, previousPos.y, previousPos.z);
//    printf("cur: %.2f %.2f %.2f\n", currentPos.x, currentPos.y, currentPos.z);
//    printf("\n");
    
    
    #define NOMOVEMENT 1e-10
    if (normTangentMove < NOMOVEMENT && vector3_length(normalMove) > NOMOVEMENT)
    {
        BaseObstacleManagement(d, dt);
    }
    
    
}

static void Walk(tdstDynamics *d, float t)
{
    tdstVector3D tmp;
    tdstVector3D currentPos = game_matrix4_position(d->base.currentTransform.matrix);
    tdstVector3D previousPos = game_matrix4_position(d->base.previousTransform.matrix);
    tdstVector3D translation = vector3_sub(currentPos, previousPos);
    
    tdstVector3D *currentPosP = (tdstVector3D*)&d->base.currentTransform.matrix.m30;
    tdstVector3D *previousPosP = (tdstVector3D*)&d->base.previousTransform.matrix.m30;
    
    float C = 0.0f;
    unsigned int n = ((vector3_length(translation) / ZDM_RADIUS) * C) + 1.0f;
    
    /* Slow mode */
    float inv = 1.0f / (float)n;
    tmp = vector3_sub(currentPos, previousPos);
    tmp = vector3_mulf(tmp, inv);
    *currentPosP = vector3_add(previousPos, tmp);
    d->base.previousSpeed = vector3_mulf(d->base.previousSpeed, inv);
    
    GroundMovement(d, t);
    
    /* Default mode */
    tmp = vector3_sub(currentPos, previousPos);
    tmp = vector3_mulf(tmp, n);
    *previousPosP = vector3_sub(currentPos, tmp);
    d->base.previousSpeed = vector3_mulf(d->base.previousSpeed, n);
}

#define FRAMELENGTH (1.0f / 60.0f)
void TransformAndCollide(tdstDynamics *d, float dt)
{
    unsigned n = (dt / FRAMELENGTH) + 0.5f;
    
//    tdstVector3D cur = game_matrix4_position(d->base.currentTransform.matrix);
//    tdstVector3D prv = game_matrix4_position(d->base.previousTransform.matrix);
//
//    printf("crr: %.2f %.2f %.2f\n", cur.x, cur.y, cur.z);
//    printf("prv: %.2f %.2f %.2f\n\n", prv.x, prv.y, prv.z);
    
    tdstMatrix4D previousMatrix = d->base.previousTransform.matrix;
    tdstMatrix4D currentMatrix = d->base.currentTransform.matrix;
    tdstMatrix4D previousPrevious = d->complex.previousMatrixPrevious.matrix;
    
    float t = FRAMELENGTH;
    for (unsigned int i = 0; i < n; i++)
    {
        tdstMatrix4D refMatrix = matrix4_identity;
        tdstMatrix4D invRefMatrix = matrix4_identity;
        //ProposeSpeed(d, &refMatrix, &invRefMatrix, t);
//        Inertia(d, &refMatrix, &invRefMatrix, t);
//        Gravity(d, t);
//        SpeedCalculation(d, t);
//
//        if (vector3_length(d->base.previousSpeed) <= 0) i = n;
    }
    
    //SpeedAdjustment(d, t);
    
    /* Now, collide with the world. */
    Walk(d, t);
    
    
    tdstVector3D stablePosition;
    
    //ProposeSpeed(d, <#float t#>)
}
