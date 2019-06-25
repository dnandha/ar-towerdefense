#ifndef _TOWER_H
#define _TOWER_H

#include "entity.h"


class Unit;

/*
 * Describes tower game objects
 */
class Tower : public Entity
{
  int _type;
  float _fov;
  double _time;

public:
  float damage = 10;
  float range = 100; // in pixels
  float cooldown = 1; // in seconds

  Tower(int id, const std::string& name, const std::string& meshname) :
    Entity(id, name, meshname), _time(chrono::now()) {}

  void Build();
  void Destroy();

  bool Hits(Unit* unit);

  float GetDamage(double delta) { return this->damage; }

  void Update(double delta);
  void Render(Renderer* renderer);
};

// SPECIFIC TOWERS
/**
 *
 */
struct DragonTower : public Tower {
    DragonTower(const std::string& name) :
        Tower(100, name, "dragon.mesh") {};
};

#endif
