#include "path_detector.h"

PathDetector::PathDetector() {}

PathDetector::~PathDetector() {}

void PathDetector::RangeThresholdBinary(cv::Mat* frame_pointer,
                                        cv::Scalar lowerBoundary,
                                        cv::Scalar upperBoundary) {
  inRange(*frame_pointer, lowerBoundary, upperBoundary, *frame_pointer);
}

void PathDetector::Gaussian(cv::Mat* frame_pointer) {
  // Gauss
  GaussianBlur(*frame_pointer, *frame_pointer, cv::Size(3, 3), 0, 0, 4);
  cvtColor(*frame_pointer, *frame_pointer, cv::COLOR_BGR2GRAY);
}

void PathDetector::Laplace(cv::Mat* frame_pointer) {
  // Gauss->Laplace->Edgedetection
  Gaussian(frame_pointer);

  cv::Mat help;
  // Laplace
  Laplacian(*frame_pointer, help, 3, 1, 1.0, 0.0, 4);
  convertScaleAbs(help, *frame_pointer);
}

void PathDetector::CannyTwo(cv::Mat frame) { Canny(frame, frame, 50, 200, 3); }

std::vector<cv::Point2f> PathDetector::DrawHoughLines(cv::Mat frame,
                                                      int threshold,
                                                      double minLineLength,
                                                      double maxLineGap) {
  cv::Mat help = frame.clone();
  // Canny-> HoughLines

  Canny(help, frame, 50, 200, 3);

  cvtColor(frame, help, cv::COLOR_GRAY2BGR);

  //#if 0

  std::vector<cv::Point2f> corners;
  std::vector<cv::Vec4i> lines;

  HoughLinesP(frame, lines, 1, 3.1415 / 180, threshold, minLineLength,
              maxLineGap);

  for (size_t i = 0; i < lines.size(); i++) {
    cv::Vec4i l = lines[i];
    line(help, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]),
         cv::Scalar(0, 0, 255), 3);
    corners.push_back({(float)l[0], (float)l[1]});
    corners.push_back({(float)l[2], (float)l[3]});
  }
  imshow("Hough", help);
  frame = help.clone();

  return corners;
}

std::vector<cv::Point2f> PathDetector::Cornerdetection(
    cv::Mat frame, double minDistance, int maxCorners, double qualityLevel,
    int blockSize, bool useHarrisDetector, double k) {
  cv::Mat help;
  cvtColor(frame, help, cv::COLOR_BGR2GRAY);
  inRange(help, cv::Scalar(0, 0, 0), cv::Scalar(100, 100, 100), help);
  Canny(help, frame, 50, 200, 3);

  std::vector<cv::Point2f> corners;
  goodFeaturesToTrack(frame, corners, maxCorners, qualityLevel, minDistance,
                      cv::Mat(), blockSize, useHarrisDetector, k);

  return corners;
}

void PathDetector::AdaptiveThreshold(cv::Mat* frame_pointer) {
  cv::Mat help;
  cvtColor(*frame_pointer, help, cv::COLOR_BGR2GRAY);
  // Adaptive Threshhold
  adaptiveThreshold(help, *frame_pointer, 200, cv::ADAPTIVE_THRESH_MEAN_C,
                    cv::THRESH_BINARY, 3, 2);
}

void PathDetector::AdaptiveThresholdBinary(cv::Mat* frame_pointer) {
  // Adaptive ThreshholdXBin�r
  AdaptiveThreshold(frame_pointer);
  cv::Mat help, bnw;
  cv::Mat frame = *frame_pointer;

  inRange(*frame_pointer, cv::Scalar(0, 0, 0), cv::Scalar(75, 75, 75), bnw);
  help = frame.mul(bnw);

  frame_pointer = &frame;
  *frame_pointer = help.clone();
}

void PathDetector::PathMeshInImg(Mesh mesh, cv::Mat frame) {
  cv::RNG rng(12345);
  cv::Scalar color;
  for (int p = 0; p < mesh._polygons.size(); p++) {
    color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
                       rng.uniform(0, 255));
    line(frame,
         cv::Point(mesh._vertices[mesh._polygons[p].indexA].x,
                   mesh._vertices[mesh._polygons[p].indexA].y),
         cv::Point(mesh._vertices[mesh._polygons[p].indexB].x,
                   mesh._vertices[mesh._polygons[p].indexB].y),
         color);
    line(frame,
         cv::Point(mesh._vertices[mesh._polygons[p].indexB].x,
                   mesh._vertices[mesh._polygons[p].indexB].y),
         cv::Point(mesh._vertices[mesh._polygons[p].indexC].x,
                   mesh._vertices[mesh._polygons[p].indexC].y),
         color);
    line(frame,
         cv::Point(mesh._vertices[mesh._polygons[p].indexC].x,
                   mesh._vertices[mesh._polygons[p].indexC].y),
         cv::Point(mesh._vertices[mesh._polygons[p].indexA].x,
                   mesh._vertices[mesh._polygons[p].indexA].y),
         color);
  }
}

void PathDetector::PathCentroids(Mesh mesh, cv::Mat frame) {
  for (int p = 0; p < mesh._polygons.size() - 1; p++) {
    line(frame,
         cv::Point(mesh._polygons[p].centroid.x, mesh._polygons[p].centroid.y),
         cv::Point(mesh._polygons[p + 1].centroid.x,
                   mesh._polygons[p + 1].centroid.y),
         cv::Scalar(0, 0, 255));
  }
}

void PathDetector::DrawPath(cv::Mat frame, std::vector<cv::Point2f> path,
                            cv::Scalar colour) {
  for (int p = 1; p < path.size(); p++) {
    line(frame, path[p - 1], path[p], colour);
  }
}

void PathDetector::DrawGraph(cv::Mat frame, Graph graph) {
  for (int n = 0; n < graph.nodes.size(); n++) {
    Node node = graph.nodes[n];
    for (int e = 0; e < node.edges.size(); e++) {
      line(frame, node.point, graph.nodes[node.edges[e].nodeIndex].point,
           cv::Scalar(0, 0, 255));
    }
  }
}
