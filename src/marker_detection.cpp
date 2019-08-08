#include "extern.h"

void MarkerDetection::Detect(Mat img) {
    std::vector<Marker> markers = _imgproc.DetectMarkers(img);
  // TODO : condition that markers got detected
  //if (false) {
    for (auto& m : markers) {
        // todo: filter non markers
        MarkersDetectedEvent event(m);
        EventBus::FireEvent(event);
    }
  //}
}
