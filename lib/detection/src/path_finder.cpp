#include "path_finder.h"

PathFinder::PathFinder(std::vector<Polygon> polygons,
                       std::vector<Vertex> vertices) {
  for (int p = 0; p < polygons.size(); p++) {
    Node node;
    node.point = {(float)polygons[p].centroid.x, (float)polygons[p].centroid.y};
    _graph.nodes.push_back(node);

    for (int l = 0; l < polygons.size(); l++) {
      if (p != l) {
        int counter = 0;
        if (polygons[p].indexA == polygons[l].indexA ||
            polygons[p].indexA == polygons[l].indexB ||
            polygons[p].indexA == polygons[l].indexC) {
          counter++;
        }
        if (polygons[p].indexB == polygons[l].indexA ||
            polygons[p].indexB == polygons[l].indexB ||
            polygons[p].indexB == polygons[l].indexC) {
          counter++;
        }
        if (polygons[p].indexC == polygons[l].indexA ||
            polygons[p].indexC == polygons[l].indexB ||
            polygons[p].indexC == polygons[l].indexC) {
          counter++;
        }
        if (counter == 2) {
          DirectedEdge edge;
          edge.nodeIndex = l;
          cv::Point2f point = {(float)polygons[l].centroid.x,
                               (float)polygons[l].centroid.y};
          edge.computeDistance(_graph.nodes[p].point, point);
          _graph.nodes[p].edges.push_back(edge);
        }
      }
    }
  }
  GetStartAndEnd();
  ComputeHeuristics();
}

PathFinder::~PathFinder() {}

std::vector<std::vector<cv::Point2f>> PathFinder::ComputeNPaths(float C,
                                                                int N) {
  std::vector<std::vector<cv::Point2f>> paths;
  for (int n = 1; n <= N; n++) {
    Reset();
    paths.push_back(ComputeSinglePath(n, C));
  }
  return paths;
}

Graph PathFinder::GetGraph() { return _graph; }

float PathFinder::DetermineExplorationRate(cv::Mat frame) {
  return ((frame.cols / 10) + (frame.rows / 10)) / 2;
}

void PathFinder::GetStartAndEnd() {
  for (int n = 1; n < _graph.nodes.size(); n++) {
    if (_graph.nodes[_endNode].point.x < _graph.nodes[n].point.x) {
      _endNode = n;
    }
    if (_graph.nodes[_startNode].point.x > _graph.nodes[n].point.x) {
      _startNode = n;
    }
  }
}

void PathFinder::ComputeHeuristics() {
  for (int n = 0; n < _graph.nodes.size(); n++) {
    cv::Point2f p1 = _graph.nodes[n].point;
    cv::Point2f p2 = _graph.nodes[_endNode].point;
    _graph.nodes[n].distanceToGoal = cv::norm(cv::Mat(p1), cv::Mat(p2));
  }
}

std::vector<cv::Point2f> PathFinder::ComputeSinglePath(int n, float C) {
  std::vector<FringeElement> fringe;

  Node currentNode = _graph.nodes[_startNode];
  for (DirectedEdge &edge : currentNode.edges) {
    FringeElement element;
    element.distance = edge.distance;
    element.edge = edge;
    element.path.push_back(_startNode);
    fringe.push_back(element);
  }

  while (NotFinished(fringe)) {
    int minIndex = 0;
    float minCost = std::numeric_limits<float>::max();
    for (int f = 0; f < fringe.size(); f++) {
      Node potentialNext = _graph.nodes[fringe[f].edge.nodeIndex];
      if (!potentialNext.closed) {
        float cost = CostFunction(potentialNext, fringe[f].distance, n, C);
        if (cost < minCost) {
          minCost = cost;
          minIndex = f;
        }
      }
    }
    for (DirectedEdge &edge :
         _graph.nodes[fringe[minIndex].edge.nodeIndex].edges) {
      FringeElement element;
      element.distance = edge.distance + fringe[minIndex].distance;
      element.edge = edge;
      for (int &index : fringe[minIndex].path) {
        element.path.push_back(index);
      }
      element.path.push_back(fringe[minIndex].edge.nodeIndex);
      fringe.push_back(element);
    }
    _graph.nodes[fringe[minIndex].path.back()].closed = true;
    _graph.nodes[fringe[minIndex].path.back()].timesVisited++;
    fringe.erase(fringe.begin() + minIndex);
  }

  return CreatePath(fringe);
}

float PathFinder::DetermineDirection(std::vector<cv::Point2f> path,
                                     DirectedEdge edge) {
  float distance = edge.distance;
  if (path.size() > 2) {
    if (path[path.size() - 2] == _graph.nodes[edge.nodeIndex].point) {
      distance = distance * (-1);
    }
  }
  return distance;
}

float PathFinder::CostFunction(Node node, float distanceTravelled, int n,
                               float C) {
  return distanceTravelled + node.distanceToGoal + C * (node.timesVisited / n);
}

bool PathFinder::NotFinished(std::vector<FringeElement> fringe) {
  for (FringeElement &element : fringe) {
    if (element.path.back() == _endNode) {
      return false;
    }
  }
  return true;
}

std::vector<cv::Point2f> PathFinder::CreatePath(
    std::vector<FringeElement> fringe) {
  std::vector<int> pathIndices;
  for (FringeElement &element : fringe) {
    if (element.path.back() == _endNode) {
      pathIndices = element.path;
    }
  }
  std::vector<cv::Point2f> path;
  for (int nodeIndex : pathIndices) {
    path.push_back(_graph.nodes[nodeIndex].point);
  }
  return path;
}

void PathFinder::Reset() {
  for (int n = 0; n < _graph.nodes.size(); n++) _graph.nodes[n].closed = false;
}
