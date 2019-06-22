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
  PathDetector() {}
  PathDetector(Mat frame) : _frame(frame) {}

  Size FrameSize();
  Mat RangeThresholdBinary(Mat frame, Scalar lowerBoundary = Scalar(0, 0, 0),
                           Scalar upperBoundary = Scalar(100, 100, 100));
  void Gaussian();
  void Laplace();
  void CannyTwo();
  void DrawHoughLines(int threshold = 1, double minLineLength = 10,
                      double maxLineGap = 5);
  vector<Point2f> Cornerdetection(Mat frame, int maxCorners = 1000,
                                  double qualityLevel = 0.001,
                                  double minDistance = 25, int blockSize = 3,
                                  bool useHarrisDetector = false,
                                  double k = 0.04);
  void AdaptiveThreshold();
  void AdaptiveThresholdBinary();
  Mat PathMeshInImg(Mat frame, Mesh mesh);
  Mat PathCentroids(Mat frame, Mesh mesh);

 private:
  Mat _frame;
  Mat _modifiedFrame;
  string _currentMask;
};

#endif