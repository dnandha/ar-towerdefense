#ifndef _PATH_DETECTOR_H
#define _PATH_DETECTOR_H

#include <stdint.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "mesh.h"

using namespace cv;
using namespace std;
class PathDetector {
 public:
  PathDetector();
  PathDetector(string);
  ~PathDetector();
  void ShowFrame();
  Size FrameSize();
  void ShowModifiedFrame();
  void RangeThresholdBinary(Scalar lowerBoundary = Scalar(0, 0, 0),
                            Scalar upperBoundary = Scalar(75, 75, 75));
  void Gaussian();
  void Laplace();
  void CannyTwo();
  void DrawHoughLines(int threshold = 1, double minLineLength = 10,
                      double maxLineGap = 5);
  vector<Point2f> Cornerdetection(int maxCorners = 200,
                                  double qualityLevel = 0.0001,
                                  double minDistance = 30, int blockSize = 3,
                                  bool useHarrisDetector = false,
                                  double k = 0.04);
  void AdaptiveThreshold();
  void AdaptiveThresholdBinary();
  void PathMeshInImg(Mesh mesh);
  void PathCentroids(Mesh mesh);

 private:
  Mat frame;
  Mat modifiedFrame;
  string currentMask;
};

#endif