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

#include "paths_initializer.h"

std::vector<std::vector<cv::Point2f>> PathsInitializer::InitializePaths() {
  ImageProcessor improc;
  bool houghLines = false;
  bool pathFound = false;
  int houghCounter = 0;

  while (_cam.Grab()) {
    cv::Mat image = _cam.GetFrame();
    vector<Marker> markers = improc.DetectMarkers(image);

    cv::Mat markerImage = improc.DrawMarkers(image, markers);
    imshow("cam feed", markerImage);

    if (improc.ContainsBorderMarkers(markers)) {
      cv::Mat warpedPaperImage =
          improc.WarpPaperImage(image, markers, 900, 600);

      if (!pathFound) {
        if (DetectPaths(warpedPaperImage, houghLines)) {
          pathFound = true;
        } else {
          houghCounter++;
          if (houghCounter == 100) {
            houghLines = true;
            houghCounter == 0;
          }
        }
      }
    }
    if (pathFound) {
      for (auto &marker : markers) {
        if (marker.category == Start) {
          cv::destroyWindow("path");
          cv::destroyWindow("cam feed");
          return _paths;
        }
        if (marker.category == Pause) {
          pathFound = false;
          cv::destroyWindow("path");
          break;
        }
        if (marker.category == DetectionSwitch) {
          pathFound = false;
          cv::destroyWindow("path");
          houghLines = !houghLines;
          break;
        }
      }
    }
  }
  cout << "Cant grab _camera image";
}

bool PathsInitializer::DetectPaths(cv::Mat frame, bool houghLines) {
  double range = 25;
  int maxCorners = 1000;
  double qualityLevel = 0.001;
  int blockSize = 6;
  bool useHarris = true;
  double k = 0.01;

  std::vector<cv::Point2f> pathCorners;
  PathDetector detector;
  if (!houghLines) {
    pathCorners = detector.Cornerdetection(
        frame, range, maxCorners, qualityLevel, blockSize, useHarris, k);
  } else {
    pathCorners = detector.DrawHoughLines(frame);
  }

  if (pathCorners.size() > 2) {
    Mesh mesh(pathCorners);

    PathFinder pf(mesh._polygons, mesh._vertices);
    Graph graph = pf.GetGraph();
    detector.DrawGraph(frame, graph);

    if (pf.ComputeNPaths(10)) {
      _paths.clear();
      std::vector<std::vector<cv::Point2f>> paths = pf.GetPaths();
      for (int p = 0; p < paths.size(); p++) {
        cv::Scalar colour(0, 0, (p + 1) * 25);
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
