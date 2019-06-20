#include "mesh.h"

using namespace cv;
using namespace std;

Mesh::Mesh(vector<Vertex> vertices, vector<Vertex> cornerVertices, int xmax,
           int ymax, int xmin, int ymin) {
  this->xmax = xmax;
  this->xmin = xmin;
  this->ymax = ymax;
  this->ymin = ymin;
  this->vertices = vertices;
  this->cornerVertices = cornerVertices;
}

Mesh::~Mesh() {}

void Mesh::MakeObjFile(string name) {
  ofstream objFile;
  objFile.open(name);

  int offset = 0;

  for (int v = 0; v < cornerVertices.size(); v++) {
    objFile << "v " << cornerVertices[v].x << " " << cornerVertices[v].z << " "
            << cornerVertices[v].y << " "
            << "\n";
    offset++;
  }
  for (int v = 0; v < vertices.size(); v++) {
    objFile << "v " << vertices[v].x << " " << vertices[v].z << " "
            << vertices[v].y << " "
            << "\n";
  }

  int alternate = 0;

  for (int p = 1; p < cornerVertices.size() - 1; p++) {
    if (alternate == 1) {
      objFile << "f " << p << " " << p + 1 << " " << p + 2 << " "
              << "\n";
      alternate = 0;
    } else if (alternate == 0) {
      objFile << "f " << p + 2 << " " << p + 1 << " " << p << " "
              << "\n";
      alternate = 1;
    }
  }
  alternate = 0;
  for (int p = 0; p < polygons.size(); p++) {
    if (alternate == 1) {
      objFile << "f " << polygons[p].indexA + offset << " "
              << polygons[p].indexB + offset << " "
              << polygons[p].indexC + offset << " "
              << "\n";
      alternate = 0;
    } else if (alternate == 0) {
      objFile << "f " << polygons[p].indexC + offset << " "
              << polygons[p].indexB + offset << " "
              << polygons[p].indexA + offset << " "
              << "\n";
      alternate = 1;
    }
  }

  objFile.close();
}

void Mesh::CreatePolygonsRelativToIndividualVertices() {
  for (int v = 0; v < vertices.size(); v++) {
    FindTwoClosestVertices(v);
  }

  ComputeCentroids();
  OrderPolygonsAfterCentroiddistances();
}

int Mesh::ComputeCentroidClosestToBorder() {
  int vertexWithShortestDistance_Index_X = 0;
  float minDistanceToBorder_X = ((int)xmax / 2) - polygons[0].centroid.x;

  int vertexWithShortestDistance_Index_Y = 0;
  float minDistanceToBorder_Y = ((int)ymax / 2) - polygons[0].centroid.y;

  float distance_X;
  float distance_Y;
  for (int p = 1; p < polygons.size(); p++) {
    distance_X = ((int)xmax / 2) - polygons[p].centroid.x;
    if (distance_X < minDistanceToBorder_X) {
      vertexWithShortestDistance_Index_X = p;
      minDistanceToBorder_X = distance_X;
    }

    distance_Y = ((int)ymax / 2) - polygons[p].centroid.y;
    if (distance_Y < minDistanceToBorder_Y) {
      vertexWithShortestDistance_Index_Y = p;
      minDistanceToBorder_Y = distance_Y;
    }
  }

  return minDistanceToBorder_Y < minDistanceToBorder_X
             ? vertexWithShortestDistance_Index_Y
             : vertexWithShortestDistance_Index_X;
}

void Mesh::FindTwoClosestVertices(int index) {
  vector<double> distances = ComputeVertexDistancesTo(index);
  int min_index;
  Polygon new_polygon;
  for (int v = 0; v <= 2; v++) {
    min_index =
        min_element(distances.begin() + v, distances.end()) - distances.begin();
    swap(distances[v], distances[min_index]);

    switch (v) {
      case 0:
        new_polygon.indexA = min_index;
        break;
      case 1:
        new_polygon.indexB = min_index;
        break;
      case 2:
        new_polygon.indexC = min_index;
        break;
    }
  }
  polygons.push_back(new_polygon);
}

vector<double> Mesh::ComputeVertexDistancesTo(int vertex_Index) {
  double x;
  double y;
  double dist;
  vector<double> distances;

  for (int v = 0; v < vertices.size(); v++) {
    if (v != vertex_Index) {
      x = vertices[vertex_Index].x -
          vertices[v].x;  // calculating number to square in next step
      y = vertices[vertex_Index].y - vertices[v].y;

      dist = pow(x, 2) + pow(y, 2);  // calculating Euclidean distance
      dist = sqrt(dist);

      distances.push_back(dist);
    } else {
      distances.push_back(0);
    }
  }

  if (vertex_Index == 0) {
    ofstream objFile;
    objFile.open("distances.txt");

    for (int d = 0; d < distances.size(); d++) {
      objFile << d << " : " << distances[d] << "\n";
    }

    objFile.close();
  }

  return distances;
}

void Mesh::ComputeCentroids() {
  for (int p = 0; p < polygons.size(); p++) {
    polygons[p].centroid = {
        (vertices[polygons[p].indexA].x + vertices[polygons[p].indexB].x +
         vertices[polygons[p].indexC].x) /
            3,
        (vertices[polygons[p].indexA].y + vertices[polygons[p].indexB].y +
         vertices[polygons[p].indexC].y) /
            3,
        0.0};
  }
}

void Mesh::OrderPolygonsAfterCentroiddistances() {
  int index = ComputeCentroidClosestToBorder();
  waitKey(0);
  int min_index;

  swap(polygons[0], polygons[index]);

  for (int p = 1; p < polygons.size(); p++) {
    vector<Polygon> polygonClone(polygons.begin() + p, polygons.end());
    vector<double> distances = ComputeCentroidDistances(p - 1, polygonClone);

    min_index =
        min_element(distances.begin(), distances.end()) - distances.begin();
    swap(polygons[p], polygons[min_index + p]);
  }
}
vector<double> Mesh::ComputeCentroidDistances(int centroid_Index,
                                              vector<Polygon> polygonClone) {
  double x;
  double y;
  double dist;
  vector<double> distances;

  for (int p = 0; p < polygonClone.size(); p++) {
    x = polygons[centroid_Index].centroid.x -
        polygonClone[p]
            .centroid.x;  // calculating number to square in next step
    y = polygons[centroid_Index].centroid.y - polygonClone[p].centroid.y;

    dist = pow(x, 2) + pow(y, 2);  // calculating Euclidean distance
    dist = sqrt(dist);

    distances.push_back(dist);
  }

  return distances;
}
