#ifndef _EVENTS_H
#define _EVENTS_H

#include <string>

#include "image_processor.h"

/*
 *
 */
class Event {
 public:
  Event() { _canceled = false; };

  // DONT DELETE: virtual member is important for typeid operator behaviour
  virtual ~Event() {}

  bool GetCanceled() { return _canceled; };
  void SetCanceled(bool canceled) { _canceled = canceled; };

 private:
  bool _canceled;
};

class MarkersDetectedEvent : public Event {
  Marker _m;

 public:
  MarkersDetectedEvent(Marker m) : _m(m) {}
  Marker GetMarker() { return _m; }
};

class UnitKilledEvent : public Event {
  std::string _name;

 public:
  UnitKilledEvent(std::string name) : _name(name) {}
  std::string GetName() { return _name; }
};

class HomographyComputedEvent : public Event {
  cv::Mat _homography;

 public:
  HomographyComputedEvent(cv::Mat homography) : _homography{homography} {}
  cv::Mat GetHomography() { return _homography; }
}

#endif
