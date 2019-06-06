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
 * 
 */
class ImageProcessor
{
public:
    Mat Process(Mat image);

private:
    vector<Point2f> CalcPaperBorders(vector<vector<Point2f>> corners);
    vector<Point2i> Vertices2ConvexHull(vector<Point2f> vertices);
    Mat CutConvecHull(Mat image, vector<Point2f> vertices);
};