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
  cv::Mat frame = cam.GetFrame();
  for (int p = 0; p < paths.size(); p++) {
    cv::Scalar colour((p + 1) * 25, 0, 0);
    detector.DrawPath(frame, paths[p], colour);
    cv::namedWindow("TEST", 1);
    cv::imshow("TEST", frame);
    cv::waitKey(0);
  }
  return 1;
}