#ifndef event_hh
#define event_hh

#include <any>
#include <map>
#include <functional>
#include <string>
#include <vector>

class Event {
public:
  enum class Type {
    FrameEvent, // Event is scheduled before/after frame
    SubFrameEvent // Event is scheduled whenever inside the frame
  };
  
  using Param = std::map<std::string, std::any>;
  using Function = std::map<std::string, std::any>;
  using Listener =
  
  class Subscriber {
  public:
    using Function = std::function<void(Param&)>;
    Function fn;
    Subscriber(Function& fn) : fn(fn) {
      /* ... */
    }
    
    void operator ()(Param& p) {
      fn(p);
    }
  };
  
  Event(std::string name) : name(name) {
    /* ... */
  }
  
  /** fire the event with specified parameters */
  void fire(Param p = {});
  
  void subscribe(std::string name, Listener::Function listener) {
    subscribers.emplace_back(listener);
  }
  
private:
  std::string name;
  std::vector<Subscriber> subscribers;
};

using EventList = std::map<std::string, Event*>;

#endif /* event_hh */
