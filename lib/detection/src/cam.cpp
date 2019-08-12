/**
* ARTD (Augmented Reality Tower Defense)
* Copyright (C) 2019 Jaeger,Stegmueller,Boche,Nandha 
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "cam.h"

Cam::Cam(int camId) {
  cout << "INIT CAMERA..." << endl;

  int width = 1280;
  int height = 720;

  // Using GSTREAMER backend
  std::ostringstream desc;
  desc << "v4l2src device=/dev/video" << camId << " ! videoconvert"
       << " ! video/x-raw,width=" << width << ",height=" << height
       << " ! appsink";
  _cap.open(desc.str());

  if (!_cap.isOpened()) {
    cout << "CAMERA INIT FAILED." << endl;
    return;
  }

  cout << "\tCAMERA ID\t\t: " << camId << endl;
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
