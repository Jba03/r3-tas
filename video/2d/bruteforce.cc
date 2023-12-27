//
//  bruteforce.cpp
//  r3-tas
//
//  Created by Jba03 on 2023-08-07.
//

#include "gui.hh"
#include "game.hh"

static void dumpGeometry(FILE *fp) {
//  game::p_stFatherSector->recurse([&](stSuperObject *spo, void*) {
//    // Write type
//    fwrite(&spo->type, sizeof(uint32_t), 1, fp);
//    
//    if (spo->type == superobject_type_ipo) {
//      stInstantiatedPhysicalObject *ipo = (stInstantiatedPhysicalObject*)spo->data;
//      fwrite(ipo->name, sizeof(char), 50, fp);
//      
//      if (ipo->physicalObject) {
//        if (ipo->physicalObject->physicalCollideset) {
//          stCollideObject *zdr = ipo->physicalObject->physicalCollideset->zdr;
//          if (zdr) {
//            fwrite(&zdr->numVertices, sizeof(short), 1, fp);
//            fwrite(zdr->vertices, sizeof(stVector3D), (int16_t)zdr->numVertices, fp);
//            
//            fwrite(zdr->octree, <#size_t __size#>, <#size_t __nitems#>, <#FILE *__stream#>)
//            
//            int nElements = zdr->countOfElements(collideObjectIndexedTriangles);
//            fwrite(&nElements, sizeof(int), 1, fp);
//            
//            zdr->forEachElement([&](int16 type, void *element) {
//              if (type == collideObjectIndexedTriangles) {
//                stCollideElementIndexedTriangles *triangles = (stCollideElementIndexedTriangles*)element;
//                
//                triangles->
//              }
//            });
//          }
//        }
//      }
//    }
//  }, nullptr);
}


static void draw() {
//  if (!game::isValidGameState()) return;
//  ImGui::Begin("Bruteforce");
//  
//  stSuperObject *spo = game::p_stDynamicWorld->find("Rayman", game::g_stObjectTypes);
//  stEngineObject *obj = (stEngineObject*)spo->data;
//  stDynam *dynam = obj->dynam;
//  stDynamics *dynamics = dynam->dynamics;
//  
//  if (ImGui::Button("save")) {
//    saveRequested = true;
//    printf("save\n");
//  }
//  
//  if (ImGui::Button("load")) {
//    loadRequested = true;
//   // block.load(spo);
//  }
//  
//  ImGui::End();
}

Window bruteforceWindow = { &draw };
