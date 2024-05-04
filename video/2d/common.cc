#include "gui.hh"
#include "xray.hpp"

struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 590) {
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

CommonWindow::CommonWindow() {
  /* ... */
}

void CommonWindow::draw() {
  static ScrollingBuffer speedbuffer_h;
  static ScrollingBuffer speedbuffer_v;
  static ScrollingBuffer rotationbuffer;
  static float t = 0.0f;
  
  ImGui::Begin("Common");
  
  try {
    t += ImGui::GetIO().DeltaTime;
    float h = game::g_stEngineStructure->currentMainPlayers[0]->actor->horizontalSpeed();
    float v = game::g_stEngineStructure->currentMainPlayers[0]->actor->verticalSpeed();
    float r = game::g_stEngineStructure->currentMainPlayers[0]->actor->dynam->dynamics->base.report->currentAbsoluteSpeed.angular.angle;
    
    //printf("%X\n", game::g_stEngineStructure->currentMainPlayers[0]->actor->dynam->dynamics->base.numFrames.memoryOffset().physicalAddress());
    
    
    //stMatrix3D rot = game::g_stEngineStructure->currentMainPlayers[0]->actor->dynam->dynamics->base.imposedRotation;
//    ImGui::Text("[%.2f  %.2f  %.2f]", float(rot.m[0]), float(rot.m[1]), float(rot.m[2]));
//    ImGui::Text("[%.2f  %.2f  %.2f]", float(rot.m[3]), float(rot.m[4]), float(rot.m[5]));
//    ImGui::Text("[%.2f  %.2f  %.2f]", float(rot.m[6]), float(rot.m[7]), float(rot.m[8]));
    
    gui::drawTransform(game::g_stEngineStructure->standardCamera->globalTransform);
    
    speedbuffer_h.AddPoint(t, h);
    speedbuffer_v.AddPoint(t, v);
    rotationbuffer.AddPoint(t, r);
  } catch (bad_pointer& e) {
    std::cout << "Failed to get speed data: " << e.what() << "\n";
  }
  
  static float history = 10;
  
 
  ImPlot::SetNextAxisToFit(ImAxis_Y1);
  if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1,150), ImPlotFlags_NoFrame)) {
    
    ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_AutoFit);
    ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
   // ImPlot::SetupAxisLimits(ImAxis_Y1,0,-1);
    ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 1.0f);
    //ImPlot::PlotShaded("Speed", &speedbuffer.Data[0].x, &speedbuffer.Data[0].y, speedbuffer.Data.size(), -INFINITY, 0, speedbuffer.Offset, 2 * sizeof(float));
    if (speedbuffer_h.Data.size() > 1) {
      ImPlot::PlotLine("HSpeed", &speedbuffer_h.Data[0].x, &speedbuffer_h.Data[0].y, speedbuffer_h.Data.size(), 0, speedbuffer_h.Offset, 2*sizeof(float));
      ImPlot::PlotLine("VSpeed", &speedbuffer_v.Data[0].x, &speedbuffer_v.Data[0].y, speedbuffer_v.Data.size(), 0, speedbuffer_v.Offset, 2*sizeof(float));
      ImPlot::PlotLine("AngleR", &rotationbuffer.Data[0].x, &rotationbuffer.Data[0].y, rotationbuffer.Data.size(), 0, rotationbuffer.Offset, 2*sizeof(float));
    }
    ImPlot::EndPlot();
  }
  
  
 
  if (game::isValidGameState()) {
    ImGui::Text("Position: ");
    ImGui::SameLine();
    gui::drawVector3(game::g_stEngineStructure->currentMainPlayers[0]->position());
    ImGui::Text("Speed: ");
    ImGui::SameLine();
    gui::drawVector3(game::g_stEngineStructure->currentMainPlayers[0]->actor->speed());
    ImGui::SameLine();
    ImGui::Text("(%.3f, %.3f)", game::g_stEngineStructure->currentMainPlayers[0]->actor->horizontalSpeed(), game::g_stEngineStructure->currentMainPlayers[0]->actor->verticalSpeed());
  }
    
  
  if (ImGui::Button("Generate xrays!"))
  //if (game::isValidGameState())
  {
    static float dirX = 0.0f, dirY = 0.0f;
    extern stVector3D genericDirection;
    ImGui::SliderFloat("##X", &dirX, -10.0f, 10.0f);
    ImGui::SliderFloat("##Y", &dirY, -10.0f, 10.0f);
//    genericDirection.x = -dirX;
//    genericDirection.y = dirY;
    
    
    
    pointer<stSuperObject> currentSector = sectorSearch(game::p_stFatherSector, game::g_stEngineStructure->currentMainPlayers[0]->position());
    
    xRAY xray;
    std::vector<stVector3D> list = xray.derivePoints(currentSector);
    std::vector<xRAY::ThetaNode*> theta_nodes;
    
    xRAY::ThetaNode *start = new xRAY::ThetaNode;
    start->vertex = game::g_stEngineStructure->currentMainPlayers[0]->position();
    theta_nodes.push_back(start);
    
    for (auto& vertex : list) {
      xRAY::ThetaNode *node = new xRAY::ThetaNode;
      node->vertex = vertex;
      theta_nodes.push_back(node);
    }
    
//    xRAY::ThetaNode *goal = new xRAY::ThetaNode;
//    goal->vertex = stVector3D();
//    goal->vertex.x = 54.0f;
//    goal->vertex.y = 38.0f;
//    goal->vertex.z = -2.0f;
//    theta_nodes.push_back(goal);
    
    // construct neighbors
    for (auto& node : theta_nodes) {
      while (node->neighbors.size() < 5) {
        float radius = 10.0;
        for (auto& maybeNeighbor : theta_nodes) {
          if ((maybeNeighbor->position() - node->position()).length() < radius) {
            node->neighbors.push_back(maybeNeighbor);
            if (node->neighbors.size() >= 5) break;
          }
        }
        radius *= 2.0f;
      }
    }
    
    
    
    std::vector<xRAY::ThetaNode*> out = xray.theta(start, theta_nodes.back());
    
    gui::xrayPoints.clear();
    for (auto& node : out) {
      gui::xrayPoints.push_back(node->position());
    }
//    for (auto& tri : list) {
//      auto vertices = tri.vertices();
//      gui::xrayPoints.push_back((stVector3D)vertices[0]);
//      gui::xrayPoints.push_back((stVector3D)vertices[1]);
//    }
  }
  
  ImGui::End();
}
