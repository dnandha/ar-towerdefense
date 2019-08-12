#ifndef _TOWER_H
#define _TOWER_H

#include "entity.h"


class Unit;

/**
 * Describes general tower game object
 */
class Tower : public Entity,
    public EventHandler<MarkersDetectedEvent>
{
  EventRegistration* _registration;

  int _type;
  float _fov;
  double _time;
  bool _allow_placement;

protected:
  void OnEvent(MarkersDetectedEvent& e);

  float DistanceTo(Unit* unit);
  bool Hits(Unit* unit);

public:
  float damage = 10; // todo: make virtual and override in each sub class
  float range = 10;
  float cooldown = 1; // in seconds

  Tower(int id, const std::string& name, const std::string& meshname) :
    Entity(id, name, meshname) {
      _registration = EventBus::AddHandler(*this);
    }

  ~Tower() {
      // remove event handlers
      _registration->RemoveHandler();
  }

  void AllowPlacement(bool allow) { _allow_placement = allow; }
  bool IsPlacementAllowed() { return _allow_placement; }

  void Build();
  void Destroy();

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
