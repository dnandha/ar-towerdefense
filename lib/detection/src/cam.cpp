#include "cam.h"

Cam::Cam(int camId) {
  cout << "INIT CAMERA..." << endl;

  int cameraId = 1;
  int width = 1280;
  int height = 720;

  // Using GSTREAMER backend
  std::ostringstream desc;
  desc << "v4l2src device=/dev/video" << cameraId << " ! videoconvert"
       << " ! video/x-raw,width=" << width << ",height=" << height
       << " ! appsink";
  _cap.open(desc.str());

  if (!_cap.isOpened()) {
    cout << "CAMERA INIT FAILED." << endl;
    return;
  }

  cout << "\tCAMERA ID\t\t: " << cameraId << endl;
  cout << "\tCAMERA RESOLUTION\t: " << width << "x" << height << endl;

  cout << "CAMERA INIT SUCCESSFUL.\n";
}

bool Cam::Grab() { return _cap.grab(); }

cv::Mat Cam::GetFrame() {
  cv::Mat image;
  _cap.retrieve(image);

  cv::waitKey(10);
  return image;
}