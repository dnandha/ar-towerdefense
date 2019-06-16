#include "image_processor.h"

struct SortY
{
    bool operator()(cv::Point pt1, cv::Point pt2) { return (pt1.y < pt2.y); }
} mySortY;

struct SortX
{
    bool operator()(cv::Point pt1, cv::Point pt2) { return (pt1.x < pt2.x); }
} mySortX;

ImageProcessor::ImageProcessor()
{
    // calibration results
    _camMatrix = (Mat_<double>(3, 3) << 1.7436456814490055e+03, 0., 9.7759459497189175e+02, 0.,
                  1.7258234422776150e+03, 6.3571222615276474e+02, 0., 0., 1.);
    _distCoeffs = (Mat_<double>(5, 1) << 2.0355046505753788e-01, -1.9960458729811597e+00,
                   4.9231630538066791e-02, 1.1433409329564154e-02, 0.);

    // dictionary: DICT_4X4_50=0, DICT_4X4_100=1, DICT_4X4_250=2,"
    // "DICT_4X4_1000=3, DICT_5X5_50=4, DICT_5X5_100=5, DICT_5X5_250=6, DICT_5X5_1000=7, "
    // "DICT_6X6_50=8, DICT_6X6_100=9, DICT_6X6_250=10, DICT_6X6_1000=11, DICT_7X7_50=12,"
    // "DICT_7X7_100=13, DICT_7X7_250=14, DICT_7X7_1000=15, DICT_ARUCO_ORIGINAL = 16,"
    // "DICT_APRILTAG_16h5=17, DICT_APRILTAG_25h9=18, DICT_APRILTAG_36h10=19, DICT_APRILTAG_36h11=20}
    _dictionary =
        aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(0));

    // using standard detector parametes
    _detectorParams = aruco::DetectorParameters::create();
    // Corner refinement method (0: None, 1: Subpixel, 2:contour, 3: AprilTag 2)
    _detectorParams->cornerRefinementMethod = 2;

    _markerLength = 2.6;
}

Mat ImageProcessor::GetDistCoeffs()
{
    return _distCoeffs;
}

vector<Marker> ImageProcessor::DetectMarkers(Mat image)
{
    try
    {
        // marker ID's
        vector<int> ids;
        // lists of corners of detected markers (clockwise order)
        vector<vector<Point2f>> corners, rejected;
        // rotation and translation vectors respectively for each of the markers in corners
        vector<Vec3d> rvecs, tvecs;

        vector<Marker> markers;

        // detect markers and estimate pose
        aruco::detectMarkers(image, _dictionary, corners, ids, _detectorParams, rejected);

        if (ids.size() > 0)
        {
            aruco::estimatePoseSingleMarkers(corners, _markerLength, _camMatrix, _distCoeffs, rvecs,
                                             tvecs);

            for (int i = 0; i < ids.size(); i++)
            {
                Marker marker;
                marker.category = static_cast<MarkerCategory>(ids[i]);
                marker.corners = corners[i];
                marker.tvec = tvecs[i];
                marker.rvec = rvecs[i];

                markers.push_back(marker);
            }
        }

        return markers;
    }
    catch (std::exception &e)
    {
        std::cout << "Exception:" << std::endl;
        std::cout << e.what() << std::endl;
    }
}

Mat ImageProcessor::DrawMarkers(Mat image, vector<Marker> markers)
{
    if (markers.size() > 0)
    {
        vector<int> ids;
        vector<vector<Point2f>> corners;
        vector<Vec3d> rvecs, tvecs;

        for (int i = 0; i < markers.size(); i++)
        {
            ids.push_back((int)markers[i].category);
            corners.push_back(markers[i].corners);
            rvecs.push_back(markers[i].rvec);
            tvecs.push_back(markers[i].tvec);
        }

        Mat imageCopy;
        image.copyTo(imageCopy);

        aruco::drawDetectedMarkers(imageCopy, corners, ids);

        for (unsigned int i = 0; i < ids.size(); i++)
            aruco::drawAxis(imageCopy, _camMatrix, _distCoeffs, rvecs[i], tvecs[i],
                            _markerLength * 0.5f);
        return imageCopy;
    }
    return image;
}

bool ImageProcessor::ContainsBorderMarkers(vector<Marker> markers)
{
    if (markers.size() == 0)
    {
        return false;
    }

    bool marker0found = false;
    bool marker1found = false;

    for (auto const &marker : markers)
    {
        if (marker.category == Border0)
        {
            marker0found = true;
        }
        if (marker.category == Border1)
        {
            marker1found = true;
        }
    }

    if (marker0found && marker1found)
    {
        return true;
    }
    return false;
}

Mat ImageProcessor::WarpPaperImage(Mat image, vector<Marker> markers, int warpedImageWidth, int warpedImageHeight)
{
    Mat imageCopy;
    image.copyTo(imageCopy);

    vector<Point2f> paperBorders = CalcPaperBorders(markers);

    Marker marker0 = GetMarkerOfCategory(markers, Border0);
    Marker marker1 = GetMarkerOfCategory(markers, Border1);

    Mat cuttedMarker0Image = CutConvecHull(imageCopy, marker0.corners);
    Mat cuttedMarker1Image = CutConvecHull(cuttedMarker0Image, marker1.corners);

    vector<Point2f> orgPaperBorders = CastVector<Point2i, Point2f>(Vertices2ConvexHull(paperBorders));

    // sorting paper borders
    sort(orgPaperBorders.begin(), orgPaperBorders.end(), mySortY);
    sort(orgPaperBorders.begin(), orgPaperBorders.begin() + 2, mySortX);
    sort(orgPaperBorders.begin() + 2, orgPaperBorders.end(), mySortX);

    vector<Point2f> destPaperBorders;

    destPaperBorders.push_back(Point2i(0, 0));
    destPaperBorders.push_back(Point2i(warpedImageWidth, 0));
    destPaperBorders.push_back(Point2i(0, warpedImageHeight));
    destPaperBorders.push_back(Point2i(warpedImageWidth, warpedImageHeight));

    Mat perspectiveMat = getPerspectiveTransform(orgPaperBorders, destPaperBorders);

    Mat warpedImage;
    warpPerspective(cuttedMarker1Image, warpedImage, perspectiveMat, Size(900, 600));

    return warpedImage;
}

vector<Point2f> ImageProcessor::CalcPaperBorders(vector<Marker> markers)
{
    if (markers.size() == 0 || !ContainsBorderMarkers(markers))
    {
        throw 20;
    }

    Marker marker0 = GetMarkerOfCategory(markers, Border0);
    Marker marker1 = GetMarkerOfCategory(markers, Border1);
    vector<Point2f> m0c = marker0.corners;
    vector<Point2f> m1c = marker1.corners;

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

Marker ImageProcessor::GetMarkerOfCategory(vector<Marker> markers, MarkerCategory category)
{
    for (std::size_t i = 0; i != markers.size(); i++)
    {
        if (markers[i].category == category)
        {
            return markers[i];
        }
    }
    throw out_of_range("Marker with id:" + to_string((int)category) + "is not in collection.");
}