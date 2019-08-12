#ifndef _SCENE_H
#define _SCENE_H

#include "common.h"
#include "entity.h"
#include "unit.h"
#include "tower.h"


/**
 * Scene holds all entities to be renderered, event aware
 * (Singleton pattern)
 */
class Scene : 
    public EventHandler<MarkersDetectedEvent>
//                ,public EventHandler<UnitKilledEvent>
{

    CamPosition _corner_ul;
    CamPosition _corner_br;

  EventRegistration* _registration;
  Scene() {
      _registration = EventBus::AddHandler(*this);
  }
  Scene(Scene const&) = delete;
  void operator=(Scene const&) = delete;

  std::list<Entity*> _entities;

protected:
  void OnEvent(MarkersDetectedEvent& e);
  //void OnEvent(UnitKilledEvent& e);

public:
  static Scene& GetInstance() {
    static Scene instance;
    return instance;
  }

  ~Scene() {
      // remove all entities
      for (auto* ent : _entities) {
          delete ent;
      }
      // remove event handlers
      _registration->RemoveHandler();
  }

  // todo: somehow merge with other update/render methods?
  void Update(double delta);
  void Render(Renderer* renderer);

  /**
   * Adds entity to scene (make sure entity doesn't get destroyed preliminary)
   */
  void AddEntity(Entity* entity) {
    _entities.push_back(entity);
  }

  // todo
  //void RemoveEntity(Entity& entity) {
  //  _entities.remove(entity);
  //}

  std::list<Entity*> GetEntities() {
    return _entities;
  }

  /**
   * Return entities of type T
   */
  template <class T>
  std::list<T*> GetEntities() {
    std::list<T*> matches;

    T* c;
    for (Entity* ent : _entities) {
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
