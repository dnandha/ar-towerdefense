#ifndef _EXTERN_H
#define _EXTERN_H

#include "event_bus.hpp"
#include "renderer.h"

#include "cam.h"
#include "image_processor.h"

/*
 * Computer vision component
 */
class CV {
  // todo
};

/*
 * Holds some sort of path finding logic
 */
class PathFinding {
 public:
  PathFinding();
};

struct CamPosition
{
    Vec3d tvec;
    Vec3d rvec;
};

/*
 *
 */
class MarkerDetection {
    ImageProcessor _imgproc;
public:
    MarkerDetection() {
        _imgproc = ImageProcessor();
    }
    
    void Detect(Mat img);
};

#endif
