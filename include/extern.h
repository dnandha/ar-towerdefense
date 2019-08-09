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
      : _relativePath{relativePath} {
      _registration = EventBus::AddHandler(*this);
  }
  ~AbsolutePath() {
      // remove event handlers
      _registration->RemoveHandler();
  }

  cv::Vec3d GetPoint(int index);

 protected:
  void OnEvent(HomographyComputedEvent& e);

 private:
  EventRegistration* _registration;
  std::vector<cv::Point2f> _relativePath;
  Mat _homography;
  Mat _camMat;
  Marker _m0;
  Marker _m1;
  bool _homo_computed = false;
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
