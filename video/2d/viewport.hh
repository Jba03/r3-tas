#pragma once

struct Viewport {
  Viewport();
  virtual void draw() = 0;
  auto hasMouse() -> bool;
  
  float alpha = 1.0f;
  ImVec2 size = ImVec2(640, 528);
  ImPlotPoint position;
};

struct GameViewport : Viewport {
  GameViewport();
  void draw();
  
  ImVec2 size = ImVec2(640, 528);
  ImPlotPoint position;
};

struct CollisionViewport : Viewport {
  CollisionViewport();
  void draw();
  
  ImVec2 size = ImVec2(640, 528);
  ImPlotPoint position = ImPlotPoint(640, 0);
  float alpha = 1.0f;
  
private:
  GraphicsContext* graphics = MakeGraphicsContext();
};

struct TopdownCollisionViewport : Viewport {
  TopdownCollisionViewport();
  void draw();
  
  ImVec2 size = ImVec2(640, 528);
  ImPlotPoint position = ImPlotPoint(640, 528);
  float alpha = 1.0f;
  
private:
  GraphicsContext* graphics = MakeGraphicsContext();
};
