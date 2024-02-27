#include "gui.hh"

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

CommonWindow::CommonWindow() {
  /* ... */
}

void CommonWindow::draw() {
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
}
