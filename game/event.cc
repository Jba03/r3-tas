#include "event.hh"

void Event::fire(Event::Param p) {
  for (auto& s : subscribers) s(p);
}
