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

#ifndef _IMAGE_PROCESSOR_H
#define _IMAGE_PROCESSOR_H

#include <Eigen/Dense>
#include <algorithm>
#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace Eigen;
using namespace std;

/**
 *  Casts CV vectors
 */
template <typename I, typename O>
static vector<O> CastVector(vector<I> inputVector) {
  vector<O> outputVector;

  for (int i = 0; i < inputVector.size(); i++) {
    outputVector.push_back(static_cast<O>(inputVector[i]));
  }

  return outputVector;
}

/**
 * Enum for Marker IDs
 */
enum MarkerCategory {
  Border0 = 0,
  Border1 = 1,
  Start = 2,
  Pause = 3,
  Action = 4,
  Cursor0 = 5,
  Cursor1 = 6,
  Cursor2 = 7,
  Cursor3 = 8,
  Cursor4 = 9
};

/**
 * Aruco marker
 */
struct Marker {
  MarkerCategory category;
  vector<cv::Point2f> corners;
  cv::Vec3d tvec;
  cv::Vec3d rvec;
};

/**
 * Main class for image processing / marker detection
 */
class ImageProcessor {
 public:
  ImageProcessor();
  vector<Marker> DetectMarkers(cv::Mat image);
  cv::Mat GetCamMatrix() {return _camMatrix;}
  cv::Mat GetDistCoeffs();
  cv::Mat DrawMarkers(cv::Mat image, vector<Marker> markers);
  bool ContainsBorderMarkers(vector<Marker> markers);
  cv::Mat WarpPaperImage(cv::Mat image, vector<Marker> markers,
                         int warpedImageWidth, int warpedImageHeight);
  Marker GetMarkerOfCategory(vector<Marker> markers, MarkerCategory category);
  cv::Mat GetHomography();

 private:
  cv::Mat _camMatrix;
  cv::Mat _distCoeffs;
  cv::Ptr<cv::aruco::Dictionary> _dictionary;
  cv::Ptr<cv::aruco::DetectorParameters> _detectorParams;
  float _markerLength;
  vector<cv::Point2f> _paperBorders;
  cv::Mat _homography;

  cv::Mat ChangePerspective(cv::Mat image, vector<cv::Point2f> srcCorners,
                            vector<cv::Point2f> destCorners);
  vector<cv::Point2f> CalcPaperBorders(vector<Marker> markers);
  vector<cv::Point2i> Vertices2ConvexHull(vector<cv::Point2f> vertices);
  cv::Mat CutMarker(cv::Mat image, vector<cv::Point2f> vertices);
};

#endif
