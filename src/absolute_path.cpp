#include "extern.h"

Vec3d UVtoXYZ(Point2f uv, Mat camMat, Marker m0, Marker m1) {
    Vec3d point;
    Point2f cornervec = m1.corners[0] - m0.corners[0]; // pix
    Vec3d ptvec = m1.tvec - m0.tvec; // ogre

    double pixpo_x = cornervec.x / ptvec[0]; // px / ogre
    double pixpo_y = cornervec.y / ptvec[1]; // px / ogre
    
    double x3d = uv.x / pixpo_x;
    double y3d = uv.y / pixpo_y;
    double z3d = 0;

    //double rel = norm(Mat(cornervec)) / norm(ptvec);

//    Mat inv = camMat.inv();
//  std::cout << "GOTIT1" << std::endl;
//    Mat points = inv * Mat(uv);
//  std::cout << "GOTIT2" << std::endl;
//
    Vec3d pt(x3d, y3d, z3d);

    // transform 
    Mat R;
    Rodrigues(m1.rvec, R);
    //Mat transpt = R.t() * pt;
    //transpt += R.t() * m1.tvec;
    
    Mat transpt = R * pt + Mat(m1.tvec);

    std::cout << transpt << std::endl;

    return Vec3d(transpt.at<double>(0,0), transpt.at<double>(1,0), transpt.at<double>(2,0));
}

cv::Vec3d AbsolutePath::GetPoint(int index) {
    if (!_homo_computed)
        return Vec3d(0.0, 0.0, 0.0);

  cv::Point2f point = _relativePath[index];
  cv::Mat H = _homography.inv();
  cv::Mat pt1 = (cv::Mat_<double>(3, 1) << point.x, point.y, 1);
  cv::Mat pt2 = H * pt1;
  pt2 /= pt2.at<double>(2);
  cv::Point2f pt(pt2.at<double>(0, 0), pt2.at<double>(0, 1));
  std::cout << "POINT " << pt << std::endl;
  return UVtoXYZ(pt, _camMat, _m0, _m1);
}

void AbsolutePath::OnEvent(HomographyComputedEvent& e) {
  _homography = e.GetHomography();
  _camMat = e.GetCamMatrix();
  _m0 = e.GetMarker0();
  _m1 = e.GetMarker1();
  _homo_computed = true;
}
