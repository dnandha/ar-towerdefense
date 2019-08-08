#include "cam.h"
#include "paths_initializer.h"

/*
 * Main
 */
int main() {
  Cam cam(1);
  PathsInitializer initializer(cam);
  std::vector<std::vector<cv::Point2f>> paths = initializer.InitializePaths();
  return 1;
}