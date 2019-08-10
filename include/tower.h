#ifndef _TOWER_H
#define _TOWER_H

#include "entity.h"


class Unit;

/**
 * Describes tower game objects
 */
class Tower : public Entity,
    public EventHandler<MarkersDetectedEvent>
{
  EventRegistration* _registration;

  int _type;
  float _fov;
  double _time;

protected:
  void OnEvent(MarkersDetectedEvent& e);

public:
  float damage = 10;
  float range = 100; // in pixels
  float cooldown = 1; // in seconds

  Tower(int id, const std::string& name, const std::string& meshname) :
    Entity(id, name, meshname) {
      _registration = EventBus::AddHandler(*this);
    }

  ~Tower() {
      // remove event handlers
      _registration->RemoveHandler();
  }

  void Build();
  void Destroy();

  bool Hits(Unit* unit);

  float GetDamage(double delta) { return this->damage; }

  void Update(double delta);
  void Render(Renderer* renderer);
};

// SPECIFIC TOWERS
/**
 * Tower which fires single projectiles
 */
struct DragonTower : public Tower {
    DragonTower(int subid, const std::string& name) :
        Tower(6+subid, name, "Medieval_tower_low.mesh") {};
};

#endif
