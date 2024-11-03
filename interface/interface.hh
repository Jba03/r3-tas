#ifndef interface_hh
#define interface_hh

#include <map>
#include <string>
#include <filesystem>

#include <cpatools/serialize.hpp>
using nlohmann::json;
using nlohmann::ordered_json;

#include "emulator.hh"
#include "event.hh"
#include "settings.hh"

struct Interface {
  Interface();
  
//  Debugger *debugger;
//  CollisionTracker *collisionTracker;
  
  //void Update(emulator::message *msg);
  
  bool initialize();
  void applyOptimizations();
  void r3solveInit();
  
  /// Mode selection:
  ///   Speedrun
  ///   Practice
  ///   Advanced
  std::string mode = "Practice";
  
  EventList events;
  
  std::filesystem::path configPath();
  
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

extern ordered_json config;

#endif /* interface_hh */
