#ifndef _ENTITIES_H
#define _ENTITIES_H

#include "common.h"

#include "extern.h"

/*
 * Describes the walking path of a unit
 * todo: put in another file or make entity type
 */
struct Path
{
  Position points;
};

/*
 * Abstract object which represents game entity
 */
class GameEntity
{
  int _id;
  std::string _name;
  std::string _meshname;
  Position _pos;

public:
  bool on_screen;

  GameEntity(int id, const std::string& name, const std::string& meshname) :
    _id(id), _name(name), _meshname(meshname), on_screen(false) {}

  std::string GetName() { return _name; }
  std::string GetMeshName() { return _meshname; }

  void SetPosition(Position pos) { _pos = pos; }
  Position GetPosition() { return _pos; }

  // each entity should be able to update and render itself (visitor pattern)
  virtual void Update(double delta) {};
  virtual void Render(Renderer* renderer) {};
};


/*
 * Describes the walking path of a unit
 * todo: put in another file or makunit
 */
class Unit : public GameEntity
{
  PathFinding* _pf;

  float _damagetaken;

public:
  float walkspeed = 0.5; // pixels per ms
  float hitpoints = 100;

  Unit(int id, const std::string& name, const std::string& meshname, PathFinding* pf) :
    GameEntity(id, name, meshname), _damagetaken(.0f), _pf(pf) {}

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

class MobSinbad : public Unit {
    public:
        MobSinbad(const std::string& name, PathFinding* pf) :
            Unit(0, name, "Sinbad.mesh", pf) {};
};

/*
 * Describes tower game objects
 */
class Tower : public GameEntity
{
  int _type;
  float _fov;

public:
  float range = 100; // in pixels
  float dpms = 10; // damage per ms

  void Build();
  void Destroy();

  bool Hits(Unit* unit);

  float GetDamage(double delta) { return this->dpms * delta; }

  void Update(double delta);
  void Render(Renderer* renderer);
};

#endif
