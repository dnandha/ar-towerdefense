#ifndef _UNIT_H
#define _UNIT_H

#include "entity.h"

/**
 * Describes the walking path of a unit
 * todo: put in another file or makunit
 */
class Unit : public Entity
{
  PathFinding* _pf;

  float _damagetaken;

public:
  float walkspeed = 2.0; // pixels per ms
  float hitpoints = 100;

  Unit(int id, const std::string& name, const std::string& meshname, PathFinding* pf) :
    Entity(id, name, meshname), _damagetaken(.0f), _pf(pf) {}

  //void Walk(); // pf contains walking instructions
  //void Stop();
  //void LookAt(Position lookAt);
  //

  void Kill();

  bool HasReachedEnd();

  void TakeDamage(float dmg) { _damagetaken += dmg; }
  bool IsDead() { return _damagetaken >= this->hitpoints; }

  void Update(double delta);
  void Render(Renderer* renderer);
};

// SPECIFIC UNITS
/**
 * Basic mob to grind
 */
struct MobSinbad : public Unit {
    MobSinbad(const std::string& name, PathFinding* pf) :
        Unit(0, name, "Sinbad.mesh", pf) {};
};

#endif
