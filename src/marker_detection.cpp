#include "extern.h"

void MarkerDetection::Detect() {
  // TODO : condition that markers got detected
  //if (false) {
    MarkersDetectedEvent markersDetectedEvent("mock");
    EventBus::FireEvent(markersDetectedEvent);
  //}
}
