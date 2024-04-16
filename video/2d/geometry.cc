static auto drawGeometryRecursive(pointer<stSuperObject> object, const stMatrix4D transform, ImDrawList* drawlist, ImVec2 off, ImVec2 sz, ImVec4 color) -> void {
  if (!object) return;
  
  stMatrix4D T = object->globalTransform->matrix * transform;
  //try {
    
    
    if (object->type == stSuperObject::type::IPO) {
      try {
        pointer<stCollideObject> zdr = object->ipo->physicalObject->physicalCollideset->zdr;
        
        for (int i = 0; i < zdr->numElements; i++) {
          int16_t type = zdr->elementTypes[i];
          if (type == stCollideObject::type::IndexedTriangles) {
            pointer<stCollideElementIndexedTriangles> element = zdr->elements[i];
            stVector3D* vertices = zdr->vertices;
            uint16* indices = element->faceIndices;
            
            for (int16_t index = 0; index < element->numFaces; index++) {
              uint16 idx0 = *(indices + index * 3 + 0);
              uint16 idx1 = *(indices + index * 3 + 1);
              uint16 idx2 = *(indices + index * 3 + 2);
              
              stVector3D Pa = *(vertices + idx0);
              stVector3D Pb = *(vertices + idx1);
              stVector3D Pc = *(vertices + idx2);
              
              stVector4D TpA = T * Pa;
              stVector4D TpB = T * Pb;
              stVector4D TpC = T * Pc;
              
              ImVec4 coord_a = gui::projectWorldCoordinate(TpA.xyz());
              ImVec4 coord_b = gui::projectWorldCoordinate(TpB.xyz());
              ImVec4 coord_c = gui::projectWorldCoordinate(TpC.xyz());
              
              if (coord_a.w >= 0 && coord_b.w >= 0 && coord_c.w >= 0) {
                //printf("adad\n");
                /* Calculate the triangle center for highlighting of the face */
                //tdstVector3D center = vector3_new((TpA.x + TpB.x + TpC.x) / 3.0f, (TpA.y + TpB.y + TpC.y) / 3.0f, (TpA.z + TpB.z + TpC.z) / 3.0f);
                //tdstVector4D centerT = vector4_mul_matrix4(vector4_new(center.x, center.y, center.z, 1.0f), T);
                /* Distance fading factor */
                //const float alpha = 1.0f / (vector3_length(vector3_sub(center, player)) + 1.0f);
                
                if ((coord_a.x >= 0.0f && coord_a.x <= 1.0f && coord_a.y >= 0.0f && coord_a.y <= 1.0f)
                ||  (coord_b.x >= 0.0f && coord_b.x <= 1.0f && coord_b.y >= 0.0f && coord_b.y <= 1.0f)
                ||  (coord_c.x >= 0.0f && coord_c.x <= 1.0f && coord_c.y >= 0.0f && coord_c.y <= 1.0f)) {
                
                  auto proj = [](ImVec2 screenPos) {
                    return ImPlot::PlotToPixels(ImPlotPoint(640.0f - screenPos.x * 640.0f, 528.0f - screenPos.y * 528.0f));
                  };
                  
                  ImVec2 pos1 = proj(ImVec2(coord_a.x, coord_a.y));
                  ImVec2 pos2 = proj(ImVec2(coord_b.x, coord_b.y));
                  ImVec2 pos3 = proj(ImVec2(coord_c.x, coord_c.y));
                  
                
                  
                  ImColor outline = color;
                  //outline.Value.w = alpha * (1.0f - level_geometry_fade_factor) + level_geometry_fade_factor;
                  
                  ImColor base = outline;
                  base.Value.w *= 0.35f;
                  
                  drawlist->AddTriangle(pos1, pos2, pos3, outline);
                  drawlist->AddTriangleFilled(pos1, pos2, pos3, base);
                }
                
                //              stVector3D normal = vector3_new(TnA.x, TnA.y, TnA.z);
                //              ImVec4 coord_center = project_world_coordinate(vector4_mul_matrix4(vector4_new(center.x, center.y, center.z, 1.0f), T));
                //              ImVec4 coord_norm = project_world_coordinate(vector3_add(center, normal));
                //              ImVec2 poscenter = ImVec2(off.x + sz.x - coord_center.x * sz.x, off.y + coord_center.y * sz.y);
                //              ImVec2 posnorm = ImVec2(off.x + sz.x - coord_norm.x * sz.x, off.y + coord_norm.y * sz.y);
                
                /* To the center vector, add the offset of the normal. */
                //if (normals && 1.0f / vector3_length(vector3_sub(Na, center)) < 1.0f) drawlist->AddLine(poscenter, posnorm, ImColor(normal.x, normal.y, normal.z, 1.0f));
              }
            }
          }
        }
      } catch (...) {
        
      }
    }
    
//  } catch (...) {
//    printf("raad\n");
//  }
//
    
    object->recurse([&](stSuperObject* obj, void *data) {
      try { drawGeometryRecursive(obj, T, drawlist, off, sz, color); } catch (...) {}
    }, nullptr);
  
}

static auto drawGeometryTopdown(pointer<stSuperObject> object, const stMatrix4D transform, ImDrawList* drawlist, ImVec2 off, ImVec2 sz, ImVec4 color) -> void {
  if (!object) return;
  
  stMatrix4D T = object->globalTransform->matrix * transform;
  try {
    
    
    if (object->type == stSuperObject::type::IPO) {
      try {
        pointer<stCollideObject> zdr = object->ipo->physicalObject->physicalCollideset->zdr;
        
        for (int i = 0; i < zdr->numElements; i++) {
          int16_t type = zdr->elementTypes[i];
          if (type == stCollideObject::type::IndexedTriangles) {
            pointer<stCollideElementIndexedTriangles> element = zdr->elements[i];
            stVector3D* vertices = zdr->vertices;
            uint16* indices = element->faceIndices;
            
            for (int16_t index = 0; index < element->numFaces; index++) {
              uint16 idx0 = *(indices + index * 3 + 0);
              uint16 idx1 = *(indices + index * 3 + 1);
              uint16 idx2 = *(indices + index * 3 + 2);
              
              stVector3D Pa = *(vertices + idx0);
              stVector3D Pb = *(vertices + idx1);
              stVector3D Pc = *(vertices + idx2);
              
              stVector4D TpA = T * Pa;
              stVector4D TpB = T * Pb;
              stVector4D TpC = T * Pc;
              
//              ImVec4 coord_a = gui::projectWorldCoordinate(TpA.xyz());
//              ImVec4 coord_b = gui::projectWorldCoordinate(TpB.xyz());
//              ImVec4 coord_c = gui::projectWorldCoordinate(TpC.xyz());
              
              //if (coord_a.w >= 0 && coord_b.w >= 0 && coord_c.w >= 0) {
//                if ((coord_a.x >= 0.0f && coord_a.x <= 1.0f && coord_a.y >= 0.0f && coord_a.y <= 1.0f)
//                ||  (coord_b.x >= 0.0f && coord_b.x <= 1.0f && coord_b.y >= 0.0f && coord_b.y <= 1.0f)
//                ||  (coord_c.x >= 0.0f && coord_c.x <= 1.0f && coord_c.y >= 0.0f && coord_c.y <= 1.0f)) {
                
                  auto proj = [](ImVec2 screenPos) {
                    return ImPlot::PlotToPixels(ImPlotPoint(640.0f - screenPos.x * 640.0f, 528.0f - screenPos.y * 528.0f));
                  };
                  
//                  ImVec2 pos1 = proj(ImVec2(coord_a.x, coord_a.y));
//                  ImVec2 pos2 = proj(ImVec2(coord_b.x, coord_b.y));
//                  ImVec2 pos3 = proj(ImVec2(coord_c.x, coord_c.y));
                  
//              ImVec2 pos1 = proj(ImVec2(TpA.x, TpA.y));
//              ImVec2 pos2 = proj(ImVec2(TpB.x, TpB.y));
              //ImVec2 pos3 = proj(ImVec2(coord_c.x, coord_c.y));
                
              
              ImVec2 p1 = proj(ImVec2(Pa.x, Pa.y));
              ImVec2 p2 = proj(ImVec2(Pb.x, Pb.y));
              ImVec2 p3 = proj(ImVec2(Pc.x, Pc.y));
                  
              
              
                  ImColor outline = color;
                  //outline.Value.w = alpha * (1.0f - level_geometry_fade_factor) + level_geometry_fade_factor;
                  
                  ImColor base = outline;
                  base.Value.w *= 0.15f;
                  
              //if (p1.x >= 640.0f && p2.x >= 640.0f && p3.x >= 640.0f && p1.x <= 1000 && p2.x <= 1000 && p3.x <= 1000) {
                drawlist->AddTriangle(p1, p2, p3, outline);
                drawlist->AddTriangleFilled(p1, p2, p3, base);
              //}
              //}
               // }
                
                //              stVector3D normal = vector3_new(TnA.x, TnA.y, TnA.z);
                //              ImVec4 coord_center = project_world_coordinate(vector4_mul_matrix4(vector4_new(center.x, center.y, center.z, 1.0f), T));
                //              ImVec4 coord_norm = project_world_coordinate(vector3_add(center, normal));
                //              ImVec2 poscenter = ImVec2(off.x + sz.x - coord_center.x * sz.x, off.y + coord_center.y * sz.y);
                //              ImVec2 posnorm = ImVec2(off.x + sz.x - coord_norm.x * sz.x, off.y + coord_norm.y * sz.y);
                
                /* To the center vector, add the offset of the normal. */
                //if (normals && 1.0f / vector3_length(vector3_sub(Na, center)) < 1.0f) drawlist->AddLine(poscenter, posnorm, ImColor(normal.x, normal.y, normal.z, 1.0f));
              //}
            }
          }
        }
      } catch (...) {
        
      }
    }
    
  } catch (...) {
    printf("raad\n");
  }

    
    object->recurse([&](stSuperObject* obj, void *data) {
      try { drawGeometryTopdown(obj, T, drawlist, off, sz, color); } catch (...) { }
    }, nullptr);
  
}

static auto getIPOVertices(pointer<stSuperObject> object, const stMatrix4D transform, std::vector<stVector3D>& v) -> void {
  if (!object) return;
  stMatrix4D T = object->globalTransform->matrix * transform;
  try {
    if (object->type == stSuperObject::type::IPO) {
      try {
        pointer<stCollideObject> zdr = object->ipo->physicalObject->physicalCollideset->zdr;
        for (int i = 0; i < zdr->numElements; i++) {
          int16_t type = zdr->elementTypes[i];
          if (type == stCollideObject::type::IndexedTriangles) {
            pointer<stCollideElementIndexedTriangles> element = zdr->elements[i];
            stVector3D* vertices = zdr->vertices;
            uint16* indices = element->faceIndices;
            
            for (int i = 0; i < zdr->numVertices; i++)
              v.push_back(vertices[i]);
            
//            for (int16_t index = 0; index < element->numFaces; index++) {
//              uint16 idx0 = *(indices + index * 3 + 0);
//              uint16 idx1 = *(indices + index * 3 + 1);
//              uint16 idx2 = *(indices + index * 3 + 2);
//
//              stVector3D Pa = *(vertices + idx0);
//              stVector3D Pb = *(vertices + idx1);
//              stVector3D Pc = *(vertices + idx2);
//
//              vertices.
//            }
          }
        }
      } catch (...) {}
    }
  } catch (...) {}

    
    object->recurse([&](stSuperObject* obj, void *data) {
      try { getIPOVertices(obj, T, v); } catch (...) { }
    }, nullptr);
  
}
