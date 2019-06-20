#include "event_bus.h"

/* Null, because instance will be initialized on demand. */
EventBus *EventBus::_instance = nullptr;

EventBus *EventBus::GetInstance() {
  if (_instance == nullptr) {
    _instance = new EventBus();
  }

  return _instance;
}

EventBus::EventBus() {}

void EventBus::FireEvent(Event &e) {
  EventBus *instance = GetInstance();

  Registrations *registrations = instance->_handlers[typeid(e)];

  // If the registrations list is null, then no handlers have been registered
  // for this event
  if (registrations == nullptr) {
    return;
  }

  // Iterate through all the registered handlers and dispatch to each one if the
  // sender matches the source or if the sender is not specified
  for (auto &reg : *registrations) {
    // This is where some magic happens. The void * handler is statically cast
    // to an event handler of generic type Event and dispatched. The HandleEvent
    // function will then do a dynamic cast to the correct event type so the
    // matching OnEvent method can be called
    static_cast<EventHandler<Event> *>(reg->GetHandler())->HandleEvent(e);
  }
}