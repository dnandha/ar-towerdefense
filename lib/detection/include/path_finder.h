/*
* ARTD (Augmented Reality Tower Defense)
* Copyright (C) 2019 Jaeger,Stegmueller,Boche,Nandha 
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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

struct GNode {
  cv::Point2f point;
  int timesVisited = 1;
  bool closed = false;
  float distanceToGoal;
  std::vector<DirectedEdge> edges;
};

struct Graph {
  std::vector<GNode> nodes;
};

struct FringeElement {
  float distance;
  std::vector<int> path;
  DirectedEdge edge;
};

/**
 * Find path using A* algorithm
 */
class PathFinder {
 public:
  PathFinder(std::vector<Polygon> polygons, std::vector<Vertex> vertices);
  ~PathFinder();
  bool ComputeNPaths(int N = 1);
  Graph GetGraph();
  std::vector<std::vector<cv::Point2f>> GetPaths();

 private:
  Graph _graph;
  int _startNode = 0;
  int _endNode = 0;
  std::vector<std::vector<cv::Point2f>> _paths;
  void GetStartAndEnd();
  void ComputeHeuristics();
  bool ComputeSinglePath(int N);
  float DetermineDirection(std::vector<cv::Point2f> path, DirectedEdge edge);
  float CostFunction(GNode node, float distanceTravelled, int N);
  bool NotFinished(std::vector<FringeElement> fringe);
  std::vector<cv::Point2f> CreatePath(std::vector<FringeElement> fringe);
  void Reset();
};
