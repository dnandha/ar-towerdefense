#ifndef _EXTERN_H
#define	_EXTERN_H

#include "event.h"

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
    MarkerDetection(CV* cv, EventBus bus);
};

/*
 * Holds some sort of path finding logic
 */
class PathFinding {
  public:
    PathFinding(CV* cv, EventBus bus);
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
