#include "path_detector.h"
#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdint.h>
using namespace cv;
using namespace std;


PathDetector::PathDetector()
{
}

PathDetector::PathDetector(string frameName)
{
	frame = imread(frameName, 1);
}


PathDetector::~PathDetector()
{
}

void PathDetector::ShowFrame()
{
	imshow("Frame", frame);
	waitKey(0);
}

void PathDetector::ShowModifiedFrame()
{
	imshow(currentMask, modifiedFrame);
	waitKey(0);
}

void PathDetector::RangeThresholdBinary(Scalar lowerBoundary, Scalar upperBoundary)
{
	inRange(frame, lowerBoundary, upperBoundary, modifiedFrame);
	currentMask = "Black and White";
}

void PathDetector::Gaussian()
{
	//Gauss
	GaussianBlur(frame, modifiedFrame, Size(3, 3), 0, 0, 4);
	cvtColor(frame, modifiedFrame, COLOR_BGR2GRAY);
	currentMask = "Gauss";
}

void PathDetector::Laplace()
{
	//Gauss->Laplace->Edgedetection
	Gaussian();

	Mat help;
	//Laplace
	Laplacian(modifiedFrame, help, 3, 1, 1.0, 0.0, 4);
	convertScaleAbs(help, modifiedFrame);
	currentMask = "Laplace auf Gauss";
}

void PathDetector::CannyTwo()
{
	Canny(frame, modifiedFrame, 50, 200, 3);
	currentMask = "Canny";
}

void PathDetector::DrawHoughLines(int threshold, double minLineLength, double maxLineGap)
{
	//Canny-> HoughLines
	CannyTwo();

	Mat help;

	cvtColor(modifiedFrame, help, COLOR_GRAY2BGR);

	//#if 0

	vector<Vec4i> lines;

	HoughLinesP(modifiedFrame, lines, 1, 3.1415 / 180, threshold, minLineLength, maxLineGap);

	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(help, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3);
	}

	modifiedFrame = help.clone();
	currentMask = "HoughLines auf Canny";
}

void PathDetector::Cornerdetection()
{
	/*
	Mat help;
	Mat corners;

	//bild in graut�ne umwandeln
	cvtColor(frame, help, COLOR_BGR2GRAY);
	
	//Cornerdetection
	modifiedFrame = frame.clone();
	goodFeaturesToTrack(help, corners, 10, 0.01, 10, Mat(), 3, false, 0.04);

	/// Draw corners detected

	cout << "** Number of corners detected: " << corners << endl;
	int r = 4;
	for (int i = 0; i < corners.size().height; i++) {
		Point2i thing = corners.at<Point2i>(i, 0);
		circle(modifiedFrame, thing, r, Scalar(61, 70, 252), -1, 8, 0);
	}
	currentMask = "Corners";
	*/
}

void PathDetector::AdaptiveThreshold()
{
	Mat help;
	cvtColor(frame, help, COLOR_BGR2GRAY);
	//Adaptive Threshhold
	adaptiveThreshold(help, modifiedFrame, 200, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 2);
	currentMask = "Adaptive Threshold";
}

void PathDetector::AdaptiveThresholdBinary()
{
	//Adaptive ThreshholdXBin�r
	AdaptiveThreshold();
	Mat help, bnw;
	inRange(frame, Scalar(0, 0, 0), Scalar(75, 75, 75), bnw);
	help = modifiedFrame.mul(bnw);
	modifiedFrame = help.clone();
	currentMask = "Adaptive Threshold Binary";
}
