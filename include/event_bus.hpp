#ifndef _EVENT_BUS_H
#define _EVENT_BUS_H

#include "events.h"

#include <list>
#include <typeindex>
#include <unordered_map>

// Source: https://github.com/dquist/EventBus/

/**
 * EventHandler to be implemented by each listener
 */
template <class T>
class EventHandler {
 public:
  EventHandler() {
    // An error here indicates you're trying to implement EventHandler with a
    // type that is not derived from Event
    static_assert(std::is_base_of<Event, T>::value,
                  "EventHandler<T>: T must be a class derived from Event");
  };
  virtual void OnEvent(T &) = 0;
  void HandleEvent(Event &e) { OnEvent(dynamic_cast<T &>(e)); };
};

/**
 * Registration keeps track of all event handlers of specific type
 */
class EventRegistration {
 public:
  typedef std::list<EventRegistration *> Registrations;

  EventRegistration(void *const handler, Registrations *const registrations)
      : _handler(handler), _registrations(registrations), _registered(true) {}

  /**
   * Gets the event handler for this registration
   * @return The event handler
   */
  void *const GetHandler() { return _handler; }

  /**
   * Removes an event handler from the registration collection
   * The event handler will no longer receive events for this event type
   */
  void RemoveHandler() {
    if (_registered) {
      _registrations->remove(this);
      _registered = false;
    }
  }

 private:
  void *const _handler;
  Registrations *const _registrations;
  bool _registered;
};

/**
 * Singleton bus for event transmission
 */
class EventBus {
 private:
  typedef std::list<EventRegistration *> Registrations;
  typedef std::unordered_map<std::type_index, std::list<EventRegistration *> *>
      TypeMap;
  TypeMap _handlers;

  /* Private constructor to prevent instancing. */
  EventBus() {}
  EventBus(EventBus const&) = delete;
  void operator=(EventBus const&) = delete;

 public:

  static EventBus& GetInstance() {
      static EventBus instance;
      return instance;
  }

  template <class T>
  static EventRegistration *const AddHandler(EventHandler<T> &handler) {
    // Fetch the list of event pairs unique to this event type
    Registrations *registrations = GetInstance()._handlers[typeid(T)];

    if (registrations == nullptr) {
      registrations = new Registrations();
      GetInstance()._handlers[typeid(T)] = registrations;
    }

    EventRegistration* registration =
        new EventRegistration(static_cast<void*>(&handler), registrations);

    registrations->push_back(registration);

    return registration;
  }

  static void FireEvent(Event &e) {
      Registrations* registrations = GetInstance()._handlers[typeid(e)];

      if (registrations == nullptr) {
        return;
      }

      for (auto &reg : *registrations) {
        // This is where some magic happens. The void * handler is statically cast
        // to an event handler of generic type Event and dispatched. The HandleEvent
        // function will then do a dynamic cast to the correct event type so the
        // matching OnEvent method can be called
        static_cast<EventHandler<Event> *>(reg->GetHandler())->HandleEvent(e);
      }
  }
};

#endif
