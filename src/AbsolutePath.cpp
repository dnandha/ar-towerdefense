#include "extern.h"

cv::Point2f AbsolutePath::GetPoint(int index) {
  cv::Point2f point = _relativePath[index];
  cv::Mat H = _homography.inv();
  cv::Mat pt1 = (cv::Mat_<double>(3, 1) << point.x, point.y, 1);
  cv::Mat pt2 = H * pt1;
  pt2 /= pt2.at<double>(2);
  return cv::Point2f(pt2.at<double>(0, 0), pt2.at<double>(0, 1));
}

void cv::Point2f onEvent(HomographyComputedEvent& e) {
  _homography = e.GetHomography();
}
