#ifndef _CAM_H
#define _CAM_H

/*
 *
 *
 *
 *
 *
 *
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*
 * 
 */
class Cam
{
private:
    VideoCapture _cap;

public:
    Cam(int camId);
    bool Grab();
    Mat GetFrame();
};

#endif