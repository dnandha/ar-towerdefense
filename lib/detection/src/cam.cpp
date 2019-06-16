#include "cam.h"

Cam::Cam(int camId)
{
    cout << "Init camera..." << std::endl;
    _cap.open(camId);
    if (!_cap.isOpened())
    {
        cout << "FAIL." << std::endl;
    }

    std::cout << "Camera init successful.\n";
    std::cout << "Setting cam parameters...\n";

    // std::cout << "\t[PARAM_FRAME_WIDTH] ";
    // if (!_cap.set(CAP_PROP_FRAME_WIDTH, 1280))
    // {
    //     std::cout << "SUCCESS\n";
    // }
    // else
    // {
    //     std::cout << "FAIL\n";
    // }

    // std::cout << "\t[PARAM_FRAME_HEIGHT] ";
    // if (!_cap.set(CAP_PROP_FRAME_HEIGHT, 960))
    // {
    //     std::cout << "SUCCESS\n";
    // }
    // else
    // {
    //     std::cout << "FAIL\n";
    // }

    std::cout << "\t[PARAM_FPS] ";
    if (!_cap.set(CAP_PROP_FPS, 15))
    {
        std::cout << "SUCCESS\n";
    }
    else
    {
        std::cout << "FAIL\n";
    }

    std::cout << "Setting cam parameters successful.\n";
}

bool Cam::Grab()
{
    return _cap.grab();
}

Mat Cam::GetFrame()
{
    Mat image;
    _cap.retrieve(image);
    waitKey(10);
    return image;
}