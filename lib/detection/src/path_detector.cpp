#include "path_detector.h"

Size PathDetector::FrameSize() { return _frame.size(); }

Mat PathDetector::RangeThresholdBinary(Mat frame, Scalar lowerBoundary,
                                       Scalar upperBoundary) {
  Mat grayscaleImage;
  inRange(frame, lowerBoundary, upperBoundary, grayscaleImage);

  return grayscaleImage;
}

void PathDetector::Gaussian() {
  // Gauss
  GaussianBlur(_frame, _modifiedFrame, Size(3, 3), 0, 0, 4);
  cvtColor(_frame, _modifiedFrame, COLOR_BGR2GRAY);
  _currentMask = "Gauss";
}

void PathDetector::Laplace() {
  // Gauss->Laplace->Edgedetection
  Gaussian();

  Mat help;
  // Laplace
  Laplacian(_modifiedFrame, help, 3, 1, 1.0, 0.0, 4);
  convertScaleAbs(help, _modifiedFrame);
  _currentMask = "Laplace auf Gauss";
}

void PathDetector::CannyTwo() {
  Canny(_frame, _modifiedFrame, 50, 200, 3);
  _currentMask = "Canny";
}

void PathDetector::DrawHoughLines(int threshold, double minLineLength,
                                  double maxLineGap) {
  // Canny-> HoughLines
  CannyTwo();

  Mat help;

  cvtColor(_modifiedFrame, help, COLOR_GRAY2BGR);

  //#if 0

  vector<Vec4i> lines;

  HoughLinesP(_modifiedFrame, lines, 1, 3.1415 / 180, threshold, minLineLength,
              maxLineGap);

  for (size_t i = 0; i < lines.size(); i++) {
    Vec4i l = lines[i];
    line(help, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3);
  }

  _modifiedFrame = help.clone();
  _currentMask = "HoughLines auf Canny";
}

vector<Point2f> PathDetector::Cornerdetection(Mat frame, int maxCorners,
                                              double qualityLevel,
                                              double minDistance, int blockSize,
                                              bool useHarrisDetector,
                                              double k) {
  RNG rng(12345);
  Mat help;
  vector<Point2f> corners;

  // Cornerdetection
  goodFeaturesToTrack(frame, corners, maxCorners, qualityLevel, minDistance,
                      Mat(), blockSize, useHarrisDetector, k);

  return corners;
}

void PathDetector::AdaptiveThreshold() {
  Mat help;
  cvtColor(_frame, help, COLOR_BGR2GRAY);
  // Adaptive Threshhold
  adaptiveThreshold(help, _modifiedFrame, 200, ADAPTIVE_THRESH_MEAN_C,
                    THRESH_BINARY, 3, 2);
  _currentMask = "Adaptive Threshold";
}

void PathDetector::AdaptiveThresholdBinary() {
  // Adaptive ThreshholdXBin�r
  AdaptiveThreshold();
  Mat help, bnw;
  inRange(_frame, Scalar(0, 0, 0), Scalar(75, 75, 75), bnw);
  help = _modifiedFrame.mul(bnw);
  _modifiedFrame = help.clone();
  _currentMask = "Adaptive Threshold Binary";
}

Mat PathDetector::PathMeshInImg(Mat frame, Mesh mesh) {
  Mat pathImage = frame.clone();
  RNG rng(12345);
  Scalar color;
  for (int p = 0; p < mesh.polygons.size(); p++) {
    color =
        Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
    line(pathImage,
         Point(mesh.vertices[mesh.polygons[p].indexA].x,
               mesh.vertices[mesh.polygons[p].indexA].y),
         Point(mesh.vertices[mesh.polygons[p].indexB].x,
               mesh.vertices[mesh.polygons[p].indexB].y),
         color);
    line(pathImage,
         Point(mesh.vertices[mesh.polygons[p].indexB].x,
               mesh.vertices[mesh.polygons[p].indexB].y),
         Point(mesh.vertices[mesh.polygons[p].indexC].x,
               mesh.vertices[mesh.polygons[p].indexC].y),
         color);
    line(pathImage,
         Point(mesh.vertices[mesh.polygons[p].indexC].x,
               mesh.vertices[mesh.polygons[p].indexC].y),
         Point(mesh.vertices[mesh.polygons[p].indexA].x,
               mesh.vertices[mesh.polygons[p].indexA].y),
         color);
  }

  return pathImage;
}

Mat PathDetector::PathCentroids(Mat frame, Mesh mesh) {
  Mat centroidImage = frame.clone();
  for (int p = 0; p < mesh.polygons.size() - 1; p++) {
    line(
        centroidImage,
        Point(mesh.polygons[p].centroid.x, mesh.polygons[p].centroid.y),
        Point(mesh.polygons[p + 1].centroid.x, mesh.polygons[p + 1].centroid.y),
        Scalar(0, 0, 255));
  }

  return centroidImage;
}
