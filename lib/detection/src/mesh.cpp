#include "mesh.h"

Mesh::Mesh(std::vector<cv::Point2f> points) {
  std::vector<double> coords;
  for (int c = 0; c < points.size(); c++) {
    _vertices.push_back({points[c].x, points[c].y, 0.0});
    coords.push_back(points[c].x);
    coords.push_back(points[c].y);
  }
  delaunator::Delaunator d(coords);

  for (size_t t = 0; t < d.triangles.size(); t += 3) {
    Polygon polygon = {d.triangles[t], d.triangles[t + 1], d.triangles[t + 2]};
    if (!tooLong(polygon)) {
      _polygons.push_back({(int)d.triangles[t], (int)d.triangles[t + 1],
                           (int)d.triangles[t + 2]});
      _polygons.back().ComputeCentroid(_vertices[_polygons.back().indexA],
                                       _vertices[_polygons.back().indexB],
                                       _vertices[_polygons.back().indexC]);
    }
  }
}

Mesh::~Mesh() {}

void Mesh::MakeObjFile(std::string name) {
  std::ofstream objFile;
  objFile.open(name);

  for (int v = 0; v < _vertices.size(); v++)
    objFile << "v " << _vertices[v].x << " " << _vertices[v].z << " "
            << _vertices[v].y << "\n";
  for (int p = 0; p < _polygons.size(); p++)
    objFile << "f " << _polygons[p].indexA + 1 << " " << _polygons[p].indexB + 1
            << " " << _polygons[p].indexC + 1 << "\n";

  objFile.close();
}

void Mesh::CreatePolygonsRelativToIndividualVertices() {
  for (int v = 0; v < _vertices.size(); v++) {
    CreatePolygonWithTwoClosestVertices(v);
  }

  ComputeCentroids();
  OrderPolygonsAfterCentroiddistances();
}

void Mesh::CreatePolygonsRelativToIndividualVerticesWithRange(double range) {
  for (int v = 0; v < _vertices.size(); v++) {
    CreatePolygonsWithVerticesInRange(v, range);
  }
  ComputeCentroids();
  OrderPolygonsAfterCentroiddistances();
}

void Mesh::SortVertices() {
  int vertClosestToBorder = ComputeVertexClosestToBorder();

  std::vector<double> distances = ComputeVertexDistancesTo(vertClosestToBorder);
  int min_index = vertClosestToBorder;

  std::swap(distances[0], distances[min_index]);
  std::swap(_vertices[0], _vertices[min_index]);

  for (int v = 1; v < _vertices.size(); v++) {
    min_index =
        min_element(distances.begin() + v, distances.end()) - distances.begin();

    std::swap(distances[v], distances[min_index]);
    std::swap(_vertices[v], _vertices[min_index]);
  }
}

void Mesh::WriteVertTxt() {
  std::ofstream objFile;
  objFile.open("Vertices.txt");
  for (int v = 0; v < _vertices.size(); v++)
    objFile << _vertices[v].x << "," << _vertices[v].y << "\n";
  objFile.close();
}

int Mesh::ComputeVertexClosestToBorder() {
  int vertexWithShortestDistance_Index_X = 0;
  float minDistanceToBorder_X = ((int)xmax / 2) - _vertices[0].x;

  int vertexWithShortestDistance_Index_Y = 0;
  float minDistanceToBorder_Y = ((int)ymax / 2) - _vertices[0].y;

  float distance_X;
  float distance_Y;
  for (int v = 1; v < _vertices.size(); v++) {
    distance_X = ((int)xmax / 2) - _vertices[v].x;
    if (distance_X < minDistanceToBorder_X) {
      vertexWithShortestDistance_Index_X = v;
      minDistanceToBorder_X = distance_X;
    }

    distance_Y = ((int)ymax / 2) - _vertices[v].y;
    if (distance_Y < minDistanceToBorder_Y) {
      vertexWithShortestDistance_Index_Y = v;
      minDistanceToBorder_Y = distance_Y;
    }
  }

  return minDistanceToBorder_Y < minDistanceToBorder_X
             ? vertexWithShortestDistance_Index_Y
             : vertexWithShortestDistance_Index_X;
}

int Mesh::ComputeCentroidClosestToBorder() {
  int vertexWithShortestDistance_Index_X = 0;
  float minDistanceToBorder_X = ((int)xmax / 2) - _polygons[0].centroid.x;

  int vertexWithShortestDistance_Index_Y = 0;
  float minDistanceToBorder_Y = ((int)ymax / 2) - _polygons[0].centroid.y;

  float distance_X;
  float distance_Y;
  for (int p = 1; p < _polygons.size(); p++) {
    distance_X = ((int)xmax / 2) - _polygons[p].centroid.x;
    if (distance_X < minDistanceToBorder_X) {
      vertexWithShortestDistance_Index_X = p;
      minDistanceToBorder_X = distance_X;
    }

    distance_Y = ((int)ymax / 2) - _polygons[p].centroid.y;
    if (distance_Y < minDistanceToBorder_Y) {
      vertexWithShortestDistance_Index_Y = p;
      minDistanceToBorder_Y = distance_Y;
    }
  }

  return minDistanceToBorder_Y < minDistanceToBorder_X
             ? vertexWithShortestDistance_Index_Y
             : vertexWithShortestDistance_Index_X;
}

void Mesh::CreatePolygonWithTwoClosestVertices(int index) {
  std::vector<double> distances = ComputeVertexDistancesTo(index);
  int min_index;
  Polygon new_polygon;
  for (int v = 0; v <= 2; v++) {
    min_index =
        min_element(distances.begin() + v, distances.end()) - distances.begin();
    std::swap(distances[v], distances[min_index]);

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
  _polygons.push_back(new_polygon);
}

void Mesh::CreatePolygonsWithVerticesInRange(int index, double range) {
  std::vector<double> distances = ComputeVertexDistancesTo(index);
  int min_index = index;
  std::swap(distances[0], distances[min_index]);

  int indexB = -1;
  int indexC = -1;
  Polygon new_polygon;
  for (int v = 1; v < _vertices.size(); v++) {
    min_index =
        min_element(distances.begin() + v, distances.end()) - distances.begin();
    if (distances[min_index] > range * 2) break;
    std::swap(distances[v], distances[min_index]);

    if (indexB == -1) {
      indexB = min_index;
    } else if (indexC == -1) {
      indexC = min_index;

      new_polygon.indexA = index;
      new_polygon.indexB = indexB;
      new_polygon.indexC = indexC;
      if (!contains(new_polygon)) _polygons.push_back(new_polygon);

      indexB = -1;
      indexC = -1;
    }
  }
}

std::vector<double> Mesh::ComputeVertexDistancesTo(int vertex_Index) {
  double x;
  double y;
  double dist;
  std::vector<double> distances;

  for (int v = 0; v < _vertices.size(); v++) {
    if (v != vertex_Index) {
      x = _vertices[vertex_Index].x -
          _vertices[v].x;  // calculating number to square in next step
      y = _vertices[vertex_Index].y - _vertices[v].y;

      dist = pow(x, 2) + pow(y, 2);  // calculating Euclidean distance
      dist = sqrt(dist);

      distances.push_back(dist);
    } else {
      distances.push_back(0);
    }
  }

  if (vertex_Index == 0) {
    std::ofstream objFile;
    objFile.open("distances.txt");

    for (int d = 0; d < distances.size(); d++) {
      objFile << d << " : " << distances[d] << "\n";
    }

    objFile.close();
  }

  return distances;
}

void Mesh::ComputeCentroids() {
  for (int p = 0; p < _polygons.size(); p++) {
    _polygons[p].centroid = {
        (_vertices[_polygons[p].indexA].x + _vertices[_polygons[p].indexB].x +
         _vertices[_polygons[p].indexC].x) /
            3,
        (_vertices[_polygons[p].indexA].y + _vertices[_polygons[p].indexB].y +
         _vertices[_polygons[p].indexC].y) /
            3,
        0.0};
  }
}

void Mesh::OrderPolygonsAfterCentroiddistances() {
  int index = ComputeCentroidClosestToBorder();
  int min_index;

  std::swap(_polygons[0], _polygons[index]);

  for (int p = 1; p < _polygons.size(); p++) {
    std::vector<Polygon> polygonClone(_polygons.begin() + p, _polygons.end());
    std::vector<double> distances =
        ComputeCentroidDistances(p - 1, polygonClone);

    min_index =
        min_element(distances.begin(), distances.end()) - distances.begin();
    std::swap(_polygons[p], _polygons[min_index + p]);
  }
}
std::vector<double> Mesh::ComputeCentroidDistances(
    int centroid_Index, std::vector<Polygon> polygonClone) {
  double x;
  double y;
  double dist;
  std::vector<double> distances;

  for (int p = 0; p < polygonClone.size(); p++) {
    x = _polygons[centroid_Index].centroid.x -
        polygonClone[p]
            .centroid.x;  // calculating number to square in next step
    y = _polygons[centroid_Index].centroid.y - polygonClone[p].centroid.y;

    dist = pow(x, 2) + pow(y, 2);  // calculating Euclidean distance
    dist = sqrt(dist);

    distances.push_back(dist);
  }

  return distances;
}

bool Mesh::contains(Polygon polygon) {
  for (int p = 0; p < _polygons.size(); p++) {
    if (_polygons[p].indexA == polygon.indexA &&
        _polygons[p].indexB == polygon.indexB &&
        _polygons[p].indexC == polygon.indexC)
      return true;
  }
  return false;
}

bool Mesh::tooLong(Polygon polygon) {
  double range = 100;
  double x0, x1, x2, x01, x12, x20;
  double y0, y1, y2, y01, y12, y20;
  double dist01, dist12, dist20;
  x0 = _vertices[polygon.indexA].x;
  x1 = _vertices[polygon.indexB].x;
  x2 = _vertices[polygon.indexC].x;

  y0 = _vertices[polygon.indexA].y;
  y1 = _vertices[polygon.indexB].y;
  y2 = _vertices[polygon.indexC].y;

  x01 = x0 - x1;
  y01 = y0 - y1;
  dist01 = pow(x01, 2) + pow(y01, 2);
  dist01 = sqrt(dist01);
  if (dist01 > range) {
    return true;
  }

  x12 = x1 - x2;
  y12 = y1 - y2;
  dist12 = pow(x12, 2) + pow(y12, 2);
  dist12 = sqrt(dist12);
  if (dist12 > range) {
    return true;
  }

  x20 = x2 - x0;
  y20 = y2 - y0;
  dist20 = pow(x20, 2) + pow(y20, 2);
  dist20 = sqrt(dist20);
  if (dist20 > range) {
    return true;
  }
  return false;
}
