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

#ifndef _EXTERN_H
#define _EXTERN_H

#include "event_bus.hpp"
#include "events.h"
#include "renderer.h"

#include "cam.h"
#include "image_processor.h"


struct CamPosition {
    Vec3d tvec;
    Vec3d rvec;
};

/**
 * Implements path finding from detection library
 * Holds the relative path but delivers the absolute points
 */
class AbsolutePath : public EventHandler<HomographyComputedEvent> {
 public:
  AbsolutePath(std::vector<cv::Point2f> relativePath)
      : _relativePath{relativePath} {
      _registration = EventBus::AddHandler(*this);
  }
  ~AbsolutePath() {
      // remove event handlers
      _registration->RemoveHandler();
  }

  cv::Vec3d GetRotation() {return _m0.rvec;}
  cv::Vec3d GetPoint(int index);
  int Length() {return _relativePath.size(); }

 protected:
  void OnEvent(HomographyComputedEvent& e);

 private:
  EventRegistration* _registration;
  std::vector<cv::Point2f> _relativePath;
  Mat _homography;
  Mat _camMat;
  Mat _distCoeffs;
  Marker _m0;
  Marker _m1;
  bool _homo_computed = false;
};

/**
 * Implements marker / image processing from detection library
 */
class MarkerDetection {
  ImageProcessor _imgproc;

 public:
  MarkerDetection() { _imgproc = ImageProcessor(); }

  Mat GetCamMatrix() { return _imgproc.GetCamMatrix(); }
  Mat GetDistCoeffs() { return _imgproc.GetDistCoeffs(); }

  void Detect(Mat img);
};

#endif
