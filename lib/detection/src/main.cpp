#include "cam.h"
#include "image_processor.h"
#include "mesh.h"
#include "path_detector.h"

static void detectPath(Mat frame) {
  vector<Point2f> pathCorners;

  // get necessary data form image
  PathDetector detector;

  Mat grayscaleImage = detector.RangeThresholdBinary(frame);

  imshow("Grayscale image", grayscaleImage);

  pathCorners = detector.Cornerdetection(grayscaleImage);

  // preparing mesh input
  Size size = detector.FrameSize();
  vector<Vertex> vertices;
  vector<Vertex> cornerVertices;
  cornerVertices.push_back({(double)size.width, (double)size.height, 0.0});
  cornerVertices.push_back({(double)size.width, 0.0, 0.0});
  cornerVertices.push_back({0.0, (double)size.height, 0.0});
  cornerVertices.push_back({0.0, 0.0, 0.0});
  int c;
  for (c = 0; c < pathCorners.size(); c++) {
    vertices.push_back({pathCorners[c].x, pathCorners[c].y, 0.0});
  }

  // create Mesh
  Mesh mesh(vertices, cornerVertices, size.width, size.height);

  // adjust mesh data and create .obj-file
  mesh.polygons.clear();
  mesh.CreatePolygonsRelativToIndividualVertices();
  // mesh.MakeObjFile(imagename + "_Object.obj");

  // display mesh data on image
  Mat pathImage = detector.PathMeshInImg(frame, mesh);
  imshow("Path image", pathImage);

  Mat centroidImage = detector.PathCentroids(frame, mesh);
  imshow("Centroid image", centroidImage);
}

/*
 * Main
 */
int main() {
  Cam cam(1);
  ImageProcessor improc;

  while (cam.Grab()) {
    Mat image = cam.GetFrame();
    vector<Marker> markers = improc.DetectMarkers(image);

    Mat markerImage = improc.DrawMarkers(image, markers);
    imshow("marker image", markerImage);

    if (improc.ContainsBorderMarkers(markers)) {
      Mat warpedPaperImage = improc.WarpPaperImage(image, markers, 900, 600);
      imshow("warped image", warpedPaperImage);

      detectPath(warpedPaperImage);
    }
  }

  std::cout << "Cant grab camera image";

  return 1;
}