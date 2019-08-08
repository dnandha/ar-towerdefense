#include "paths_initializer.h"

std::vector<std::vector<cv::Point2f>> PathsInitializer::InitializePaths() {
  ImageProcessor improc;

  bool pathFound = false;

  while (_cam.Grab()) {
    cv::Mat image = _cam.GetFrame();
    vector<Marker> markers = improc.DetectMarkers(image);

    cv::Mat markerImage = improc.DrawMarkers(image, markers);
    imshow("cam feed", markerImage);

    if (improc.ContainsBorderMarkers(markers)) {
      cv::Mat warpedPaperImage =
          improc.WarpPaperImage(image, markers, 900, 600);

      if (!pathFound) {
        if (DetectPaths(warpedPaperImage)) {
          pathFound = true;
        }
      }
    }
    if (pathFound) {
      for (auto &marker : markers) {
        if (marker.category == Start) {
          return SetPathsInPerspective(improc);
        }
        if (marker.category == Pause) {
          pathFound = false;
          cv::destroyWindow("path");
          break;
        }
      }
    }
  }

  cout << "Cant grab _camera image";
}

bool PathsInitializer::DetectPaths(cv::Mat frame) {
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

    float C = pf.DetermineExplorationRate(frame);
    if (pf.ComputeNPaths(10 * C, 10)) {
      _paths.clear();
      std::vector<std::vector<cv::Point2f>> paths = pf.GetPaths();
      for (int p = 0; p < paths.size(); p++) {
        cv::Scalar colour((p + 1) * 25, 0, 0);
        detector.DrawPath(frame, paths[p], colour);
        cv::namedWindow("path", 1);
        cv::imshow("path", frame);
        _paths.push_back(paths[p]);
      }
      return true;
    }
  }
  return false;
}

std::vector<std::vector<cv::Point2f>> PathsInitializer::SetPathsInPerspective(
    ImageProcessor improc) {
  std::vector<std::vector<cv::Point2f>> pathsInPerspective;
  for (auto &path : _paths) {
    std::vector<cv::Point2f> pointsInPerspective;
    for (auto &point : path) {
      pointsInPerspective.push_back(improc.SetInPerspective(point));
    }
    pathsInPerspective.push_back(pointsInPerspective);
  }
  return pathsInPerspective;
}