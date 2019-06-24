#ifndef _SCENE_H
#define _SCENE_H

#include "common.h"
#include "entities.h"

/*
 * Level/GameState/Scene
 * Uses Singleton pattern
 */
class Scene : public EventHandler<MarkersDetectedEvent>
{
  EventRegistration* _registration;
  Scene() {
      _registration = EventBus::AddHandler(*this);
  }

  Scene(Scene const&) = delete;
  void operator=(Scene const&) = delete;

  std::list<GameEntity*> _gameEntities;

protected:
  void OnEvent(MarkersDetectedEvent& e);

public:
  static Scene& GetInstance() {
    static Scene instance;
    return instance;
  }

  ~Scene() {
      // remove all entities
      for (auto* ent : _gameEntities) {
          delete ent;
      }
      // remove event handlers
      _registration->RemoveHandler();
  }

  /**
   * Adds entity to scene (make sure entity doesn't get destroyed preliminary)
   *
   */
  void AddEntity(GameEntity* entity) {
    _gameEntities.push_back(entity);
  }

  //void RemoveEntity(GameEntity& entity) {
  //  _gameEntities.remove(entity);
  //}

  std::list<GameEntity*> GetEntities() {
    return _gameEntities;
  }

  template <class T>
  std::list<T*> GetEntities() {
    std::list<T*> matches;

    T* c;
    for (GameEntity* ent : _gameEntities) {
      if ((c = dynamic_cast<T*>(ent)) != nullptr) {
        matches.push_back(c);
      }
    }

    return matches;
  }

  void Save(const char *filename);
  void Load(const char *filename);
};

#endif
