#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdint.h>
#include "PathDetector.h"
#include "Mesh.h"
using namespace cv;
using namespace std;

/**
 * Code for thinning a binary image using Guo-Hall algorithm.
 */

 /**
  * Perform one thinning iteration.
  * Normally you wouldn't call this function directly from your code.
  *
  * @param  im    Binary image with range = 0-1
  * @param  iter  0=even, 1=odd
  */
/*
void thinningGuoHallIteration(cv::Mat& im, int iter)
{
	cv::Mat marker = cv::Mat::zeros(im.size(), CV_8UC1);

	for (int i = 1; i < im.rows; i++)
	{
		for (int j = 1; j < im.cols; j++)
		{
			uchar p2 = im.at<uchar>(i - 1, j);
			uchar p3 = im.at<uchar>(i - 1, j + 1);
			uchar p4 = im.at<uchar>(i, j + 1);
			uchar p5 = im.at<uchar>(i + 1, j + 1);
			uchar p6 = im.at<uchar>(i + 1, j);
			uchar p7 = im.at<uchar>(i + 1, j - 1);
			uchar p8 = im.at<uchar>(i, j - 1);
			uchar p9 = im.at<uchar>(i - 1, j - 1);

			int C = (!p2 & (p3 | p4)) + (!p4 & (p5 | p6)) +
				(!p6 & (p7 | p8)) + (!p8 & (p9 | p2));
			int N1 = (p9 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
			int N2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p9);
			int N = N1 < N2 ? N1 : N2;
			int m = iter == 0 ? ((p6 | p7 | !p9) & p8) : ((p2 | p3 | !p5) & p4);

			if (C == 1 && (N >= 2 && N <= 3) & m == 0)
				marker.at<uchar>(i, j) = 1;
		}
	}

	im &= ~marker;
}*/

/**
 * Function for thinning the given binary image
 *
 * @param  im  Binary image with range = 0-255
 */
/*
void thinningGuoHall(cv::Mat& im)
{
	im /= 255;

	cv::Mat prev = cv::Mat::zeros(im.size(), CV_8UC1);//CV_8UC1);
	cv::Mat diff;

	do {
		thinningGuoHallIteration(im, 0);
		thinningGuoHallIteration(im, 1);
		cv::absdiff(im, prev, diff);
		im.copyTo(prev);
	} while (cv::countNonZero(diff) > 0);

	im *= 255;
}*/

//const int fps = 20;

int main(){
	string imagename = "img4";

	vector<Point2f> pathCorners;
	
	//get necessary data form image
	PathDetector detector(imagename+".jpg");
	pathCorners = detector.Cornerdetection();

	//preparing mesh input
	Size size = detector.FrameSize();
	vector<Vertex> vertices;
	vector<Vertex> cornerVertices;
	cornerVertices.push_back({ (double)size.width, (double) size.height, 0.0 });
	cornerVertices.push_back({ (double)size.width, 0.0, 0.0 });
	cornerVertices.push_back({ 0.0, (double)size.height, 0.0 });
	cornerVertices.push_back({ 0.0, 0.0, 0.0 });
	int c;
	for (c = 0; c < pathCorners.size(); c++)
	{
		vertices.push_back({ pathCorners[c].x, pathCorners[c].y, 0.0 });
	}

	//create Mesh
	Mesh mesh(vertices, cornerVertices, size.width, size.height);

	//adjust mesh data and create .obj-file
	mesh.polygons.clear();
	mesh.CreatePolygonsRelativToIndividualVertices();
	mesh.MakeObjFile(imagename+"_Object.obj");

	//display mesh data on image
	detector.PathMeshInImg(mesh);
	detector.ShowModifiedFrame();

	detector.PathCentroids(mesh);
	detector.ShowModifiedFrame();
	
	return 0;
}
/*
//EXAMPLES

		//Thinning
		
		inRange(frame, Scalar(0, 0, 0), Scalar(75, 75, 75), bnw);
		imshow("gray", bnw);
		thinningGuoHall(bnw);
		imshow("thinning", bnw);
		
		
		for (size_t i = 0; i < corners.size(); i++) {
			keyP.push_back(KeyPoint(corners[i], 1.f));
		}
		drawKeypoints(frame, keyP, frame, Scalar(0,0,0));
		imshow("frame", frame);
		

frame = imread("img1.jpg", 1);
	inRange(frame, Scalar(0, 0, 0), Scalar(125, 125, 125), bnw);
	imshow("gray", bnw);
	thinningGuoHall(bnw);
	imshow("thinning", bnw);

	frame1 = imread("img2.jpg", 1);
	inRange(frame1, Scalar(0, 0, 0), Scalar(125, 125, 125), bnw1);
	imshow("gray1", bnw1);
	thinningGuoHall(bnw1);
	imshow("thinning1", bnw1);


	Mat image = imread("img1.jpg", 1);
	namedWindow("image", WINDOW_FREERATIO);
	imshow("image", image);
	waitKey(0);

	//GET COLOR BLACK	(Blue, Green, Red)
	Mat OutputImage;
	inRange(image, Scalar(0, 0, 0), Scalar(100, 100, 100), OutputImage);

	namedWindow("Output", WINDOW_FREERATIO);

	imshow("Output", OutputImage);
	waitKey(0);


	//Accessing Pixels
	Mat Modified = imread("img1.jpg", 1);
	for (int row = 0; row < OutputImage.rows; row++) {
		for (int col = 0; col < OutputImage.cols; col++) {
			//Accesing Blue Channel of Pixel
			Modified.at<cv::Vec3b>(row, col)[0] = OutputImage.at<Vec3b>(row, col)[0] * 0;
		}
	}
	namedWindow("Modified", WINDOW_FREERATIO);
	imshow("Modified", Modified);


*/