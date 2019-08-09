#ifndef _EXTERN_H
#define _EXTERN_H

#include "event_bus.hpp"
#include "events.h"
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
 * Holds the relative path but delivers the absolute points
 */
class AbsolutePath : public EventHandler<HomographyComputedEvent> {
 public:
  AbsolutePath(std::vector<cv::Point2f> relativePath)
      : _relativePath{relativePath} {}

  cv::Point2f GetPoint(int index);

 protected:
  void OnEvent(HomographyComputedEvent& e);

 private:
  std::vector<cv::Point2f> _relativePath;
};

struct CamPosition {
  Vec3d tvec;
  Vec3d rvec;
};

/*
 *
 */
class MarkerDetection {
  ImageProcessor _imgproc;

 public:
  MarkerDetection() { _imgproc = ImageProcessor(); }

  void Detect(Mat img);
};

#endif
