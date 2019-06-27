#ifndef _IMAGE_PROCESSOR_H
#define _IMAGE_PROCESSOR_H

/*
 *
 *
 *
 *
 *
 *
 */

#include <Eigen/Dense>
#include <algorithm>
#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace Eigen;
using namespace std;
using namespace cv;

/*
 *  Casts CV vectors
 */
template <typename I, typename O>
static vector<O> CastVector(vector<I> inputVector) {
  vector<O> outputVector;

  for (int i = 0; i < inputVector.size(); i++) {
    outputVector.push_back(static_cast<O>(inputVector[i]));
  }

  return outputVector;
}

/*
 * Enum for Marker IDs
 */
enum MarkerCategory {
  Border0 = 0,
  Border1 = 1,
  Start = 2,
  Pause = 3,
  Action = 4,
  Cursor0 = 5,
  Cursor1 = 6,
  Cursor2 = 7,
  Cursor3 = 8,
  Cursor4 = 9
};

/*
 * Aruco marker
 */
struct Marker {
  MarkerCategory category;
  vector<Point2f> corners;
  Vec3d tvec;
  Vec3d rvec;
};

/*
 *
 */
class ImageProcessor {
 public:
  ImageProcessor();
  vector<Marker> DetectMarkers(Mat image);
  Mat GetDistCoeffs();
  Mat DrawMarkers(Mat image, vector<Marker> markers);
  bool ContainsBorderMarkers(vector<Marker> markers);
  Mat WarpPaperImage(Mat image, vector<Marker> markers, int warpedImageWidth,
                     int warpedImageHeight);
  Marker GetMarkerOfCategory(vector<Marker> markers, MarkerCategory category);

 private:
  Mat _camMatrix, _distCoeffs;
  Ptr<aruco::Dictionary> _dictionary;
  Ptr<aruco::DetectorParameters> _detectorParams;
  float _markerLength;

  vector<Point2f> CalcPaperBorders(vector<Marker> markers);
  vector<Point2i> Vertices2ConvexHull(vector<Point2f> vertices);
  Mat CutMarker(Mat image, vector<Point2f> vertices);
};

#endif