#include "extern.h"

void MarkerDetection::Run() {
  // TODO : condition that build tower marker got detected
  if (false) {
    BuildTowerEvent buildTowerEvent("mock");
    EventBus::FireEvent(buildTowerEvent);
  }
}