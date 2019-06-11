#include "cam.h"
#include "image_processor.h"

static void ShowImage(Mat image)
{
    if (image.empty())
    {
        cout << "Image empty!!";
    }
    else
    {
        imshow("out", image);
    }
}

/*
 * Main
 */
int main()
{
    Cam cam(1);
    ImageProcessor improc;

    while (cam.Grab())
    {
        Mat image = cam.GetFrame();
        vector<Marker> markers = improc.DetectMarkers(image);

        Mat markerImage = improc.DrawMarkers(image, markers);
        ShowImage(markerImage);

        // if (improc.ContainsBorderMarkers(markers))
        // {
        //     Mat warpedPaperImage = improc.WarpPaperImage(image, markers);
        //     ShowImage(warpedPaperImage);
        // }
    }

    return 1;
}
