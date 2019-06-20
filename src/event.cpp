#include "event_bus.h"

Event::Event() { _canceled = false; }

bool Event::GetCanceled() { return _canceled; }

void Event::SetCanceled(bool canceled) { _canceled = canceled; }