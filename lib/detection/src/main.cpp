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
        Mat processed_image = improc.Process(image);
        ShowImage(processed_image);
    }

    return 1;
}
