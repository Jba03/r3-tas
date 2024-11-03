//
//  gui.cc
//  r3-tas-memorymap
//
//  Created by Jba03 on 2023-07-06.
//

#include "game.hh"
#include "gui.hh"
#include "graphics.hh"
#include "interface.hh"

#include "dynamics.cc"

#include "imgui_internal.h"
#include "implot_internal.h"

#include "tools.hh"
#include "settings.hh"

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
  std::vector<stVector3D> xrayPoints;
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
        //ImGui::Image(graphics::texture(), size);

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
        ImGui::Checkbox("Noclip/ghost mode", reinterpret_cast<bool*>((bool*)g_bGhostMode));
        ImGui::SliderFloat("Engine timescale", &TimeScale, 0.1f, 2.0f);
        ImGui::SliderInt("Engine timescale2", &timescale_real, 0, 256*4);
        //g_stEngineStructure->timer.ticksPerMs = 40500 * 1.0f / TimeScale;
        ImGui::End();
        
        //pointer<uint32>(0x805D8510).assign(timescale_real);
        //*(uint32*)(memory::baseAddress + 0x005D8510) = timescale_real;
      
//        stSuperObject *rayman = p_stDynamicWorld->find("Rayman", game::g_stObjectTypes);
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

static auto drawWorld(pointer<stSuperObject> root, stMatrix4D T) -> void {
  if (!root) return;
  
  try {
    T = root->globalTransform->matrix * T;
    if (root->type == superobjectTypeIPO) {
      //root->drawFlags = 0; // for collision/game blend
      mainContext->drawIPO(root->data, T);
    }

    root->forEachChild([&](pointer<stSuperObject> object, void*) {
      drawWorld(object, T);
    });
  } catch (...) {

  }
}

#include "hook.hh"

namespace gui {
    
  static ImGuiID dockspaceID;
  
  static void loadStyle() {
    
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.AntiAliasedFill = true;
    style.AntiAliasedLines = true;
    
    style.FrameRounding = 5.0f;
    style.TabRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.ChildRounding = 5.0f;
    
    style.WindowPadding = ImVec2(5, 5);
    
  
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.WindowBorderSize = 1.0f;
    //style.Colors[ImGuiCol_Border] = ImColor(35, 35, 35, 255);
    
    style.Colors[ImGuiCol_MenuBarBg] = ImColor(35, 35, 35, 255);
    style.Colors[ImGuiCol_DockingEmptyBg] = ImColor(5, 5, 5, 255);
    style.Colors[ImGuiCol_WindowBg] = ImColor(10, 10, 10, 255);
    style.Colors[ImGuiCol_TitleBg] = ImColor(20, 20, 20, 255);
    style.Colors[ImGuiCol_TitleBgActive] = ImColor(30, 30, 30, 255);
    style.Colors[ImGuiCol_Border] = ImColor(35, 35, 35, 255);
    //style.Colors[ImGuiCol_BorderShadow] = ImColor(255, 25, 25, 255);
    style.Colors[ImGuiCol_Tab] = ImColor(15, 15, 15, 255);
    style.Colors[ImGuiCol_PopupBg] = ImColor(20, 20, 20, 255);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(45, 45, 45, 255);
    style.Colors[ImGuiCol_TabActive] = ImColor(65, 65, 65, 255);
    
    style.Colors[ImGuiCol_Header] = ImColor(65, 65, 65, 255);
    style.Colors[ImGuiCol_HeaderHovered] = ImColor(65, 65, 65, 255);
    style.Colors[ImGuiCol_HeaderActive] = ImColor(65, 65, 65, 255);
    
    style.Colors[ImGuiCol_Button] = ImColor(65, 65, 65, 255);
    style.Colors[ImGuiCol_ChildBg] = ImColor(30, 30, 30, 50);
   // style.Colors[ImGuiCol_HeaderActive] = ImColor(105, 65, 65, 255);
    
    if (config["mode"] == "Speedrun") {
      style.Colors[ImGuiCol_MenuBarBg] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);
      style.Colors[ImGuiCol_WindowBg] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);
      style.Colors[ImGuiCol_Border] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);
      style.PopupBorderSize = 0.0f;
      style.FrameBorderSize = 0.0f;
      style.WindowBorderSize = 0.0f;
    }
    
    
//
//    style.Colors[ImGuiCol_PopupBg] = ImColor(10, 27, 29, 255);

//    style.Colors[ImGuiCol_TableHeaderBg] =  ImColor(32, 115, 131, 64);
//    style.Colors[ImGuiCol_TableRowBg] =  ImColor(32, 115, 131, 96);
//    style.Colors[ImGuiCol_TableRowBgAlt] =  ImColor(16, 115/2, 131/2, 128);
//    style.Colors[ImGuiCol_TableBorderStrong] =  ImColor(32*2, 115*2, 131*2, 64);
//    style.Colors[ImGuiCol_TableBorderLight] =  ImColor(32*2, 115*2, 131*2, 0);

    style.Colors[ImGuiCol_CheckMark] = ImColor(52, 186, 120, 255);
     
    style.Colors[ImPlotCol_FrameBg] = ImColor(25, 25, 25, 255);
    
  }
  
  void initialize() {
    //graphics::initialize();
    GImPlot = ImPlot::CreateContext();
    ImPlot::PushColormap(ImPlotColormap_Spectral);
    
    gui::memoryEditor.Cols = 10;
    gui::memoryEditor.OptShowDataPreview = true;
    gui::memoryEditor.GotoAddr = 0xBF0C0C;
  }
  
#pragma mark - Layout
  
  static bool needsLayout = true;
  
  static void speedrunLayout(ImGuiID dockMainID) {
    ImGui::DockBuilderDockWindow("Game", dockMainID);
  }
  
  static void practiceLayout(ImGuiID dockMainID) {
    ImGuiID left1 = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Left, 0.2f, nullptr, &dockMainID);
    ImGuiID left2 = ImGui::DockBuilderSplitNode(left1, ImGuiDir_Down, 0.5f, nullptr, &left1);

    ImGuiID middle1 = dockMainID;// ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 1.0f, nullptr, &dock_main_id);
    ImGuiID middle2 = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Down, 0.33f, nullptr, &middle1);
    ImGuiID middle3 = ImGui::DockBuilderSplitNode(middle2, ImGuiDir_Right, 0.25f, nullptr, &middle2);
    
    ImGui::DockBuilderDockWindow("Common", left1);
    ImGui::DockBuilderDockWindow("Hierarchy", left2);
    ImGui::DockBuilderDockWindow("Game", middle1);
  }
  
  static void advancedLayout(ImGuiID dockMainID) {
    //ImGui::GetIO().MouseWhee
    
    ImGuiID left1 = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Left, 0.2f, nullptr, &dockMainID);
    ImGuiID left2 = ImGui::DockBuilderSplitNode(left1, ImGuiDir_Down, 0.2f, nullptr, &left1);
    ImGuiID left3 = ImGui::DockBuilderSplitNode(left2, ImGuiDir_Down, 0.75f, nullptr, &left2);
    ImGuiID left4 = ImGui::DockBuilderSplitNode(left3, ImGuiDir_Down, 0.5f, nullptr, &left3);
    ImGuiID left5 = ImGui::DockBuilderSplitNode(left4, ImGuiDir_Down, 0.3f, nullptr, &left4);
    
    ImGuiID middle1 = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Left, 0.75f, nullptr, &dockMainID);
    ImGuiID middle2 = ImGui::DockBuilderSplitNode(middle1, ImGuiDir_Down, 0.33f, nullptr, &middle1);

    ImGuiID right1 = dockMainID;
    ImGuiID right2 = ImGui::DockBuilderSplitNode(right1, ImGuiDir_Down, 0.6f, nullptr, &right1);
    ImGuiID right3 = ImGui::DockBuilderSplitNode(right2, ImGuiDir_Down, 0.6f, nullptr, &right2);
    ImGuiID right4 = ImGui::DockBuilderSplitNode(right3, ImGuiDir_Down, 0.6f, nullptr, &right3);
    
    ImGui::DockBuilderDockWindow("Common", left2);
    ImGui::DockBuilderDockWindow("Hierarchy", left3);
    ImGui::DockBuilderDockWindow("Memory editor", left4);
    ImGui::DockBuilderDockWindow("Input", left5);
    ImGui::DockBuilderDockWindow("Game", middle1);
    ImGui::DockBuilderDockWindow("AI", middle2);
    ImGui::DockBuilderDockWindow("Structure Explorer", middle2);
    ImGui::DockBuilderDockWindow("RNG", right1);
    ImGui::DockBuilderDockWindow("Cinemanager", right2);
    ImGui::DockBuilderDockWindow("Object window", right3);
    ImGui::DockBuilderDockWindow("Movie", right4);
  }
  
  static void layout(ImGuiID dockSpaceID) {
    ImGui::DockBuilderRemoveNode(dockSpaceID);
    ImGui::DockBuilderAddNode(dockSpaceID, ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::DockBuilderSetNodeSize(dockSpaceID, ImGui::GetMainViewport()->WorkSize);
    if (config["mode"] == "Speedrun") speedrunLayout(dockSpaceID);
    if (config["mode"] == "Practice") practiceLayout(dockSpaceID);
    if (config["mode"] == "Advanced") advancedLayout(dockSpaceID);
    ImGui::DockBuilderFinish(dockSpaceID);
  }
    
  
  static auto drawGraphics(void *tex) -> void {
    if (game::isValidGameState()) {
      stCameraGLI *camera = nullptr;
      if ((camera = g_stEngineStructure->viewportCamera[0])) {
        /* Construct projection matrix. Use negative fov in order to account for flipped transformations. */
        //graphics::projectionMatrix = stMatrix4D::make_perspective(-0.24f + 0.1, 640.0f / 528.0f, 0.1, 100.f);
        
        float32* rawProjection = pointer<float32>(0x80273588);
        
        stMatrix4D projectionMatrix;
        projectionMatrix.m[0] = rawProjection[0];
        projectionMatrix.m[1] = 0.0f;
        projectionMatrix.m[2] = rawProjection[1];
        projectionMatrix.m[3] = 0.0f;
        projectionMatrix.m[4] = 0.0f;
        projectionMatrix.m[5] = rawProjection[2];
        projectionMatrix.m[6] = rawProjection[3];
        projectionMatrix.m[7] = 0.0f;
        projectionMatrix.m[8] = 0.0f;
        projectionMatrix.m[9] = 0.0f;
        projectionMatrix.m[10] = rawProjection[4];
        projectionMatrix.m[11] = rawProjection[5];
        projectionMatrix.m[12] = 0.0f;
        projectionMatrix.m[13] = 0.0f;
        projectionMatrix.m[14] = -1.0f;
        projectionMatrix.m[15] = 0.0f;

        projectionMatrix(0,0) = -projectionMatrix(0,0);
        projectionMatrix(1,1) = -projectionMatrix(1,1);
        projectionMatrix(2,2) -= 1;
        projectionMatrix(3,2) *= 0.5f;
        projectionMatrix(2,3) *= 4.0f;
        
        
        pointer<stCameraGLI> camera = g_stEngineStructure->viewportCamera[0];
        //graphics::projectionMatrix = stMatrix4D::make_perspective(float(camera->xAlpha), 640.0f/528.0f, camera->near, camera->far);
        
        stMatrix4D view = camera->transform.matrix;
        view(0,1) = -(float)view(0,1); view(1,1) = -(float)view(1,1);
        view(2,1) = -(float)view(2,1); view(2,2) = -(float)view(2,2);
        view(3,1) = -(float)view(3,1); view(3,2) = -(float)view(3,2);
        view(0,2) = -(float)view(0,2); view(1,2) = -(float)view(1,2);
        
        mainContext->setProjectionMatrix(projectionMatrix);
        mainContext->setViewMatrix(view);
      }
    
//      mainContext->beginFrame();
//      //drawWorld(p_stFatherSector, stMatrix4D());
//      mainContext->endFrame();
            //graphics::beginFrame(tex, 640, 528);
      
//
            //drawWorld(p_stFatherSector, stMatrix4D());
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
    
           // graphics::endFrame();
          }
  }
  
  static void mainMenuBar() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.75f));
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("r3-tas")) {
        if (ImGui::BeginMenu("Mode")) {
          if (ImGui::MenuItem("Speedrunning", nullptr, config["mode"] == "Speedrun")) { config["mode"] = "Speedrun"; needsLayout = true; }
          if (ImGui::MenuItem("Practice", nullptr, config["mode"] == "Practice")) { config["mode"] = "Practice"; needsLayout = true; }
          if (ImGui::MenuItem("Advanced", nullptr, config["mode"] == "Advanced")) { config["mode"] = "Advanced"; needsLayout = true; }
          ImGui::EndMenu();
        }
        
        if (config["mode"] == "Speedrun") {
          ImGui::Separator();
          if (ImGui::BeginMenu("Speedrun")) {
            if (ImGui::MenuItem("New run")) {
              
            }
            
            if (ImGui::BeginMenu("Options")) {
              if (ImGui::BeginMenu("Split point")) {
                ImGui::MenuItem("First star");
                ImGui::MenuItem("Last level frame");
                ImGui::EndMenu();
              }
              ImGui::MenuItem("Remove loading times");
              ImGui::MenuItem("Upload new runs to database");
              ImGui::EndMenu();
            }
            
            ImGui::EndMenu();
          }
          ImGui::Separator();
        }
        
        if (ImGui::BeginMenu("Memory", config["mode"] != "Speedrun")) {
          ImGui::Checkbox("Readonly", &memory::readonly);
          ImGui::EndMenu();
        }
        ImGui::EndMenu();
      }
      
      ImGui::EndMainMenuBar();
    }
    ImGui::PopStyleColor();
  }

std::vector<std::pair<std::string, int>> temporaryMessages;

void addTemporaryMessage(std::string msg) {
  temporaryMessages.push_back({msg, 100});
}
  
  AIWindow *aiWindow;
  static GameWindow *gameWindow = new GameWindow();
  static CinematicWindow *cineWindow = new CinematicWindow();
  static StructureExplorerWindow *structureExplorerWindow = new StructureExplorerWindow();
  static RNGWindow *rngWindow = new RNGWindow();
  static CommonWindow *commonWindow = new CommonWindow();
  static InputWindow *inputWindow = new InputWindow();
  
  static void loadWindows() {
    aiWindow = new AIWindow(pointer<stSuperObject>(0x80BF0C0C));
  }
  
  static std::vector<pointer<stSuperObject>> actorAiProcessed;
  ImTextureID gameTexture;

  auto draw(void *c, void *texture, bool *windowed) -> void {
    

      for (auto& v : temporaryMessages) {
        if (v.second-- == 0)
          temporaryMessages.pop_back();
      }
      
    *windowed = true;
    
    GImGui = (ImGuiContext*)c;
    loadStyle();
    mainMenuBar();
    if (config["mode"] == "Speedrun") {
      drawSpeedrunTimer();
      drawRunManager();
      return;
    }
    
    gameTexture = texture;
    
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration;
    window_flags |= ImGuiWindowFlags_NoBackground;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar();
    
    dockspaceID = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    
    if (needsLayout) {
      layout(dockspaceID);
      loadStyle();
      loadWindows();
      needsLayout = false;
      
//      settings s;
//      s["hi"] = "dadasd";
//      s["test"] = true;
//      s["test2"] = 223242;
//      s["aiWindow"] = g_stEngineStructure;
//
//      s.save("test");
//
//      event("AITreeEval").subscribe("GUI", [&](Event::Param& p) {
//        pointer<stSuperObject> obj = std::any_cast<pointer<stSuperObject>>(p["object"]);
//        pointer<stNodeInterpret> tree = std::any_cast<pointer<stNodeInterpret>>(p["tree"]);
//        pointer<uGetSetParam> gs = std::any_cast<pointer<uGetSetParam>>(p["getSetParam"]);
//
//        if (obj.memoryOffset().physicalAddress() == 0x80BF0C0C) {
//          //actorAiProcessed.push_back(obj);
//
//          //aiWindow->debugMap[tree] = tree;
//        }
//      });
      
      
      
      // .fire({{"object", object}, {"tree", tree}, {"getSetParam", gsparam}});
    }

    clearMarkers();

    drawGraphics(texture);

    ImGui::SetNextWindowSizeConstraints(ImVec2(0,0), ImVec2(640,528));

    

    gameWindow->draw(static_cast<ImTextureID>(texture));

    if (config["mode"] == "Advanced") {
      
      // Draw the memory editor first in order for it not to take focus,
      // which can cause memory to be edited unknowingly when pressing buttons.
      gui::memoryEditor.ReadOnly = memory::readonly;
      gui::memoryEditor.DrawWindow("Memory editor", (void*)memory::baseAddress, memory::size);
      
      ImGui::Begin("Hierarchy");
      HierarchyWindow.Draw();
      ImGui::End();

      ImGui::Begin("Movie");
      
      ImGui::End();

      ImGui::Begin("GameSub2");
      ImGui::End();
      
      try {
        inputWindow->draw();
        commonWindow->draw();
        cineWindow->draw();
        aiWindow->draw();
        rngWindow->draw();
        structureExplorerWindow->draw();
      } catch (...) {
        
      }
        
      ImGui::Begin("Object window");
      if (game::isValidGameState()) {
        pointer<stSuperObject> spo = address(0x80BF0C0C);
        pointer<stEngineObject> eng = spo->actor;

        drawDynamics(eng->dynam->dynamics);
      }
      ImGui::End();
    }
    
    ImGui::End();
    
    ImGui::PopStyleVar(2);
    
    
      //gui::memoryEditor.HighlightFn = gui::memoryEditorHighlight;
    
      //DrawGameWindow(texture, windowed);
//
//      //if (false)
//      if (game::isValidGameState()) {
//        stCamera *camera = nullptr;
//        if ((camera = g_stEngineStructure->viewportCamera[0])) {
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
    }

auto saveLayout(const std::string& filename) -> void {
  std::string file = settings::folder() + "layout." + filename;
  ImGui::SaveIniSettingsToDisk(file.c_str());
}

auto loadLayout(const std::string& filename) -> void {
  std::string file = settings::folder() + "layout." + filename;
  //ImGui::LoadIniSettingsFromDisk(file.c_str());
}
  
ImVec4 projectWorldCoordinate(stVector3D P) {
  try {
    #define ASPECT (528.0f / 640.0f)
    #define PROJECTION_RATIO_X  0.377f
    #define PROJECTION_RATIO_Y  0.708f
    
    pointer<stCameraGLI> camera = g_stEngineStructure->viewportCamera[0];
    if (!camera)
      return ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    stMatrix4D view = camera->transform.matrix;
    /* Change sign of the two middle columns (flipping the rotation) */
    view(0,1) = -(float)view(0,1); view(1,1) = -(float)view(1,1);
    view(2,1) = -(float)view(2,1); view(2,2) = -(float)view(2,2);
    view(3,1) = -(float)view(3,1); view(3,2) = -(float)view(3,2);
    view(0,2) = -(float)view(0,2); view(1,2) = -(float)view(1,2);
    
    const float fov = camera->xAlpha;
    stMatrix4D projection = stMatrix4D::make_perspective(fov, ASPECT, camera->near, camera->far);
    stMatrix4D viewprojection = projection * view;
    
    stVector4D P2 = stVector4D(P.x(), P.y(), P.z(), 1.0f);
    stVector4D R = viewprojection * P2;
    
    float xp = (R.x() / R.w()) * PROJECTION_RATIO_X + 0.5f;
    float yp = (R.y() / R.w()) * PROJECTION_RATIO_Y + 0.5f;
    
    return ImVec4(xp, yp, R.z(), R.w());
  } catch (bad_pointer& e) {
    return ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  }
}

}

std::deque<stMatrix4D> viewMatrixStack = {{},{}};
