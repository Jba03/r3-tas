//
//  gui.cc
//  r3-tas-memorymap
//
//  Created by Jba03 on 2023-07-06.
//

#include "game.hh"
#include "gui.hh"
#include "graphics.hh"
//#include "stCameraGLI.hh"
//#include "bruteforce.hh"
//#include "stCollideElementIndexedSpheres.hh"
#include "interface.hh"

#include "dynamics.cc"

//#include "bruteforce2.hh"

#include "imgui_internal.h"
#include "implot.h"
#include "implot_internal.h"

#include "tools.hh"

#include <iostream>
#include <deque>
#include <list>

/* Global ImGUI context */
extern ImGuiContext* GImGui;
extern ImPlotContext *GImPlot;

static bool DrawGame = true;
static float TimeScale = 1.0f;
int timescale_real = 0;

//extern bState *initialBlock;
extern uint64_t initialHash;

namespace gui {
  
  // Menu
  struct contextMenu *contextMenu;
  
  // Window
  std::vector<superObjectWindow> spoWindows;
  
  MemoryEditor memoryEditor;
}

using namespace game;

static void DrawGameWindow(ImTextureID T, bool *windowed)
{
    *windowed = p_stActualWorld != NULL;
    
    if (DrawGame && p_stActualWorld)
    {
//        //*windowed = true;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
//
        ImGui::SetNextWindowContentSize(ImVec2(640*2, 528));
        ImGui::Begin("Game", &DrawGame, ImGuiWindowFlags_MenuBar);
        ImVec2 WindowPos = ImGui::GetWindowPos();
        ImVec2 WindowSize = ImGui::GetWindowSize();
//
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
      
        if (ImGui::BeginMenuBar()) {
          ImGui::TextColored(ImColor(1.0f, 1.0f, 1.0f, 0.5f), "%c", g_stEngineStructure->engineFrozen ? 'M' : char(g_stEngineStructure->mode) + '0');
            
            
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            if (ImGui::BeginMenu(g_stEngineStructure->currentLevelName))
            {
                ImGui::PopStyleColor();

                ImGui::ResetMouseDragDelta();
                for (int i = 0; i < g_stEngineStructure->levelCount; i++)
                {
                    std::string LevelName = g_stEngineStructure->levelNames[i];
                    bool selected = g_stEngineStructure->currentLevelName == LevelName;
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, i % 2 ? 1.0f : 0.75f));
                    if (ImGui::MenuItem(LevelName.c_str(), NULL, &selected))
                        g_stEngineStructure->loadLevel(LevelName);

                    ImGui::PopStyleColor();
                }

                ImGui::EndMenu();
            }
            else
            {
                ImGui::PopStyleColor();
            }

            //ImGui::TextColored(ImColor(1.0f, 0.6f, 0.0f), "Transit: %d", transition_frame);

            ImGui::EndMenuBar();
        }
        
        ImVec2 size = ImGui::GetWindowSize();
        size.x /= 2;

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));

        ImGui::Image(T, size);
        ImGui::SameLine();
        ImGui::Image(graphics::texture(), size);

        ImGui::PopStyleVar();

        ImDrawList *d = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetWindowPos();
        //d->AddImage(graphics::texture(), pos, ImVec2(pos.x+ImGui::GetWindowSize().x, pos.y+ImGui::GetWindowSize().y));

        //ImGui::Image(graphics::texture(), ImVec2(640, 528));
//
        ImGui::End();
//
//        static bool noclip = false;
        
        ImGui::Begin("Game window extras", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
        ImGui::SetWindowPos(ImVec2(WindowPos.x, WindowPos.y + WindowSize.y + 10));
        ImGui::SetWindowSize(ImVec2(WindowSize.x, 0));
        ImGui::Checkbox("Noclip/ghost mode", reinterpret_cast<bool*>(game::g_bGhostMode));
        ImGui::SliderFloat("Engine timescale", &TimeScale, 0.1f, 2.0f);
        ImGui::SliderInt("Engine timescale2", &timescale_real, 0, 256*4);
        //g_stEngineStructure->timer.ticksPerMs = 40500 * 1.0f / TimeScale;
        ImGui::End();
        
        //pointer<uint32>(0x805D8510).assign(timescale_real);
        //*(uint32*)(memory::baseAddress + 0x005D8510) = timescale_real;
      
//        stSuperObject *rayman = game::p_stDynamicWorld->find("Rayman", game::g_stObjectTypes);
//        stVector3D position = rayman->globalTransform->position();
//        printf("%.2f %.2f %.2f\n", position.x, position.y, position.z);
////
//      stEngineObject *raymanObj = rayman->data;
//      stDynamics *dynamics = ((stDynam*)raymanObj->dynam)->dynamics;
      //printf("intertia: %f %f %f\n", dynamics->advanced.xInertia.f(), dynamics->advanced.yInertia.f(), dynamics->advanced.zInertia.f());
      //printf("flags: %X, %X\n", (uint32_t)dynamics->base.flags, (uint32_t)dynamics->base.endFlags);
      
//      
//      stVector3D propose = ((stDynamicsReport*)dynamics->base.report)->currentAbsoluteSpeed.linear.host();
//      printf("propose: %f %f %f\n", propose.x, propose.y, propose.z);
      //printf("behavior: %s\n", raymanObj->brain->mind->intelligence->currentBehavior->name);
        
//        if (IPT_g_stInputStructure && p_stDynamicWorld && !g_stEngineStructure->engineFrozen)
//        {
//            if (IPT_g_stInputStructure->entries[IPT_E_Entry_Button_Jump].state + 1 > 0)
//            {
//                //stSuperObject *rayman = p_stDynamicWorld->find("Rayman", game::g_stObjectTypes);
////                //rayman->IPO;
////
////                //stDynamics *dynamics = ((stDynam*)object->dynam)->dynamics;
////
////                //std::cout << "name: " << object->getName(actor_family_name, objectType) << "\n";
////                //printf("jump: %X\n", IPT_g_stInputStructure->entries[IPT_E_Entry_Button_Jump].state.offset());
////                //rayman->localTransform->matrix.m30 = game_byteorder_f32(1.1);//vector3_new(1.0f, 1.0f, 1.0f);
////               // printf("jump! %X\n", rayman->data.offset());
//            }
//        }
        
        //if (*(bool*)(memoryBase + GCN_POINTER_NOCLIP_MODE))
        {
            //uint32_t v = IPT_g_stInputStructure->numEntries;
            //printf("%X\n", swap32(v));
            //printf("aa\n");
            
//
//            for (unsigned int i = 0; i < IPT_g_stInputStructure->numEntries; i++)
//            {
//                stInputEntryElement element = IPT_g_stInputStructure->entries[i];//.offsetByType<stInputEntryElement>(3);
//                std::cout << "#define IPT_E_Entry_" << std::hex << std::string(element.actionName) << " " + std::to_string(i) << "\n";
//            }
//
//            printf("\n\n\n");
                
            
            //std::cout << std::hex << std::to_string(element.state) << "\n";
//            if (element.state > 0)
//            {
//                printf("aaa\n");
//            }
            
            
//            if (!(input.button.a->state & 0xFF000000))
//            {
//                printf("fly\n");
//                //stDynamics *dyn = fnActorGetDynamics(actor_rayman);
//                //dyn->base.currentTransform.scale = vector4_new(0.5, 0.5f, 0.5f, 1.0f);
//            }
        }
    }
    else
    {
        *windowed = false;
    }
}

static auto drawWorld(stSuperObject *root, stMatrix4D T) -> void {
  if (!root) return;
  
  T = root->globalTransform->matrix * T; //root->globalTransform->matrix.hostByteOrder() * T;
  if (root->type == superObjectTypeIPO)
    graphics::drawIPO(root->data, T);
  
  root->forEachChild([&](stSuperObject *object, void*) {
    drawWorld(object, T);
  });
}

#include "hook.hh"

namespace gui {
    
  ImGuiID dockspaceID;
  
  static auto loadStyle() -> void {
    
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.AntiAliasedFill = true;
    style.FrameRounding = 0.0f;
    style.TabRounding = 5.0f;
    
    style.WindowPadding = ImVec2(5, 5);
    
    style.FrameBorderSize = 0.0f;
    style.WindowBorderSize = 0.0f;

    style.Colors[ImGuiCol_WindowBg] = ImColor(10, 10, 10, 255);
    style.Colors[ImGuiCol_TitleBg] = ImColor(20, 20, 20, 255);
    style.Colors[ImGuiCol_TitleBgActive] = ImColor(30, 30, 30, 255);
    style.Colors[ImGuiCol_Border] = ImColor(20, 20, 20, 255);
    style.Colors[ImGuiCol_Tab] = ImColor(15, 15, 15, 255);
//    style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(17, 49, 54, 255);
//    style.Colors[ImGuiCol_TabActive] = ImColor(17, 49, 54, 255);
//    style.Colors[ImGuiCol_DockingEmptyBg] = ImColor(0, 10, 10, 255);
//    style.Colors[ImGuiCol_DockingPreview] = ImColor(68, 168, 183, 128);
//    style.Colors[ImGuiCol_Button] = ImColor(32, 115, 131, 255);
//    style.Colors[ImGuiCol_ModalWindowDimBg] = ImColor(0.0f, 0.0f, 0.0f, 0.9f);
//    style.Colors[ImGuiCol_FrameBg] =  ImColor(32, 115, 131, 128);
//
//    style.Colors[ImGuiCol_PopupBg] = ImColor(10, 27, 29, 255);

//    style.Colors[ImGuiCol_TableHeaderBg] =  ImColor(32, 115, 131, 64);
//    style.Colors[ImGuiCol_TableRowBg] =  ImColor(32, 115, 131, 96);
//    style.Colors[ImGuiCol_TableRowBgAlt] =  ImColor(16, 115/2, 131/2, 128);
//    style.Colors[ImGuiCol_TableBorderStrong] =  ImColor(32*2, 115*2, 131*2, 64);
//    style.Colors[ImGuiCol_TableBorderLight] =  ImColor(32*2, 115*2, 131*2, 0);

    //style.Colors[ImGuiCol_CheckMark] = ImColor(52, 186, 120, 255);
    
    ImPlot::GetStyle().Colors[ImPlotCol_FrameBg] = ImVec4(0,0,0,0);
    
  }
  
  void initialize() {
    graphics::initialize();
    GImPlot = ImPlot::CreateContext();
    ImPlot::PushColormap(ImPlotColormap_Hot);
  }
  
  auto drawPanels() -> void {
    
  }
  
  auto dockLayout() -> void {
   
    
    ImGui::DockBuilderRemoveNode(dockspaceID); // Clear out existing layout
    ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace); // Add empty node
    ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->WorkSize);
    
    ImGuiID dock_main_id = dockspaceID;
    
    ImGuiID left1 = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
    ImGuiID left2 = ImGui::DockBuilderSplitNode(left1, ImGuiDir_Down, 0.5f, nullptr, &left1);
    ImGuiID left3 = ImGui::DockBuilderSplitNode(left2, ImGuiDir_Down, 0.5f, nullptr, &left2);

    ImGuiID middle1 = dock_main_id;// ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 1.0f, nullptr, &dock_main_id);
    ImGuiID middle2 = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.33f, nullptr, &middle1);
    ImGuiID middle3 = ImGui::DockBuilderSplitNode(middle2, ImGuiDir_Right, 0.25f, nullptr, &middle2);

    ImGuiID right1 = ImGui::DockBuilderSplitNode(middle1, ImGuiDir_Right, 0.275f, nullptr, &middle1);;
    ImGuiID right2 = ImGui::DockBuilderSplitNode(right1, ImGuiDir_Down, 0.33f, nullptr, &right1);
    
    ImGui::DockBuilderDockWindow("Common", left1);
    ImGui::DockBuilderDockWindow("Hierarchy", left2);
    ImGui::DockBuilderDockWindow("Movie", left3);
    ImGui::DockBuilderDockWindow("Game", middle1);
    ImGui::DockBuilderDockWindow("Object window", middle2);
    ImGui::DockBuilderDockWindow("GameSub2", middle3);
    ImGui::DockBuilderDockWindow("RNG", right1);
    ImGui::DockBuilderDockWindow("Test", right2);
    
    ImGui::DockBuilderFinish(dockspaceID);
    
  }
  
  static bool firstTime = true;
    
  
  static auto drawGraphics() -> void {
    if (game::isValidGameState()) {
      stCamera *camera = nullptr;
      if ((camera = game::g_stEngineStructure->viewportCamera[0])) {
        /* Construct projection matrix. Use negative fov in order to account for flipped transformations. */
        graphics::projectionMatrix = stMatrix4D::perspective(-0.24f + 0.1, 640.0f / 528.0f, 0.1, 100.f);
        
        //            float *proj = graphics::getProjectionMatrix();
        //            for (int i = 0; i < 16; i++) {
        //              graphics::projectionMatrix.m[i] = *(proj + i); //float(graphics::getProjectionMatrix()->m[i]);
        //            }
        //
        //                            graphics::projectionMatrix.m00 = -graphics::projectionMatrix.m00;
        //                            graphics::projectionMatrix.m11 = -graphics::projectionMatrix.m11;
        //                            graphics::projectionMatrix.m22 -= 1;
        //                            graphics::projectionMatrix.m32 *= 0.5f;
        //                            graphics::projectionMatrix.m23 *= 4.0f;
    
    
        
        
//        graphics::viewMatrix = camera->transform;
//        graphics::viewMatrix.m01 = -graphics::viewMatrix.m01; graphics::viewMatrix.m11 = -graphics::viewMatrix.m11;
//        graphics::viewMatrix.m21 = -graphics::viewMatrix.m21; graphics::viewMatrix.m22 = -graphics::viewMatrix.m22;
//        graphics::viewMatrix.m31 = -graphics::viewMatrix.m31; graphics::viewMatrix.m32 = -graphics::viewMatrix.m32;
//        graphics::viewMatrix.m02 = -graphics::viewMatrix.m02; graphics::viewMatrix.m12 = -graphics::viewMatrix.m12;
      }
    
//            graphics::beginFrame(640, 528);
//
//            drawWorld(p_stFatherSector, stMatrix4D());
//
//            stSuperObject *rayman = pointer<stSuperObject>(0x80BF0C0C);
//
//            p_stDynamicWorld->forEachChild([&](stSuperObject *spo, void*) {
//              try {
//                uint32_t c = game::objectColor(spo);
//                stVector4D color;
//                color.z = float((c & 0x00FF0000) >> 16) / 255.0f;
//                color.y = float((c & 0x0000FF00) >> 8) / 255.0f;
//                color.x = float((c & 0x000000FF) >> 0) / 255.0f;
//                color.w = 1.0f;//(c & 0x000000FF) >> 0;
//
//                graphics::drawPoint(spo->globalTransform->translation(), color);
//
//                if (spo->actor->collSet) {
//                  std::vector<std::pair<pointer<stZdxList>, stVector4D>> p = {
//                    std::pair<pointer<stZdxList>, stVector4D> { spo->actor->collSet->zddList, stVector4D(0.5f, 0.5f, 0.5f, 0.7f) },
//                    std::pair<pointer<stZdxList>, stVector4D> { spo->actor->collSet->zdeList, stVector4D(0.0f, 0.1f, 1.0f, 0.5f) },
//                    std::pair<pointer<stZdxList>, stVector4D> { spo->actor->collSet->zdmList, stVector4D(1.0f, 0.7f, 0.0f, 0.5f) },
//                    std::pair<pointer<stZdxList>, stVector4D> { spo->actor->collSet->zdrList, stVector4D(1.0f, 0.0f, 0.0f, 0.5f) },
//                  };
//
//                  for (std::pair<pointer<stZdxList>, stVector4D> pair : p) {
//                    if (pair.first) {
//                      pair.first->forEachElement([&](stCollideObject *collideObject) {
//                        if (collideObject) collideObject->forEachElement([&](int16 type, void *element) {
//                          if (type == collideElementIndexedSpheres){
//                            stCollideElementSpheres *spheres = static_cast<stCollideElementSpheres*>(element);
//                            for (int i = 0; i < spheres->numSpheres; i++) {
//                              stCollideElementIndexedSphere sphere = spheres->spheres[i];
//                              stVector3D center = *(stTransform*)spo->globalTransform * collideObject->vertices[int(sphere.indexOfCenterPoint)];
//                              graphics::drawSphere(center, sphere.radius, pair.second);
//                            }
//                          }
//                        });
//                      });
//                    }
//                  }
//                }
//              } catch (bad_ptr& e) {
//                // std::cout << "Failed to draw graphs: " + e.what() + "\n";
//              }
//            });
    
    
            // Draw graphs
            //stGraphChainList *graphList = g_stEngineStructure->graphList;
    //        while (graphList) {
    //          //printf("%X, %lX\n", g_stEngineStructure->graphList.memoryOffset().physicalAddress(), long(graphList) - long(memory::baseAddress));
    //          //printf("graph: %X, %X\n", (graphList->graph.memoryOffset().physicalAddress()), graphList->next.pointeeAddress().physicalAddress());
    //          //graphics::drawGraph(graphList->graph);
    //          graphList = graphList->next;
    //        }
    
    //        for (std::pair<uint32_t, bState> kv : blocks) {
    //          //graphics::drawPoint(kv.second.physicsState.transform.position(), stVector4D(1.0f, 0.0f, 1.0f, 1.0f));
    //        }
    
    
    
    //        graphics::drawLine(rayman->globalTransform->position(), rayman->globalTransform->position() +stVector3D(0,0,5), stVector4D(0.0f, 1.0f, 0.0f, 1.0f));
    
    
            int count = 0;
    
    
    //        bState *state = &blocks[initialHash];
    //        if (state) {
    //          while (state != NULL) {
    //            if (state->next) {
    //
    //              stVector3D a = state->physicsState.transform.position();
    //              stVector3D b = state->next->physicsState.transform.position();
    //              graphics::drawLine(a, b, stVector4D(0.0f, 1.0f, 0.0f, 1.0f));
    //            }
    //
    //            state = state->next;
    //            count++;
    //          }
    //        }
    
            //printf("num nodes: %d\n", count);
    
            graphics::endFrame();
          }
  }
  
  struct ScrollingBuffer {
      int MaxSize;
      int Offset;
      ImVector<ImVec2> Data;
      ScrollingBuffer(int max_size = 3000) {
          MaxSize = max_size;
          Offset  = 0;
          Data.reserve(MaxSize);
      }
      void AddPoint(float x, float y) {
          if (Data.size() < MaxSize)
              Data.push_back(ImVec2(x,y));
          else {
              Data[Offset] = ImVec2(x,y);
              Offset =  (Offset + 1) % MaxSize;
          }
      }
      void Erase() {
          if (Data.size() > 0) {
              Data.shrink(0);
              Offset  = 0;
          }
      }
  };
  
  static bool transitionBegan = false;
  static int framesUntilTransitionEnd = 0;
  
  static void drawGameWindow(ImTextureID texture) {
    ImGui::Begin("Game", nullptr, ImGuiWindowFlags_MenuBar);
    ImGui::SetWindowSize(ImVec2(640,528));
    
    if (ImGui::BeginMenuBar()) {
      ImGui::TextColored(ImPlot::GetColormapColor(int(g_stEngineStructure->mode)), "%d", int(g_stEngineStructure->mode));
      if (framesUntilTransitionEnd != 0) ImGui::TextColored(ImVec4(0,1,0,1), "DISK READ: -%d", framesUntilTransitionEnd--);
      if (ImGui::BeginMenu("Options")) {
        if (ImGui::BeginMenu("Change level")) {
          for (int n = 0; n < g_stEngineStructure->levelCount; n++) {
            std::string name = game::g_stEngineStructure->levelNames[n];
            bool b = name == game::g_stEngineStructure->currentLevelName;
            if (b) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.65f, 0.0f, 1.0f));
            if (ImGui::MenuItem(name.c_str())) {
              g_stEngineStructure->loadLevel(name);
            }
            if (b) ImGui::PopStyleColor();
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenu();
      }
      
      //if (ImGui::BeginMenu)
      ImGui::EndMenuBar();
    }
    
    ImVec2 p = ImGui::GetCursorPos();
    ImVec2 av = ImGui::GetContentRegionAvail();
    ImVec2 m = ImVec2(av.x / 640.0f, av.y / 528.0f);
    ImVec2 sz = av;
    if (m.y < m.x) {
      sz.x = av.y / 528.0f * 640.0f;
      ImGui::SetCursorPosX(p.x + av.x / 2 - sz.x / 2);
    } else if (m.x < m.y) {
      sz.y = av.x / 640.0f * 528.0f;
      ImGui::SetCursorPosY(p.y + av.y / 2 - sz.y / 2);
    }
    
    ImGui::Image(texture, sz);
    ImGui::End();
  }
  
  auto draw(void *c, void *texture, bool *windowed) -> void {
    *windowed = true;
    
    GImGui = (ImGuiContext*)c;
    loadStyle();
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
      
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
      
    dockspaceID = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
      
    /* Draw panels */
    if (firstTime) {
      dockLayout();
      firstTime = false;
    }
    
    //ImPlot::ShowDemoWindow();
    
    
    static ScrollingBuffer speedbuffer;
    static ScrollingBuffer speedbuffer_v;
    static float t = 0.0f;
    
    try {
      t += ImGui::GetIO().DeltaTime;
      float h = 0;//g_stEngineStructure->currentMainPlayers[0]->actor->getHorizontalSpeed();
      float v = 0;//g_stEngineStructure->currentMainPlayers[0]->actor->getVerticalSpeed();
      speedbuffer.AddPoint(t, h);
      speedbuffer_v.AddPoint(t, v);
    } catch (BadPointer& e) {
      std::cout << "Failed to get speed data: " << e.what() << "\n";
    }
    
    static float history = 10;
      
    ImGui::Begin("Common");
    if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1,150))) {
      ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_AutoFit);
      ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
      //ImPlot::SetupAxisLimits(ImAxis_Y1,0,-1);
      ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 1.0f);
      //ImPlot::PlotShaded("Speed", &speedbuffer.Data[0].x, &speedbuffer.Data[0].y, speedbuffer.Data.size(), -INFINITY, 0, speedbuffer.Offset, 2 * sizeof(float));
      if (speedbuffer.Data.size() > 1) {
        ImPlot::PlotLine("HSpeed", &speedbuffer.Data[0].x, &speedbuffer.Data[0].y, speedbuffer.Data.size(), 0, speedbuffer.Offset, 2*sizeof(float));
        ImPlot::PlotLine("VSpeed", &speedbuffer_v.Data[0].x, &speedbuffer_v.Data[0].y, speedbuffer_v.Data.size(), 0, speedbuffer_v.Offset, 2*sizeof(float));
      }
      ImPlot::EndPlot();
    }
    ImGui::End();
    
      
    ImGui::Begin("Hierarchy");
    HierarchyWindow.Draw();
    ImGui::End();
      
    ImGui::Begin("Movie");
    ImGui::End();
    
    
    drawGraphics();
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(0,0), ImVec2(640,528));
    
    drawGameWindow(static_cast<ImTextureID>(texture));
    
    //free(texture2);
      
    ImGui::Begin("Object window");
    if (game::isValidGameState()) {
      pointer<stSuperObject> spo = Address(0x80BF0C0C);
      pointer<stEngineObject> eng = spo->actor;
      
      drawDynamics(eng->dynam->dynamics);
    }
    ImGui::End();
      
    ImGui::Begin("GameSub2");
    ImGui::End();
    
    ImGui::Begin("RNG");
    ImGui::End();
    
    ImGui::Begin("Test");
    ImGui::End();
      
    AIWindow(pointer<stSuperObject>(0x80BF0C0C)).draw();
    
    ImGui::End();
    
    ImGui::PopStyleVar(3);
      
      //gui::memoryEditor.HighlightFn = gui::memoryEditorHighlight;
      
    
      //DrawGameWindow(texture, windowed);
//
//      //if (false)
//      if (game::isValidGameState()) {
//        stCamera *camera = nullptr;
//        if ((camera = game::g_stEngineStructure->viewportCamera[0])) {
//          /* Construct projection matrix. Use negative fov in order to account for flipped transformations. */
//          graphics::projectionMatrix = stMatrix4D::perspective(-camera->xAlpha + 0.1, 640.0f / 528.0f, camera->near, camera->far);
//
//          //            float *proj = graphics::getProjectionMatrix();
//          //            for (int i = 0; i < 16; i++) {
//          //              graphics::projectionMatrix.m[i] = *(proj + i); //float(graphics::getProjectionMatrix()->m[i]);
//          //            }
//          //
//          //                            graphics::projectionMatrix.m00 = -graphics::projectionMatrix.m00;
//          //                            graphics::projectionMatrix.m11 = -graphics::projectionMatrix.m11;
//          //                            graphics::projectionMatrix.m22 -= 1;
//          //                            graphics::projectionMatrix.m32 *= 0.5f;
//          //                            graphics::projectionMatrix.m23 *= 4.0f;
//
//
//          graphics::viewMatrix = camera->transform.matrix;
//          graphics::viewMatrix.m01 = -graphics::viewMatrix.m01; graphics::viewMatrix.m11 = -graphics::viewMatrix.m11;
//          graphics::viewMatrix.m21 = -graphics::viewMatrix.m21; graphics::viewMatrix.m22 = -graphics::viewMatrix.m22;
//          graphics::viewMatrix.m31 = -graphics::viewMatrix.m31; graphics::viewMatrix.m32 = -graphics::viewMatrix.m32;
//          graphics::viewMatrix.m02 = -graphics::viewMatrix.m02; graphics::viewMatrix.m12 = -graphics::viewMatrix.m12;
//        }
//
//        //graphics::beginFrame(640, 528);
//
////        drawWorld(p_stFatherSector, stMatrix4D());
////
////        stSuperObject *rayman = pointer<stSuperObject>(0x80BF0C0C);
////
////        p_stDynamicWorld->forEachChild([&](stSuperObject *spo, void*) {
////          try {
////            uint32_t c = game::objectColor(spo);
////            stVector4D color;
////            color.z = float((c & 0x00FF0000) >> 16) / 255.0f;
////            color.y = float((c & 0x0000FF00) >> 8) / 255.0f;
////            color.x = float((c & 0x000000FF) >> 0) / 255.0f;
////            color.w = 1.0f;//(c & 0x000000FF) >> 0;
////
////            graphics::drawPoint(spo->globalTransform->translation(), color);
////
////            if (spo->actor->collSet) {
////              std::vector<std::pair<pointer<stZdxList>, stVector4D>> p = {
////                std::pair<pointer<stZdxList>, stVector4D> { spo->actor->collSet->zddList, stVector4D(0.5f, 0.5f, 0.5f, 0.7f) },
////                std::pair<pointer<stZdxList>, stVector4D> { spo->actor->collSet->zdeList, stVector4D(0.0f, 0.1f, 1.0f, 0.5f) },
////                std::pair<pointer<stZdxList>, stVector4D> { spo->actor->collSet->zdmList, stVector4D(1.0f, 0.7f, 0.0f, 0.5f) },
////                std::pair<pointer<stZdxList>, stVector4D> { spo->actor->collSet->zdrList, stVector4D(1.0f, 0.0f, 0.0f, 0.5f) },
////              };
////
////              for (std::pair<pointer<stZdxList>, stVector4D> pair : p) {
////                if (pair.first) {
////                  pair.first->forEachElement([&](stCollideObject *collideObject) {
////                    if (collideObject) collideObject->forEachElement([&](int16 type, void *element) {
////                      if (type == collideElementIndexedSpheres){
////                        stCollideElementSpheres *spheres = static_cast<stCollideElementSpheres*>(element);
////                        for (int i = 0; i < spheres->numSpheres; i++) {
////                          stCollideElementIndexedSphere sphere = spheres->spheres[i];
////                          stVector3D center = *(stTransform*)spo->globalTransform * collideObject->vertices[int(sphere.indexOfCenterPoint)];
////                          graphics::drawSphere(center, sphere.radius, pair.second);
////                        }
////                      }
////                    });
////                  });
////                }
////              }
////            }
////          } catch (bad_ptr& e) {
////            // std::cout << "Failed to draw graphs: " + e.what() + "\n";
////          }
////        });
//
//
//        // Draw graphs
//        //stGraphChainList *graphList = g_stEngineStructure->graphList;
////        while (graphList) {
////          //printf("%X, %lX\n", g_stEngineStructure->graphList.memoryOffset().physicalAddress(), long(graphList) - long(memory::baseAddress));
////          //printf("graph: %X, %X\n", (graphList->graph.memoryOffset().physicalAddress()), graphList->next.pointeeAddress().physicalAddress());
////          //graphics::drawGraph(graphList->graph);
////          graphList = graphList->next;
////        }
//
////        for (std::pair<uint32_t, bState> kv : blocks) {
////          //graphics::drawPoint(kv.second.physicsState.transform.position(), stVector4D(1.0f, 0.0f, 1.0f, 1.0f));
////        }
//
//
//
////        graphics::drawLine(rayman->globalTransform->position(), rayman->globalTransform->position() +stVector3D(0,0,5), stVector4D(0.0f, 1.0f, 0.0f, 1.0f));
//
//
//        int count = 0;
//
//
////        bState *state = &blocks[initialHash];
////        if (state) {
////          while (state != NULL) {
////            if (state->next) {
////
////              stVector3D a = state->physicsState.transform.position();
////              stVector3D b = state->next->physicsState.transform.position();
////              graphics::drawLine(a, b, stVector4D(0.0f, 1.0f, 0.0f, 1.0f));
////            }
////
////            state = state->next;
////            count++;
////          }
////        }
//
//        //printf("num nodes: %d\n", count);
//
//        //graphics::endFrame();
//      }
//
//
//
//
////            stSuperObject *rayman = p_stDynamicWorld->find("Rayman", g_stObjectTypes);
////            stEngineObject *object = rayman->data;
////
////          if (object->collSet) {
////            if (object->collSet->zdmList) {
////              object->collSet->zdmList->forEachElement([&](stCollideObject *zdm) {
////                zdm->forEachElement([&](int16 type, void *element) {
////                  if (type == collideObjectIndexedSpheres) {
////                    stCollideElementSpheres* spheres = (stCollideElementSpheres*)element;
////                    for (int i = 0; i < spheres->numSpheres; i++) {
////                      stCollideElementIndexedSphere sphere = spheres->spheres[i];
////
////                      stVector3D center = zdm->vertices[(int16_t)sphere.indexOfCenterPoint].host();
////                      //printf("%f %.2f %.2f %.2f\n", sphere.radius.f(), center.x, center.y, center.z);
////
////                      //if (sphere.radius.f() != 0.8f) zdm->vertices[(int16_t)sphere.indexOfCenterPoint] = stVector3D(0.0f, 0.0f, 100.0f).game();
////
////                      center = center + rayman->globalTransform->position();
////
////
////
////                      //printf("%d %.2f %.2f %.2f\n", (int16_t)sphere.indexOfCenterPoint, center.x, center.y, center.z);
////
////                      graphics::drawPoint(center, stVector4D(1.0f, 0.0f, 0.0f, 1.0f));
////                    }
////                  }
////                });
////              });
////            }
////          }
////
//////          rayman->recurse([&](stSuperObject *child, void*) {
//////            if (child->type == superobject_type_physical_object) {
//////              stPhysicalObject *physObj = child->data;
//////              if (physObj) {
//////                if (physObj->physicalCollideset) {
//////                  stCollideObject *zdm = physObj->physicalCollideset->zdr;
//////                  printf("addr: %X\n", physObj->physicalCollideset.offset());
//////                  if (zdm) {
//////                    printf("zdm yes\n");
//////                    zdm->forEachElement([&](int16 type, void *element) {
//////                      printf("element: %d\n", type);
//////                      if (type == collideObjectIndexedSpheres) {
//////                        printf("indexed sphere\n");
//////                      }
//////                    });
//////                  }
//////                }
//////              }
//////            }
//////          }, nullptr);
////
////
//////            p_stDynamicWorld->forEachChild([&](stSuperObject *object, void*)
//////            {
//////                uint32_t c = game::objectColor(object);
//////                stVector4D color;
//////                color.z = (c & 0xFF000000) >> 24;
//////                color.y = (c & 0x00FF0000) >> 16;
//////                color.x = (c & 0x0000FF00) >> 8;
//////                color.w = (c & 0x000000FF) >> 0;
//////
//////                graphics::drawPoint(*(stVector3D*)&object->globalTransform->matrix.m30, color);
//////            });
////
//////          for (std::pair<uint32_t, attempt*> kv : blockMap) {
//////            graphics::drawPoint(kv.second->states.back().currentTranslation, stVector4D(1.0f, 0.0f, 1.0f, 1.0f));
//////          }
////
//////          if (optimalAttempt) {
//////            for (int s = 0; s < optimalAttempt->states.size(); s += 1) {
//////              state& s1 = optimalAttempt->states[s + 0];
//////              state& s2 = optimalAttempt->states[s + 1];
//////              graphics::drawLine(s1.currentTranslation, s2.currentTranslation, stVector4D(1.0f, 0.0f, 1.0f, 1.0f));
//////            }
//////
////////            for (state& s : optimalAttempt->states) {
////////              graphics::drawPoint(s.currentTranslation, stVector4D(1.0f, 0.0f, 1.0f, 1.0f));
////////            }
//////          }
////////
//////          int counter = 0;
//////          for (std::pair<uint32_t, bState> kv : blocks) {
//////            counter++;
//////            bState& block = kv.second;
//////            if (block.next && (counter % 5) == 0)
//////              graphics::drawLine(block.absolutePosition, block.next->absolutePosition, stVector4D(1.0f, 0.0f, 1.0f, 1.0f));
//////          }
////
////
////            drawWorld(p_stFatherSector, p_stFatherSector->globalTransform->matrix.hostByteOrder());
////
////            graphics::endFrame();
////
//////            ImGui::Begin("aaa");
//////            if (graphics::texture())ImGui::Image(graphics::texture(), ImVec2(640, 528));
//////            ImGui::End();
////        }
//
////        if (!ConfigurationWindow.Open) {
////            MenuBar.Draw();
////            RNGWindow.Draw();
////            HierarchyWindow.Draw();
////            RunCreateWindow.Draw();
////
////            CinematicWindow.Draw();
////
////          bruteforceWindow.Draw();
////        }
////
////        ConfigurationWindow.Draw();
//        //    MovieInputWindow.Draw();
//        //    ScriptWindow.Draw();
//        //    SuperObjectWindow.Draw();
//
//      HierarchyWindow.Draw();
//
//      for (superObjectWindow& window : spoWindows)
//        window.draw();
//
//
//      //gui::popup(nullptr, nullptr);
//
//      //gui::memoryEditor.GotoAddr = 0xBF0C0C;
//      gui::memoryEditor.ReadOnly = false;
//      gui::memoryEditor.DrawWindow("Memory editor", (void*)memory::baseAddress, 24 * 1000 * 1000);
    }
    
}
