#ifndef interface_hh
#define interface_hh

#include <map>
#include <string>

#include "emulator.hh"

enum InterfaceMode {
  Speedrun,
  Practice,
  Advanced
};

struct Interface {
  Interface();
  
//  Debugger *debugger;
//  CollisionTracker *collisionTracker;
  
  //void Update(emulator::message *msg);
  
  bool initialize();
  void applyOptimizations();
  
  InterfaceMode mode = Advanced;
  
  const std::string Host = "";
  
  //std::string configPath;
};

extern Interface *interface;

struct GCNInterface : public Interface {
  GCNInterface();
  const std::string Host = "GCN";
};

struct NativeInterface : public Interface {
  NativeInterface();
  const std::string Host = "PC";
};

#endif /* interface_hh */
