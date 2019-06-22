#ifndef _MESH_H
#define _MESH_H

#include <math.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

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
  Mesh(vector<Vertex> vertices, vector<Vertex> cornerVertices, int xmax,
       int ymax, int xmin = 0, int ymin = 0);
  virtual ~Mesh();
  void MakeObjFile(string name);
  void CreatePolygonsRelativToIndividualVertices();
  void CreatePolygonsRelativToIndividualVerticesWithRange(int range);
  vector<Vertex> vertices, cornerVertices;
  vector<Polygon> polygons;

 private:
  int xmax, xmin, ymax, ymin, vertexCount;
  int ComputeCentroidClosestToBorder();
  void FindTwoClosestVertices(int index);
  void FindVerticesInRange(int index, int range);
  vector<double> ComputeVertexDistancesTo(int vertex_Index);
  void ComputeCentroids();
  void OrderPolygonsAfterCentroiddistances();
  vector<double> ComputeCentroidDistances(int centroid_Index, vector<Polygon>);
};

#endif