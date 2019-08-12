/*
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

#include "extern.h"

void MarkerDetection::Detect(Mat img) {
    std::vector<Marker> markers = _imgproc.DetectMarkers(img);
  // TODO : condition that markers got detected
  //if (false) {
    Marker m0;
    Marker m1;
    bool m0avail = false;
    bool m1avail = false;
    for (auto& m : markers) {
        if (m.category == 0) {
            m0 = m;
            m0avail = true;
        } else if (m.category == 1) {
            m1 = m;
            m1avail = true;
        }

        // todo: filter non markers
        MarkersDetectedEvent event(m);
        EventBus::FireEvent(event);
    }

    if (m0avail && m1avail) {
        _imgproc.WarpPaperImage(img, markers, 900, 600);
        Mat homo = _imgproc.GetHomography();
        Mat camMat = _imgproc.GetCamMatrix();
        Mat distCoeffs = _imgproc.GetDistCoeffs();
        HomographyComputedEvent event(homo, camMat, distCoeffs, m0, m1);
        EventBus::FireEvent(event);
    }

  //}
}
