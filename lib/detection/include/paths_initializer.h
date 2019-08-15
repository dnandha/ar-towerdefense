/*
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
#include "cam.h"
#include "image_processor.h"
#include "mesh.h"
#include "path_detector.h"

/**
 * Recognition of path / line
 */
class PathsInitializer {
 public:
  PathsInitializer(Cam cam) : _cam{cam} {}
  std::vector<std::vector<cv::Point2f>> InitializePaths();

 private:
  Cam _cam;
  std::vector<std::vector<cv::Point2f>> _paths;
  bool DetectPaths(cv::Mat frame, bool houghLines);
};
