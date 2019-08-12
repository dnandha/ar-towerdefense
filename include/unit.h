#ifndef _UNIT_H
#define _UNIT_H

#include "entity.h"

/**
 * Describes general game unit/mob
 */
class Unit : public Entity
{
  AbsolutePath* _pf;

  float _damagetaken = 0.0;
  int _i_pos = 0; // counter for pathfinding position
  bool _mark_removed = false; // todo: not needed if unit truly removed upon death

public:
  float walkspeed = 2.0; // pixels per ms
  float hitpoints = 100;

  Unit(int id, const std::string& name, const std::string& meshname, AbsolutePath* pf) :
    Entity(id, name, meshname), _pf(pf) {}

  //void Walk(); // pf contains walking instructions
  //void Stop();
  //void LookAt(Position lookAt);
  //

  void Kill();

  bool HasJustSpawned() {return _i_pos == 0;}
  bool HasReachedEnd() {return _i_pos == _pf->Length();}

  void TakeDamage(float dmg) { _damagetaken += dmg; }
  bool IsDead() { return _damagetaken >= this->hitpoints; }

  void Update(double delta);
  void Render(Renderer* renderer);
};

// SPECIFIC UNITS
/**
 * Basic mob to grind xp
 */
struct MobSinbad : public Unit {
    MobSinbad(const std::string& name, AbsolutePath* pf) :
        Unit(0, name, "Sinbad.mesh", pf) {};
};

#endif
