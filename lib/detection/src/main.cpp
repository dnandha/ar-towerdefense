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

#include "cam.h"
#include "path_detector.h"
#include "paths_initializer.h"

std::vector<cv::Point3f> getCornersInCameraWorld(double side, cv::Vec3d rvec,
                                                 cv::Vec3d tvec) {
  double half_side = side / 2;

  // compute rot_mat
  cv::Mat rot_mat;
  Rodrigues(rvec, rot_mat);

  // transpose of rot_mat for easy columns extraction
  cv::Mat rot_mat_t = rot_mat.t();

  // the two E-O and F-O vectors
  double *tmp = rot_mat_t.ptr<double>(0);
  cv::Point3f camWorldE(tmp[0] * half_side, tmp[1] * half_side,
                        tmp[2] * half_side);

  tmp = rot_mat_t.ptr<double>(1);
  cv::Point3f camWorldF(tmp[0] * half_side, tmp[1] * half_side,
                        tmp[2] * half_side);

  // convert tvec to point
  cv::Point3f tvec_3f(tvec[0], tvec[1], tvec[2]);

  // return vector:
  std::vector<cv::Point3f> ret(4, tvec_3f);

  ret[0] += camWorldE + camWorldF;
  ret[1] += -camWorldE + camWorldF;
  ret[2] += -camWorldE - camWorldF;
  ret[3] += camWorldE - camWorldF;

  return ret;
}

/*
 * Main
 */
int main() {
  Cam cam(1);
  ImageProcessor improc;

  PathDetector detector;
  PathsInitializer initializer(cam);
  std::vector<std::vector<cv::Point2f>> paths = initializer.InitializePaths();

  cv::Mat camMat = improc.GetCamMatrix();

  while (cam.Grab()) {
    cv::Mat image = cam.GetFrame();
    vector<Marker> markers = improc.DetectMarkers(image);

    cv::Mat markerImage = improc.DrawMarkers(image, markers);

    bool m0found = false;
    bool m1found = false;
    Marker m0;
    Marker m1;
    for (auto &marker : markers) {
      if (marker.category == 0) {
        m0found = true;
        m0 = marker;
      }
      if (marker.category == 1) {
        m1found = true;
        m1 = marker;
      }
    }
    if (m0found && m1found) {
      cv::Point3f m0Corner = getCornersInCameraWorld(2.6, m0.rvec, m0.tvec)[0];
      cv::Point3f m1Corner = getCornersInCameraWorld(2.6, m1.rvec, m1.tvec)[0];

      // transform m1 corner to world coordinates
      cv::Mat R;
      cv::Rodrigues(m1.rvec, R);
      cv::Mat m1c(m1Corner);
      cv::Mat m1c_f64;
      m1c.convertTo(m1c_f64, CV_64F);

      cv::Mat m1c_in_c = R * m1c_f64 + cv::Mat(m1.tvec);

      // transform m1 corner in world coordinates to m0 coordinates
      cv::Rodrigues(m0.rvec, R);
      cv::Mat m1c0_in_m0 = R.t() * m1c_in_c - R.t() * cv::Mat(m0.tvec);

      // span paper area in m0 coordinates
      cv::Point3f origin(0.0, 0.0, 0.0);
      double x_max = m1c0_in_m0.at<double>(0, 0);
      double y_max = m1c0_in_m0.at<double>(1, 0);

      std::vector<cv::Point3f> transformedPath;
      double org_x_axis = 900;
      double org_y_axis = 600;
      for (auto &point : paths[0]) {
        // scale path points and reverse y axis
        cv::Point3f scaledPoint(point.x * (x_max / org_x_axis),
                                (org_y_axis - point.y) * (y_max / org_y_axis),
                                0);
        cv::Mat scaledPointMat(scaledPoint);
        cv::Mat scaledPoint_f64;
        scaledPointMat.convertTo(scaledPoint_f64, CV_64F);
        // transform points back to camera coordinates
        cv::Mat transformedPoint = R * scaledPoint_f64 + cv::Mat(m0.tvec);
        transformedPath.push_back(
            cv::Point3f(transformedPoint.at<double>(0, 0),
                        transformedPoint.at<double>(1, 0),
                        transformedPoint.at<double>(2, 0)));
      }

      std::cout << cv::Mat(transformedPath[0]) << std::endl;

      // draw path
      for (int i = 0; i < transformedPath.size() - 1; i++) {
        double fx = camMat.at<double>(0, 0);
        double fy = camMat.at<double>(1, 1);
        double cx = camMat.at<double>(0, 2);
        double cy = camMat.at<double>(1, 2);

        double u0 = fx * transformedPath[i].x / transformedPath[i].z + cx;
        double v0 = fy * transformedPath[i].y / transformedPath[i].z + cy;
        double u1 =
            fx * transformedPath[i + 1].x / transformedPath[i + 1].z + cx;
        double v1 =
            fy * transformedPath[i + 1].y / transformedPath[i + 1].z + cy;
        cv::Point2f p0(u0, v0);
        cv::Point2f p1(u1, v1);
        cv::line(markerImage, p0, p1, cv::Scalar(0, 0, 255));
      }
    }
    imshow("cam feed", markerImage);
  }
  return 1;
}
