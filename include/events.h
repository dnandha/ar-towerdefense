#ifndef _EVENTS_H
#define _EVENTS_H

#include "event_bus.h"

// TODO: add detection lib depenency to specifiy list of markers as args
class MarkersDetectedEvent : public Event {
 public:
  MarkersDetectedEvent(std::string mockArg) : _mockArg(mockArg) {}

  std::string GetMockArg() { return _mockArg; };

 private:
  std::string _mockArg;
};

#endif
