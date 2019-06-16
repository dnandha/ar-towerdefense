#ifndef _EVENT_H
#define _EVENT_H

/*
 *
 */
enum EventType {
  MarkerPlaceTower,
  MarkerBuildTower,
  MarkerDestroyTower,
  MarkerStartGame,
  MarkerPauseGame,
  NavMeshLoaded,
  NavStartDefined,
  NavEndDefined,
  NavRouteFound
};

/*
 *
 */
class EventListener {
  public:
    virtual void HandleEvent(EventType event) {};
};

/*
 *
 */
class EventBus {
  std::list<EventListener> _listeners;
  public:
    EventBus() {}
    void AddHandler(EventListener listener) { _listeners.push_back(listener); }
    void FireEvent(EventType event) { for (EventListener listener : _listeners) listener.HandleEvent(event); }
};

#endif
