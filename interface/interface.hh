#ifndef interface_hh
#define interface_hh

#include <map>
#include <string>

#include "emulator.hh"
#include "event.hh"

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
  
  EventList events;
  
  const std::string Host = "";
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

static inline Event& event(std::string name) {
  EventList& events = interface->events;
  if (events.find(name) == events.end())
    events[name] = new Event(name);
  return *events[name];
}

#endif /* interface_hh */
