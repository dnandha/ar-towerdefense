#pragma once
#include <math.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "delaunator.hpp"

struct Vertex {
  double x, y, z;
};
struct Polygon {
  int indexA, indexB, indexC;
  Vertex centroid;
  void ComputeCentroid(Vertex vertexA, Vertex vertexB, Vertex vertexC) {
    this->centroid = {(vertexA.x + vertexB.x + vertexC.x) / 3,
                      (vertexA.y + vertexB.y + vertexC.y) / 3,
                      (vertexA.z + vertexB.z + vertexC.z) / 3};
  }
};

class Mesh {
 public:
  Mesh(std::vector<cv::Point2f> points);
  virtual ~Mesh();
  void MakeObjFile(std::string name);
  void CreatePolygonsRelativToIndividualVertices();
  void CreatePolygonsRelativToIndividualVerticesWithRange(double range);
  void SortVertices();
  void WriteVertTxt();
  std::vector<Vertex> _vertices;
  std::vector<Polygon> _polygons;

 private:
  int xmax, xmin, ymax, ymin, vertexCount;
  int ComputeVertexClosestToBorder();
  int ComputeCentroidClosestToBorder();
  void CreatePolygonWithTwoClosestVertices(int index);
  void CreatePolygonsWithVerticesInRange(int index, double range);
  std::vector<double> ComputeVertexDistancesTo(int vertex_Index);
  void ComputeCentroids();
  void OrderPolygonsAfterCentroiddistances();
  std::vector<double> ComputeCentroidDistances(int centroid_Index,
                                               std::vector<Polygon>);
  bool contains(Polygon polygon);
  bool tooLong(Polygon polygon);
};
