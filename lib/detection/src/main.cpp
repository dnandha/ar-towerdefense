#include "cam.h"
#include "image_processor.h"
#include "mesh.h"
#include "path_detector.h"

void detectPath(cv::Mat frame) {
  double range = 25;
  int maxCorners = 1000;
  double qualityLevel = 0.001;
  int blockSize = 6;
  bool useHarris = true;
  double k = 0.01;

  std::vector<cv::Point2f> pathCorners;
  PathDetector detector;
  pathCorners = detector.Cornerdetection(frame, range, maxCorners, qualityLevel,
                                         blockSize, useHarris, k);

  if (pathCorners.size() > 2) {
    Mesh mesh(pathCorners);

    PathFinder pf(mesh._polygons, mesh._vertices);
    Graph graph = pf.GetGraph();
    detector.DrawGraph(frame, graph);
    cv::imshow("graph", frame);

    float C = pf.DetermineExplorationRate(frame);
    std::cout << C;
    std::vector<std::vector<cv::Point2f>> paths = pf.ComputeNPaths(C, 10);
    for (int p = 0; p < paths.size(); p++) {
      cv::Scalar colour((p + 1) * 25, 0, 0);
      detector.DrawPath(frame, paths[p], colour);
      cv::imshow("path", frame);
    }
  }
}

/*
 * Main
 */
int main() {
  Cam cam(1);
  ImageProcessor improc;

  while (cam.Grab()) {
    cv::Mat image = cam.GetFrame();
    vector<Marker> markers = improc.DetectMarkers(image);

    cv::Mat markerImage = improc.DrawMarkers(image, markers);
    imshow("marker image", markerImage);

    if (improc.ContainsBorderMarkers(markers)) {
      cv::Mat warpedPaperImage =
          improc.WarpPaperImage(image, markers, 900, 600);
      imshow("warped image", warpedPaperImage);

      cv::Point2f testPoint(300, 300);
      cv::Point2f imagePoint = improc.SetInPerspective(testPoint);

      detectPath(warpedPaperImage);
    }
  }

  std::cout << "Cant grab camera image";

  return 1;
}