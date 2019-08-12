#ifndef _CAM_H
#define _CAM_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

/**
 * Webcam class
 */
class Cam {
 private:
  cv::VideoCapture _cap;

 public:
  Cam(int camId);
  bool Grab();
  cv::Mat GetFrame();
};

#endif
