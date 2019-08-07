#pragma once
#include <limits.h>
#include <stdint.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "mesh.h"

struct DirectedEdge {
  int nodeIndex;
  float distance;
  void computeDistance(cv::Point2f p1, cv::Point2f p2) {
    distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
  }
};

struct Node {
  cv::Point2f point;
  int timesVisited = 1;
  bool closed = false;
  float distanceToGoal;
  std::vector<DirectedEdge> edges;
};

struct Graph {
  std::vector<Node> nodes;
};

struct FringeElement {
  float distance;
  std::vector<int> path;
  DirectedEdge edge;
};

class PathFinder {
 public:
  PathFinder(std::vector<Polygon> polygons, std::vector<Vertex> vertices);
  ~PathFinder();
  std::vector<std::vector<cv::Point2f>> ComputeNPaths(float C = 1.0, int N = 1);
  Graph GetGraph();
  float DetermineExplorationRate(cv::Mat frame);

 private:
  Graph _graph;
  int _startNode = 0;
  int _endNode = 0;
  void GetStartAndEnd();
  void ComputeHeuristics();
  std::vector<cv::Point2f> ComputeSinglePath(int N, float C);
  float DetermineDirection(std::vector<cv::Point2f> path, DirectedEdge edge);
  float CostFunction(Node node, float distanceTravelled, int N, float C);
  bool NotFinished(std::vector<FringeElement> fringe);
  std::vector<cv::Point2f> CreatePath(std::vector<FringeElement> fringe);
  void Reset();
};
