#include "path_finder.h"

PathFinder::PathFinder(std::vector<Polygon> polygons,
                       std::vector<Vertex> vertices) {
  for (int p = 0; p < polygons.size(); p++) {
    GNode node;
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

/*
 * Compute N paths on graph. Returns false on failure.
 */
bool PathFinder::ComputeNPaths(int N) {
  _paths.clear();
  for (int n = 1; n <= N; n++) {
    Reset();
    if (!ComputeSinglePath(n)) {
      return false;
    }
  }
  return true;
}

Graph PathFinder::GetGraph() { return _graph; }
std::vector<std::vector<cv::Point2f>> PathFinder::GetPaths() { return _paths; }

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

bool PathFinder::ComputeSinglePath(int n) {
  std::vector<FringeElement> fringe;

  GNode currentNode = _graph.nodes[_startNode];
  for (DirectedEdge &edge : currentNode.edges) {
    FringeElement element;
    element.distance = edge.distance;
    element.edge = edge;
    element.path.push_back(_startNode);
    fringe.push_back(element);
  }

  while (NotFinished(fringe)) {
    if (fringe.size() == 0) {
      return false;
    }

    int minIndex = 0;
    float minCost = std::numeric_limits<float>::max();
    int closedCounter = 0;
    for (int f = 0; f < fringe.size(); f++) {
      GNode potentialNext = _graph.nodes[fringe[f].edge.nodeIndex];
      if (!potentialNext.closed) {
        float cost = CostFunction(potentialNext, fringe[f].distance, n);
        if (cost < minCost) {
          minCost = cost;
          minIndex = f;
        }
      } else {
        closedCounter++;
        // check if all elements in fringe are closed
        if (closedCounter >= fringe.size()) {
          return false;
        }
      }
    }
    for (DirectedEdge &edge :
         _graph.nodes[fringe[minIndex].edge.nodeIndex].edges) {
      FringeElement element;
      element.distance =
          edge.distance +
          fringe[minIndex].distance *
              (_graph.nodes[fringe[minIndex].edge.nodeIndex].timesVisited / n);
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
  _paths.push_back(CreatePath(fringe));
  return true;
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

float PathFinder::CostFunction(GNode node, float distanceTravelled, int n) {
  return (distanceTravelled + node.distanceToGoal) * (node.timesVisited / n);
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
