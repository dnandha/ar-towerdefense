#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "mesh.h"
#include "path_finder.h"

class PathDetector {
 public:
  PathDetector();
  ~PathDetector();
  void RangeThresholdBinary(cv::Mat* frame_pointer,
                            cv::Scalar lowerBoundary = cv::Scalar(0, 0, 0),
                            cv::Scalar upperBoundary = cv::Scalar(75, 75, 75));
  void Gaussian(cv::Mat* frame_pointer);
  void Laplace(cv::Mat* frame_pointer);
  void CannyTwo(cv::Mat frame);
  std::vector<cv::Point2f> DrawHoughLines(cv::Mat frame, int threshold = 1,
                                          double minLineLength = 5,
                                          double maxLineGap = 5);
  std::vector<cv::Point2f> Cornerdetection(cv::Mat frame, double minDistance,
                                           int maxCorners = 1000,
                                           double qualityLevel = 0.1,
                                           int blockSize = 3,
                                           bool useHarrisDetector = false,
                                           double k = 0.04);
  void AdaptiveThreshold(cv::Mat* frame_pointer);
  void AdaptiveThresholdBinary(cv::Mat* frame_pointer);
  void PathMeshInImg(Mesh mesh, cv::Mat frame);
  void PathCentroids(Mesh mesh, cv::Mat frame);
  void DrawPath(cv::Mat frame, std::vector<cv::Point2f> path,
                cv::Scalar colour);
  void DrawGraph(cv::Mat frame, Graph graph);

 private:
};
