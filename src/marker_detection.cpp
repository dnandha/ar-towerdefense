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
