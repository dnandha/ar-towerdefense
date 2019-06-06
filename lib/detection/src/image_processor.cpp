#include "image_processor.h"

struct sortY
{
    bool operator()(cv::Point pt1, cv::Point pt2) { return (pt1.y < pt2.y); }
} mySortY;
struct sortX
{
    bool operator()(cv::Point pt1, cv::Point pt2) { return (pt1.x < pt2.x); }
} mySortX;

Mat ImageProcessor::Process(Mat image)
{
    Mat camMatrix, distCoeffs;

    // calibration results
    camMatrix = (Mat_<double>(3, 3) << 1.7436456814490055e+03, 0., 9.7759459497189175e+02, 0.,
                 1.7258234422776150e+03, 6.3571222615276474e+02, 0., 0., 1.);
    distCoeffs = (Mat_<double>(5, 1) << 2.0355046505753788e-01, -1.9960458729811597e+00,
                  4.9231630538066791e-02, 1.1433409329564154e-02, 0.);

    Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();
    // Corner refinement method (0: None, 1: Subpixel, 2:contour, 3: AprilTag 2)
    detectorParams->cornerRefinementMethod = 1;

    // dictionary: DICT_4X4_50=0, DICT_4X4_100=1, DICT_4X4_250=2,"
    // "DICT_4X4_1000=3, DICT_5X5_50=4, DICT_5X5_100=5, DICT_5X5_250=6, DICT_5X5_1000=7, "
    // "DICT_6X6_50=8, DICT_6X6_100=9, DICT_6X6_250=10, DICT_6X6_1000=11, DICT_7X7_50=12,"
    // "DICT_7X7_100=13, DICT_7X7_250=14, DICT_7X7_1000=15, DICT_ARUCO_ORIGINAL = 16,"
    // "DICT_APRILTAG_16h5=17, DICT_APRILTAG_25h9=18, DICT_APRILTAG_36h10=19, DICT_APRILTAG_36h11=20}
    Ptr<aruco::Dictionary> dictionary =
        aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(0));

    float markerLength = 2.6;

    // marker ID's
    vector<int> ids;
    // lists of corners of detected markers (clockwise order)
    vector<vector<Point2f>> corners, rejected;
    // rotation and translation vectors respectively for each of the markers in corners
    vector<Vec3d> rvecs, tvecs;

    // detect markers and estimate pose
    aruco::detectMarkers(image, dictionary, corners, ids, detectorParams, rejected);
    if (ids.size() > 0)
    {
        aruco::estimatePoseSingleMarkers(corners, markerLength, camMatrix, distCoeffs, rvecs,
                                         tvecs);
    }

    if (ids.size() >= 2)
    {
        vector<Point2f> paperBorders = CalcPaperBorders(corners);
        int lineThickness = 2;
        line(image, paperBorders[0], paperBorders[1], (0, 255, 0), lineThickness);
        line(image, paperBorders[1], paperBorders[2], (0, 255, 0), lineThickness);
        line(image, paperBorders[0], paperBorders[3], (0, 255, 0), lineThickness);
        line(image, paperBorders[2], paperBorders[3], (0, 255, 0), lineThickness);

        Mat cuttedMarker0Image = CutConvecHull(image, corners[0]);
        Mat cuttedMarker1Image = CutConvecHull(cuttedMarker0Image, corners[1]);

        imshow("cutted_image", cuttedMarker1Image);

        vector<Point2f> orgPaperBorders = CastVector<Point2i, Point2f>(Vertices2ConvexHull(paperBorders));

        sort(orgPaperBorders.begin(), orgPaperBorders.end(), mySortY);
        sort(orgPaperBorders.begin(), orgPaperBorders.begin() + 2, mySortX);
        sort(orgPaperBorders.begin() + 2, orgPaperBorders.end(), mySortX);

        vector<Point2f> destPaperBorders;

        destPaperBorders.push_back(Point2i(0, 0));
        destPaperBorders.push_back(Point2i(900, 0));
        destPaperBorders.push_back(Point2i(0, 600));
        destPaperBorders.push_back(Point2i(900, 600));

        Mat perspectiveMat = getPerspectiveTransform(orgPaperBorders, destPaperBorders);

        Mat warpedImage;
        warpPerspective(cuttedMarker1Image, warpedImage, perspectiveMat, Size(900, 600));

        imshow("perspective_transform", warpedImage);
    }

    // draw results
    if (ids.size() > 0)
    {
        aruco::drawDetectedMarkers(image, corners, ids);

        for (unsigned int i = 0; i < ids.size(); i++)
            aruco::drawAxis(image, camMatrix, distCoeffs, rvecs[i], tvecs[i],
                            markerLength * 0.5f);
    }

    if (rejected.size() > 0)
        aruco::drawDetectedMarkers(image, rejected, noArray(), Scalar(100, 0, 255));

    Mat processed_image;
    image.copyTo(processed_image);
    return processed_image;
}

vector<Point2f> ImageProcessor::CalcPaperBorders(vector<vector<Point2f>> corners)
{
    vector<Point2f> m0c = corners[0];
    vector<Point2f> m1c = corners[1];

    typedef Eigen::Hyperplane<float, 2> Line2; // Hyperplane in 2d is a line
    typedef Eigen::Vector2f Vec2;

    // Collect points of interrest
    Vec2 m0c0(m0c[0].x, m0c[0].y);
    Vec2 m0c1(m0c[1].x, m0c[1].y);
    Vec2 m0c3(m0c[3].x, m0c[3].y);
    Vec2 m1c0(m1c[0].x, m1c[0].y);
    Vec2 m1c1(m1c[1].x, m1c[1].y);
    Vec2 m1c3(m1c[3].x, m1c[3].y);

    // Calcuate lines
    Line2 m0c0c1 = Line2::Through(m0c0, m0c1);
    Line2 m0c0c3 = Line2::Through(m0c0, m0c3);
    Line2 m1c0c1 = Line2::Through(m1c0, m1c1);
    Line2 m1c0c3 = Line2::Through(m1c0, m1c3);

    // Calucate intersections of lines
    Vec2 intersection1 = m0c0c1.intersection(m1c0c3);
    Vec2 intersection2 = m0c0c3.intersection(m1c0c1);

    Point2f intersecPoint1(intersection1[0], intersection1[1]);
    Point2f intersecPoint2(intersection2[0], intersection2[1]);

    vector<Point2f> paperBorders;
    paperBorders.push_back(m0c[0]);
    paperBorders.push_back(intersecPoint1);
    paperBorders.push_back(m1c[0]);
    paperBorders.push_back(intersecPoint2);

    return paperBorders;
}

vector<Point2i> ImageProcessor::Vertices2ConvexHull(vector<Point2f> vertices)
{
    // Cast Point2f to Point2i for fillConvexPoly function
    vector<Point2i> vertices_i = CastVector<Point2f, Point2i>(vertices);

    vector<Point2i> hull;

    convexHull(vertices_i, hull);

    return hull;
}

Mat ImageProcessor::CutConvecHull(Mat image, vector<Point2f> vertices)
{
    vector<Point2i> hull = Vertices2ConvexHull(vertices);

    Mat roi(image.rows, image.cols, CV_8U, Scalar(255)); //white image
    fillConvexPoly(roi, hull, Scalar(0));                //draw ROI in black

    //Filter original image according to ROI
    Mat cutted_image;
    image.copyTo(cutted_image, roi); //alternative: bitwise_and(image, roi, filtered_image);

    return cutted_image;
}