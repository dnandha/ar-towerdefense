#pragma once
#include "cam.h"
#include "image_processor.h"
#include "mesh.h"
#include "path_detector.h"

class PathsInitializer {
 public:
  PathsInitializer(Cam cam) : _cam{cam} {}
  std::vector<std::vector<cv::Point2f>> InitializePaths();

 private:
  Cam _cam;
  std::vector<std::vector<cv::Point2f>> _paths;
  bool DetectPaths(cv::Mat frame);
  std::vector<std::vector<cv::Point2f>> SetPathsInPerspective(
      ImageProcessor improc);
};