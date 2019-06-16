#ifndef _EVENT_H
#define _EVENT_H

#include <unordered_map>
#include <typeindex>
#include <list>

/*
 *
 */
class Event
{
public:
  Event();

  // DONT DELETE: virtual member is important for typeid operator behaviour
  virtual ~Event() {}

  bool GetCanceled();
  void SetCanceled(bool canceled);

private:
  bool _canceled;
};

/*
 *
 */
template <class T>
class EventHandler
{
public:
  EventHandler()
  {
    // An error here indicates you're trying to implement EventHandler with a type that is not derived from Event
    static_assert(std::is_base_of<Event, T>::value, "EventHandler<T>: T must be a class derived from Event");
  };
  virtual void OnEvent(T &) = 0;
  void HandleEvent(Event &e)
  {
    OnEvent(dynamic_cast<T &>(e));
  };
};

class EventRegistration
{
public:
  typedef std::list<EventRegistration *> Registrations;

  EventRegistration(void *const handler, Registrations *const registrations) : _handler(handler),
                                                                               _registrations(registrations),
                                                                               _registered(true)
  {
  }

  /**
		 * Gets the event handler for this registration
		 * @return The event handler
		 */
  void *const GetHandler()
  {
    return _handler;
  }

  /**
		 * Removes an event handler from the registration collection
		 * The event handler will no longer receive events for this event type
		 */
  void RemoveHandler()
  {
    if (_registered)
    {
      _registrations->remove(this);
      _registered = false;
    }
  }

private:
  void *const _handler;
  Registrations *const _registrations;
  bool _registered;
};

/*
 *
 */
class EventBus
{
private:
  static EventBus *_instance;
  typedef std::list<EventRegistration *> Registrations;
  typedef std::unordered_map<std::type_index, std::list<EventRegistration *> *> TypeMap;

  TypeMap _handlers;

  /* Private constructor to prevent instancing. */
  EventBus();

public:
  static EventBus *GetInstance();

  template <class T>
  static EventRegistration *const AddHandler(EventHandler<T> &handler)
  {
    EventBus *instance = GetInstance();

    // Fetch the list of event pairs unique to this event type
    Registrations *registrations = instance->_handlers[typeid(T)];

    // Create a new collection instance for this type if it hasn't been created yet
    if (registrations == nullptr)
    {
      registrations = new Registrations();
      instance->_handlers[typeid(T)] = registrations;
    }

    // Create a new EventPair instance for this registration.
    // This will group the handler, sender, and registration object into the same class
    EventRegistration *registration = new EventRegistration(static_cast<void *>(&handler), registrations);

    // Add the registration object to the collection
    registrations->push_back(registration);

    return registration;
  }

  static void FireEvent(Event &e);
};

#endif
