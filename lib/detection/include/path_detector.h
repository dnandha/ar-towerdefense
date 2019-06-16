#ifndef _PATH_DETECTOR_H
#define _PATH_DETECTOR_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdint.h>
using namespace cv;
using namespace std;
class PathDetector
{
public:
	PathDetector();
	PathDetector(string);
	~PathDetector();
	void ShowFrame();
	void ShowModifiedFrame();
	void RangeThresholdBinary(Scalar lowerBoundary = Scalar(0, 0, 0), Scalar upperBoundary = Scalar(75, 75, 75));
	void Gaussian();
	void Laplace();
	void CannyTwo();
	void DrawHoughLines(int threshold = 1, double minLineLength = 10, double maxLineGap = 5);
	void Cornerdetection();
	void AdaptiveThreshold();
	void AdaptiveThresholdBinary();

private:
	Mat frame;
	Mat modifiedFrame;
	string currentMask;
};

#endif