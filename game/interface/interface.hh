#ifndef interface_hh
#define interface_hh

#include <map>
#include <string>

#include "emulator.hh"

//#include "bruteforce.hh"
//#include "debugger.hh"
//#include "collision.hh"

//struct Message {
//  const char *msg;
//  void *data;
//  uint64_t ts;
//};

struct Interface {
  Interface();
  
//  Debugger *debugger;
//  CollisionTracker *collisionTracker;
  
  //void Update(emulator::message *msg);
  
  bool initialize();
  void applyOptimizations();
  
  const std::string Host = "";
  
private:
  
  //std::string configPath;
};

struct GCNInterface : public Interface {
  GCNInterface();
  const std::string Host = "GCN";
};

struct NativeInterface : public Interface {
  NativeInterface();
  const std::string Host = "PC";
};

#endif /* interface_hh */
