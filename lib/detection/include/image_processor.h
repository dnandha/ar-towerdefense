/*
 *
 *
 *
 *
 *
 *
 */

#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;
using namespace cv;

/*
 *  Casts CV vectors
 */
template <typename I, typename O>
static vector<O> CastVector(vector<I> inputVector)
{
    vector<O> outputVector;

    for (int i = 0; i < inputVector.size(); i++)
    {
        outputVector.push_back(static_cast<O>(inputVector[i]));
    }

    return outputVector;
}

/*
 * Aruco marker
 */
struct Marker
{
    int id;
    vector<Point2f> corners;
    Vec3d tvec;
    Vec3d rvec;
};

/*
 * 
 */
class ImageProcessor
{
public:
    ImageProcessor();
    vector<Marker> DetectMarkers(Mat image);
    Mat DrawMarkers(Mat image, vector<Marker> markers);
    bool ContainsBorderMarkers(vector<Marker> markers);
    Mat WarpPaperImage(Mat image, vector<Marker> markers);
    Marker GetMarkerForID(vector<Marker> markers, int id);

private:
    Mat _camMatrix, _distCoeffs;
    Ptr<aruco::Dictionary> _dictionary;
    Ptr<aruco::DetectorParameters> _detectorParams;
    float _markerLength;

    vector<Point2f> CalcPaperBorders(vector<Marker> markers);
    vector<Point2i> Vertices2ConvexHull(vector<Point2f> vertices);
    Mat CutConvecHull(Mat image, vector<Point2f> vertices);
};