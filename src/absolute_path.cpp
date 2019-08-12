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

#include "extern.h"

cv::Vec3d AbsolutePath::GetPoint(int index) {
  if (!_homo_computed)
    return Vec3d(0.0, 0.0, 0.0);

  cv::Point2f point = _relativePath[index];
  cv::Mat H = _homography.inv();
  cv::Mat pt1 = (cv::Mat_<double>(3, 1) << point.x, point.y, 1);
  cv::Mat pt2 = H * pt1;
  pt2 /= pt2.at<double>(2);
  cv::Point2f pt(pt2.at<double>(0, 0), pt2.at<double>(0, 1));

  // map point back into 3d space
  std::vector<Vec2d> inpt;
  std::vector<Vec2d> outpt;
  inpt.push_back(Vec2d(pt.x, pt.y));
  undistortPoints(inpt, outpt, _camMat, _distCoeffs);
  double z = _m0.tvec[2]; // take z value from marker (like a pro)

  return Vec3d(outpt.at(0)[0]*z, outpt.at(0)[1]*z, z);
}

void AbsolutePath::OnEvent(HomographyComputedEvent& e) {
  _homography = e.GetHomography();
  _camMat = e.GetCamMatrix();
  _distCoeffs = e.GetDistCoeffs();
  _m0 = e.GetMarker0();
  _m1 = e.GetMarker1();
  _homo_computed = true;
}
