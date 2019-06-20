#include "PathDetector.h"
#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdint.h>
#include "Mesh.h"
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

Size PathDetector::FrameSize()
{
	return frame.size();
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

vector<Point2f> PathDetector::Cornerdetection(int maxCorners, double qualityLevel, double minDistance, int blockSize, bool useHarrisDetector, double k)
{
	RNG rng(12345);
	Mat help;
	vector<Point2f> corners;

	//bild in grautöne umwandeln
	//cvtColor(frame, help, COLOR_BGR2GRAY);
	//inRange(frame, Scalar(0, 0, 0), Scalar(130, 150, 150), help);
	CannyTwo();
	//cvtColor(modifiedFrame, help, COLOR_GRAY2BGR);
	
	//Cornerdetection
	
	goodFeaturesToTrack(modifiedFrame, corners, maxCorners, qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector, k);
	modifiedFrame = frame.clone();
	/// Draw corners detected

	cout << "** Number of corners detected: " << corners << endl;
	int r = 4;
	for (int i = 0; i < corners.size(); i++)
	{
		circle(modifiedFrame, corners[i], r, Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
			rng.uniform(0, 255)), -1, 8, 0);
	}
	currentMask = "Corners";

	return corners;
	
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
	//Adaptive ThreshholdXBinär
	AdaptiveThreshold();
	Mat help, bnw;
	inRange(frame, Scalar(0, 0, 0), Scalar(75, 75, 75), bnw);
	help = modifiedFrame.mul(bnw);
	modifiedFrame = help.clone();
	currentMask = "Adaptive Threshold Binary";
}

void PathDetector:: PathMeshInImg(Mesh mesh)
{
	modifiedFrame = frame.clone();
	RNG rng(12345);
	Scalar color;
	for (int p = 0; p < mesh.polygons.size(); p++)
	{
		color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		line(modifiedFrame, Point(mesh.vertices[mesh.polygons[p].indexA].x, mesh.vertices[mesh.polygons[p].indexA].y), Point(mesh.vertices[mesh.polygons[p].indexB].x, mesh.vertices[mesh.polygons[p].indexB].y), color);
		line(modifiedFrame, Point(mesh.vertices[mesh.polygons[p].indexB].x, mesh.vertices[mesh.polygons[p].indexB].y), Point(mesh.vertices[mesh.polygons[p].indexC].x, mesh.vertices[mesh.polygons[p].indexC].y), color);
		line(modifiedFrame, Point(mesh.vertices[mesh.polygons[p].indexC].x, mesh.vertices[mesh.polygons[p].indexC].y), Point(mesh.vertices[mesh.polygons[p].indexA].x, mesh.vertices[mesh.polygons[p].indexA].y), color);
	}
	currentMask = "Mesh Path";
}

void PathDetector::PathCentroids(Mesh mesh)
{
	modifiedFrame = frame.clone();
	for (int p = 0; p < mesh.polygons.size()-1; p++)
	{
		line(modifiedFrame, Point(mesh.polygons[p].centroid.x, mesh.polygons[p].centroid.y), Point(mesh.polygons[p+1].centroid.x, mesh.polygons[p+1].centroid.y), Scalar(0, 0, 255));
	}
	currentMask = "Centroid Path";
}
