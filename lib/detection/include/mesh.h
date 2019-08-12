/**
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
