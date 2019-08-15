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

#include "image_processor.h"

struct SortY {
  bool operator()(cv::Point pt1, cv::Point pt2) { return (pt1.y < pt2.y); }
} mySortY;

struct SortX {
  bool operator()(cv::Point pt1, cv::Point pt2) { return (pt1.x < pt2.x); }
} mySortX;

ImageProcessor::ImageProcessor() {
  // calibration results
  _camMatrix = (cv::Mat_<double>(3, 3) << 9.5688579936159397e+02, 0.,
                6.5204540592076330e+02, 0., 9.5245103032304894e+02,
                3.6538198893031625e+02, 0., 0., 1.);
  _distCoeffs = (cv::Mat_<double>(5, 1) << -5.8248455454749333e-02,
                 1.5857181599691350e-02, -7.2418305586809265e-03,
                 5.7471593045364724e-03, 0.);

  // dictionary: DICT_4X4_50=0, DICT_4X4_100=1, DICT_4X4_250=2,"
  // "DICT_4X4_1000=3, DICT_5X5_50=4, DICT_5X5_100=5, DICT_5X5_250=6,
  // DICT_5X5_1000=7, " "DICT_6X6_50=8, DICT_6X6_100=9, DICT_6X6_250=10,
  // DICT_6X6_1000=11, DICT_7X7_50=12," "DICT_7X7_100=13, DICT_7X7_250=14,
  // DICT_7X7_1000=15, DICT_cv::aruco_ORIGINAL = 16," "DICT_APRILTAG_16h5=17,
  // DICT_APRILTAG_25h9=18, DICT_APRILTAG_36h10=19, DICT_APRILTAG_36h11=20}
  _dictionary = cv::aruco::getPredefinedDictionary(
      cv::aruco::PREDEFINED_DICTIONARY_NAME(0));

  // using standard detector parametes
  _detectorParams = cv::aruco::DetectorParameters::create();
  // Corner refinement method (0: None, 1: Subpixel, 2:contour, 3: AprilTag 2)
  _detectorParams->cornerRefinementMethod = 2;

  _markerLength = 2.6;
}

/*
 * Returns distortion coefficients cv::Matrix
 */
cv::Mat ImageProcessor::GetDistCoeffs() { return _distCoeffs; }

/*
 * Detects marker categories, corners, and poses
 */
vector<Marker> ImageProcessor::DetectMarkers(cv::Mat image) {
  // marker ID's
  vector<int> ids;
  // lists of corners of detected markers (clockwise order)
  vector<vector<cv::Point2f>> corners, rejected;
  // rotation and translation vectors respectively for each of the markers in
  // corners
  vector<cv::Vec3d> rvecs, tvecs;

  vector<Marker> markers;

  // detect markers and esticv::Mate pose
  cv::aruco::detectMarkers(image, _dictionary, corners, ids, _detectorParams,
                           rejected);

  if (ids.size() > 0) {
    cv::aruco::estimatePoseSingleMarkers(corners, _markerLength, _camMatrix,
                                         _distCoeffs, rvecs, tvecs);

    for (int i = 0; i < ids.size(); i++) {
      Marker marker;
      marker.category = static_cast<MarkerCategory>(ids[i]);
      marker.corners = corners[i];
      marker.tvec = tvecs[i];
      marker.rvec = rvecs[i];

      markers.push_back(marker);
    }
  }

  return markers;
}

/*
 * Draws marker edges and poses on image
 */
cv::Mat ImageProcessor::DrawMarkers(cv::Mat image, vector<Marker> markers) {
  if (markers.size() > 0) {
    vector<int> ids;
    vector<vector<cv::Point2f>> corners;
    vector<cv::Vec3d> rvecs, tvecs;

    for (int i = 0; i < markers.size(); i++) {
      ids.push_back((int)markers[i].category);
      corners.push_back(markers[i].corners);
      rvecs.push_back(markers[i].rvec);
      tvecs.push_back(markers[i].tvec);
    }

    cv::Mat imageCopy;
    image.copyTo(imageCopy);

    cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);

    for (unsigned int i = 0; i < ids.size(); i++)
      cv::aruco::drawAxis(imageCopy, _camMatrix, _distCoeffs, rvecs[i],
                          tvecs[i], _markerLength * 0.5f);
    return imageCopy;
  }
  return image;
}

/*
 * Checks if vector contains the markers to determine paper borders
 */
bool ImageProcessor::ContainsBorderMarkers(vector<Marker> markers) {
  if (markers.size() == 0) {
    return false;
  }

  bool marker0found = false;
  bool marker1found = false;

  for (auto const &marker : markers) {
    if (marker.category == Border0) {
      marker0found = true;
    }
    if (marker.category == Border1) {
      marker1found = true;
    }
  }

  if (marker0found && marker1found) {
    return true;
  }
  return false;
}

cv::Mat ImageProcessor::GetHomography() { return _homography; }

/*
 * Removes perspective from with paper as ROI of the image
 */
cv::Mat ImageProcessor::WarpPaperImage(cv::Mat image, vector<Marker> markers,
                                       int warpedImageWidth,
                                       int warpedImageHeight) {
  cv::Mat imageCopy;
  image.copyTo(imageCopy);

  Marker marker0 = GetMarkerOfCategory(markers, Border0);
  Marker marker1 = GetMarkerOfCategory(markers, Border1);

  _paperBorders = CalcPaperBorders(markers);

  vector<cv::Point2f> warpedPaperBorders;
  warpedPaperBorders.push_back(cv::Point2i(0, 0));
  warpedPaperBorders.push_back(cv::Point2i(warpedImageWidth, 0));
  warpedPaperBorders.push_back(cv::Point2i(0, warpedImageHeight));
  warpedPaperBorders.push_back(
      cv::Point2i(warpedImageWidth, warpedImageHeight));

  // cut markers out of image
  cv::Mat cuttedMarker0Image = CutMarker(imageCopy, marker0.corners);
  cv::Mat cuttedMarker1Image = CutMarker(cuttedMarker0Image, marker1.corners);

  return ChangePerspective(cuttedMarker1Image, _paperBorders,
                           warpedPaperBorders);
}

/*
 *
 */
cv::Mat ImageProcessor::ChangePerspective(cv::Mat image,
                                          vector<cv::Point2f> srcCorners,
                                          vector<cv::Point2f> destCorners) {
  _homography = getPerspectiveTransform(srcCorners, destCorners);
  cv::Mat warpedImage;
  float width = destCorners.back().x;
  float height = destCorners.back().y;
  warpPerspective(image, warpedImage, _homography, cv::Size(width, height));

  return warpedImage;
}

/*
 * Calcuates paper borders for markers with border categories
 */
vector<cv::Point2f> ImageProcessor::CalcPaperBorders(vector<Marker> markers) {
  if (markers.size() == 0 || !ContainsBorderMarkers(markers)) {
    throw 20;
  }

  Marker marker0 = GetMarkerOfCategory(markers, Border0);
  Marker marker1 = GetMarkerOfCategory(markers, Border1);
  vector<cv::Point2f> m0c = marker0.corners;
  vector<cv::Point2f> m1c = marker1.corners;

  typedef Eigen::Hyperplane<float, 2> Line2;  // Hyperplane in 2d is a line
  typedef Eigen::Vector2f Vec2;

  // Collect points of interrest
  Vec2 m0c0(m0c[0].x, m0c[0].y);
  Vec2 m0c1(m0c[1].x, m0c[1].y);
  Vec2 m0c3(m0c[3].x, m0c[3].y);
  Vec2 m1c0(m1c[0].x, m1c[0].y);
  Vec2 m1c1(m1c[1].x, m1c[1].y);
  Vec2 m1c3(m1c[3].x, m1c[3].y);

  // Calcuate lines
  Line2 m0c0c1 = Line2::Through(m0c0, m0c1);
  Line2 m0c0c3 = Line2::Through(m0c0, m0c3);
  Line2 m1c0c1 = Line2::Through(m1c0, m1c1);
  Line2 m1c0c3 = Line2::Through(m1c0, m1c3);

  // Calucate intersections of lines
  Vec2 intersection1 = m0c0c1.intersection(m1c0c3);
  Vec2 intersection2 = m0c0c3.intersection(m1c0c1);

  cv::Point2f intersecPoint1(intersection1[0], intersection1[1]);
  cv::Point2f intersecPoint2(intersection2[0], intersection2[1]);

  vector<cv::Point2f> paperBorders;
  paperBorders.push_back(m0c[0]);
  paperBorders.push_back(intersecPoint1);
  paperBorders.push_back(m1c[0]);
  paperBorders.push_back(intersecPoint2);

  vector<cv::Point2f> orgPaperBorders =
      CastVector<cv::Point2i, cv::Point2f>(Vertices2ConvexHull(paperBorders));

  // sorting paper borders
  sort(orgPaperBorders.begin(), orgPaperBorders.end(), mySortY);
  sort(orgPaperBorders.begin(), orgPaperBorders.begin() + 2, mySortX);
  sort(orgPaperBorders.begin() + 2, orgPaperBorders.end(), mySortX);

  return orgPaperBorders;
}

/*
 * Get convex hull for vertices
 */
vector<cv::Point2i> ImageProcessor::Vertices2ConvexHull(
    vector<cv::Point2f> vertices) {
  // Cast cv::Point2f to cv::Point2i for fillConvexPoly function
  vector<cv::Point2i> vertices_i =
      CastVector<cv::Point2f, cv::Point2i>(vertices);

  vector<cv::Point2i> hull;

  convexHull(vertices_i, hull);

  return hull;
}

/*
 * Draws white rectangle over marker
 */
cv::Mat ImageProcessor::CutMarker(cv::Mat image, vector<cv::Point2f> vertices) {
  vector<cv::Point2i> hull = Vertices2ConvexHull(vertices);

  // Get bounding rect of hull
  cv::Rect bounding = boundingRect(hull);
  int upscale = 10;

  // Scale bounding rect up to prevent balck lines on the edges
  cv::Rect boundingScaled(bounding.x - upscale / 2, bounding.y - upscale / 2,
                          bounding.width + upscale, bounding.height + upscale);

  cv::Point p1 = cv::Point(boundingScaled.x, boundingScaled.y);
  cv::Point p2 = cv::Point(boundingScaled.x + boundingScaled.width,
                           boundingScaled.y + boundingScaled.height);

  // Draw white filled rect over marker
  rectangle(image, p1, p2, cv::Scalar(255, 255, 255), -1);

  return image;
}

/*
 * Returns marker with given category from vector
 */
Marker ImageProcessor::GetMarkerOfCategory(vector<Marker> markers,
                                           MarkerCategory category) {
  for (std::size_t i = 0; i != markers.size(); i++) {
    if (markers[i].category == category) {
      return markers[i];
    }
  }
  throw out_of_range("Marker with id:" + to_string((int)category) +
                     "is not in collection.");
}
