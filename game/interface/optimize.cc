#include <cmath>

#include "structure.hh"
#include "interface.hh"
#include "hook.hh"
#include "log.hh"

#pragma mark - Helper functions

/** fnPointInTriangle: test if point lies in triangle **/
static auto pointInTriangle(stVector3D P, stVector3D A, stVector3D B, stVector3D C, stVector3D N) -> bool {
    stVector3D p0 = (A - P);
    stVector3D p1 = (B - P);
    stVector3D p2 = (C - P);
    
    #define M_INV_SQRT_3 5.773502691896259E-1
    #define SameSign(a,b) ((a >= 0.0f) == (b >= 0.0f))
    
    #define MSM(a,b,c,d) ((a)*(b) - (c)*(d))
    #define dXY(a,b) MSM(a.x, b.y, a.y, b.x)
    #define dYZ(a,b) MSM(a.y, b.z, a.z, b.y)
    #define dZX(a,b) MSM(a.z, b.x, a.x, b.z)
    
    float det = 0.0f;
    if (std::fabs(N.z) >= M_INV_SQRT_3) {
        det = dXY(p0, p1);
        if (SameSign(det, N.z)) {
            det = dXY(p1, p2);
            if (SameSign(det, N.z)) {
                det = dXY(p2, p0);
                return SameSign(det, N.z);
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        if (std::fabs(N.x) >= M_INV_SQRT_3) {
            det = dYZ(p0, p1);
            if (SameSign(det, N.x)) {
                det = dYZ(p1, p2);
                if (SameSign(det, N.x)) {
                    det = dYZ(p2, p0);
                    return SameSign(det, N.x);
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            det = dZX(p0, p1);
            if (SameSign(det, N.y)) {
                det = dZX(p1, p2);
                if (SameSign(det, N.y)) {
                    det = dZX(p2, p0);
                    return SameSign(det, N.y);
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
    }
}


#pragma mark - Optimize

template<class T> const T& min(const T& a, const T& b) { return (((a) < (b)) ? (a) : (b)); }
template<class T> const T& max(const T& a, const T& b) { return (((a) > (b)) ? (a) : (b)); }

namespace interface {
  
  static auto MTH4D_M_vMulMatrixVector() -> void {
    stVector3D *dst = pointer<stVector3D>GPR(29);
    stMatrix4D *T   = pointer<stMatrix4D>GPR(30);
    stVector3D *src = pointer<stVector3D>GPR(31);
    
    stMatrix4D m = *T;
    stVector3D s = *src;
    
    stVector3D result;
    result.x = (m.m20 * s.z) + (m.m00 * s.x) + (m.m10 * s.y) + m.m30;
    result.y = (m.m21 * s.z) + (m.m01 * s.x) + (m.m11 * s.y) + m.m31;
    result.z = (m.m22 * s.z) + (m.m02 * s.x) + (m.m12 * s.y) + m.m32;
    
    *dst = result;
    
    uint32_t sp = GPR(1) & 0x7FFFFFFF; /* sp = r1 */
    LR = GPR(0) = (*(uint32*)(memory::baseAddress + sp + 0x24)); /* lr = r0 */
    GPR(29) = (*(uint32*)(memory::baseAddress + sp + 0x14));
    GPR(30) = (*(uint32*)(memory::baseAddress + sp + 0x18));
    GPR(31) = (*(uint32*)(memory::baseAddress + sp + 0x1C));
    GPR(1) += 0x20;
    NPC = LR;
  }
  
//  static auto MTH4D_M_vMulMatrixVector() -> void {
//    stVector3D *dst = pointer<stVector3D, true>GPR(29);
//    stMatrix4D *T   = pointer<stMatrix4D, true>GPR(30);
//    stVector3D *src = pointer<stVector3D, true>GPR(31);
//
//    stMatrix4D m = T->hostByteOrder();
//    stVector3D s = src->host();
//
//    stVector3D result;
//    result.x = (m.m20 * s.z) + (m.m00 * s.x) + (m.m10 * s.y) + m.m30;
//    result.y = (m.m21 * s.z) + (m.m01 * s.x) + (m.m11 * s.y) + m.m31;
//    result.z = (m.m22 * s.z) + (m.m02 * s.x) + (m.m12 * s.y) + m.m32;
//
//    *dst = result.game();
//
//    uint32_t sp = GPR(1) & 0x7FFFFFFF; /* sp = r1 */
//    LR = GPR(0) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x24)); /* lr = r0 */
//    GPR(29) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x14));
//    GPR(30) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x18));
//    GPR(31) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x1C));
//    GPR(1) += 0x20;
//    NPC = LR;
//  }
  
  
  static auto POS_fn_vCopyMatrix() -> void {
    stTransform *dst = pointer<stTransform>GPR(3);
    stTransform *src = pointer<stTransform>GPR(4);
    *dst = *src;
    NPC = LR;
  }
  
  static auto POS_fn_vMulMatrixVertex() -> void {
    stVector3D *result = pointer<stVector3D>GPR(3);
    stTransform *T = pointer<stTransform>GPR(4);
    stVector3D *v1 = pointer<stVector3D>GPR(5);
    *result = T->matrix * (*v1);
    NPC = LR;
  }
  
  static auto INT_fn_bDetectIntersectSegmentWithTriangle() -> void {
    //printf("ine\n");
    NPC = LR;
    return;
    
    stVector3D S0;
    stVector3D SV;
    stVector3D A;
    stVector3D B;
    stVector3D C;
    stVector3D N;
    float dP;
    
    float dot = N.dot(SV);
    if (dot < 0.0f) {
      float d = N.dot(S0) + dP;
      float t = -(d / dot);
      if (t >= 0.0f && t <= 1.0f) {
        stVector3D hit = S0 + (SV * t);
        // return pointInTriangle(hit, A, B, C, N);
      } else {
        // return false;
      }
    } else {
      // return false;
    }
  }
  
  static auto INT_fn_bIntersectSegmentWithTriangle() -> void {
    //printf("ine\n");
    NPC = LR;
    return;
    
    stVector3D S0;
    stVector3D SV;
    stVector3D A;
    stVector3D B;
    stVector3D C;
    stVector3D N;
    float dP;
    
    float dot = N.dot(SV);
    if (dot < 0.0f) {
      float d = N.dot(S0) + dP;
      float t = -(d / dot);
      if (t >= 0.0f && t <= 1.0f) {
        stVector3D hit = S0 + (SV * t);
        // return pointInTriangle(hit, A, B, C, N);
      } else {
        // return false;
      }
    } else {
      // return false;
    }
  }
  
  static auto replace1() -> void { NPC = LR; }
  static auto replace2() -> void { NPC = LR; }
  static auto replace3() -> void { NPC = LR; }
  
//  auto INT_fn_bIntersectBoxWithBox(stVector3D *p_stMinPoint1, stVector3D *p_stMaxPoint1, stVector3D *p_stMinPoint2, stVector3D *p_stMaxPoint2) -> bool {
//    stVector3D min1 = p_stMinPoint1->host();
//    stVector3D max1 = p_stMaxPoint1->host();
//    stVector3D min2 = p_stMinPoint2->host();
//    stVector3D max2 = p_stMaxPoint2->host();
//    return max(min1.x, min2.x) <= min(max1.x, max2.x) &&
//           max(min1.y, min2.y) <= min(max1.y, max2.y) &&
//           max(min1.z, min2.z) <= min(max1.z, max2.z);
//  }
  
  auto INT_fn_bIntersectBoxWithBox(stVector3D *p_stMinPoint1, stVector3D *p_stMaxPoint1, stVector3D *p_stMinPoint2, stVector3D *p_stMaxPoint2) -> bool {
    stVector3D min1 = *p_stMinPoint1;
    stVector3D max1 = *p_stMaxPoint1;
    stVector3D min2 = *p_stMinPoint2;
    stVector3D max2 = *p_stMaxPoint2;
    return max(float(min1.x), float(min2.x)) <= min(float(max1.x), float(max2.x)) &&
           max(float(min1.y), float(min2.y)) <= min(float(max1.y), float(max2.y)) &&
           max(float(min1.z), float(min2.z)) <= min(float(max1.z), float(max2.z));
  }
  
//  static auto COL_fn_vExploreRecursiveOctreeWithBox(stOctreeNode *p_stNodeToBeExplored, stVector3D *p_stMinSituation, stVector3D *p_stMaxSituation, uint32_t *d_pstSelectedNode, short *p_xNumberOfSelectedNodes) -> void {
//    if (!p_stNodeToBeExplored) return;
//
//    if (INT_fn_bIntersectBoxWithBox( p_stMinSituation, p_stMaxSituation, &p_stNodeToBeExplored->min, &p_stNodeToBeExplored->max)) {
//      if (p_stNodeToBeExplored->children) {
//        for (int i = 0; i < 8; i++) {
//          stOctreeNode *node = pointer<stOctreeNode>(*(uint32_t*)(memoryBase + p_stNodeToBeExplored->children.address() + i * 4));
//          COL_fn_vExploreRecursiveOctreeWithBox ( node, p_stMinSituation, p_stMaxSituation, d_pstSelectedNode, p_xNumberOfSelectedNodes );
//        }
//      } else {
//        if (p_stNodeToBeExplored->faceIndices) {
//          /* on recupere les noeuds */
//          if ( *p_xNumberOfSelectedNodes < 100 ) {
//            uint32_t addr = game_byteorder_32(((uint32_t)((long)p_stNodeToBeExplored - (long)memoryBase) | 0x80000000));
//            d_pstSelectedNode[*p_xNumberOfSelectedNodes] = addr;
//            (*p_xNumberOfSelectedNodes) ++;
//          }
//        }
//      }
//    }
//  }
  
//  static auto COL_fn_vExploreOctreeWithBox() -> void {
//
//    stOctree *octree = pointer<stOctree, true>GPR(3);
//    stVector3D *minPoint = pointer<stVector3D, true>GPR(4);
//    stVector3D *maxPoint = pointer<stVector3D, true>GPR(5);
//    uint32_t *selectedNodes = pointer<stOctreeNode*, true>GPR(6);
//    short *numSelectedNodes = pointer<short, true>GPR(7);
//
//    *numSelectedNodes = host_byteorder_16(0);
//
//    COL_fn_vExploreRecursiveOctreeWithBox((stOctreeNode*)octree->root, minPoint, maxPoint, selectedNodes, numSelectedNodes);
//
//    //printf("num selected: %d\n", *numSelectedNodes);
//
//    // 803dc740
//
//    *numSelectedNodes = game_byteorder_16(*numSelectedNodes);
//
//
//    NPC = LR;
//  }
  
  static void COL_fn_vNewStaticCollisionForCharacter() {
    //printf("col static: %X\n", GPR(26));
  }
  
  static auto fn_vDisplayAll() -> void {
    NPC = LR;
  }
  
  void applyOptimizations() {
    
    info(BOLD COLOR_GREEN "Optimizations loaded\n" COLOR_DEFAULT);
    
    //hook<0x8002e974>(replace1, hookType::replace);
    
    // COL
    //hook<0x80029358>(COL_fn_vExploreOctreeWithBox, hookType::replace);
    
    // MTH
    //hook<0x800777e0>(MTH4D_M_vMulMatrixVector, hookType::replace);
    
    // POS
    //hook<0x800787dc>(POS_fn_vCopyMatrix, hookType::replace);
    
    
    //hook<0x8007568c>(POS_fn_vMulMatrixVertex, hookType::replace);
    //hook<0x80035468>(COL_fn_vNewStaticCollisionForCharacter);
    
    //hook<0x8002b258>(INT_fn_bDetectIntersectSegmentWithTriangle, hookType::replace);
    
    
    //hook<0x8002b0d0>(INT_fn_bIntersectSegmentWithTriangle, hookType::replace);
    
//    hook<0x800832fc>(replace1, hookType::replace);
//    hook<0x8007e9d8>(replace2, hookType::replace);
//    hook<0x8007d714>(replace3, hookType::replace);
    
    //hook<0x80036174>(fn_vDisplayAll, hookType::replace);
    //hook_create(0x80054498, "hook_display_fix", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_r_display_fix);
    
    
  }
}
