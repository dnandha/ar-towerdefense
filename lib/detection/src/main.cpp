#include "cam.h"
#include "path_detector.h"
#include "paths_initializer.h"

/*
 * Main
 */
int main() {
  Cam cam(1);
  PathDetector detector;
  PathsInitializer initializer(cam);
  std::vector<std::vector<cv::Point2f>> paths = initializer.InitializePaths();
  return 1;
}