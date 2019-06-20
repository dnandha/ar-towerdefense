#ifndef _EVENTS_H
#define _EVENTS_H

#include "event_bus.h"

class BuildTowerEvent : public Event {
 public:
  BuildTowerEvent(std::string mockArg) : _mockArg(mockArg) {}

  std::string GetMockArg() { return _mockArg; };

 private:
  std::string _mockArg;
};

#endif
