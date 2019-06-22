#ifndef _EXTERN_H
#define _EXTERN_H

#include "event_bus.h"
#include "events.h"

/*
 * Computer vision component
 */
class CV {
  // todo
};

/*
 *
 */
class MarkerDetection {
 public:
  //MarkerDetection();
  void Detect();
};

/*
 * Holds some sort of path finding logic
 */
class PathFinding {
 public:
  PathFinding();
};

/*
 * Rendering backend / engine
 */
class Ogre {
 public:
  void SetCamera();
  void SetBackground();
};

#endif
