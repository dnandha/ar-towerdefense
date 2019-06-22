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

class Scene; // forward declaration

/*
 * Each object should be able to update/render itself
 * (Visitor pattern)
 */
struct SceneEntity
{
  virtual void Update(double delta) {};
  virtual void Render(/*Window*/) {};
};

/*
 * Holds mesh data
 */
struct MeshEntity
{
  const char *mesh_name;
  const char *pack_name;
};

/*
 * Abstract object which represents game entity
 */
class GameEntity : SceneEntity
{
  int _id;
  const char *_name;
  MeshEntity* _mesh;
  Position _pos;

public:
  GameEntity(int id, const char *name, MeshEntity* mesh) :
    _id(id), _name(name), _mesh(mesh) {}

  virtual void Update(double delta) {};
  virtual void Render(/*Window*/) {};
};


/*
 * Describes the walking path of a unit
 * todo: put in another file or makunit
 */
class Unit : GameEntity
{
  PathFinding* _pf;

  float _damagetaken;

public:
  float walkspeed = 0.5; // pixels per ms
  float hitpoints = 100;

  Unit(int id, const char *name, MeshEntity* mesh, PathFinding* pf) :
    GameEntity(id, name, mesh), _damagetaken(.0f), _pf(pf) {}

  //void Walk(); // pf contains walking instructions
  //void Stop();
  //void LookAt(Position lookAt);
  //

  void Kill();

  bool HasReachedEnd();

  void TakeDamage(float dmg) { _damagetaken += dmg; }
  bool IsDead() { return _damagetaken >= this->hitpoints; }

  void Update(double delta);
  void Render(/*Window*/);
};

/*
 * Describes tower game objects
 */
class Tower : GameEntity
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
  void Render(/*Window*/);
};

/*
 * Unit spawner
 */
template <class T>
class Spawner 
{
public:
  T Spawn();
  void Start(double interval);
  void Stop();
};

template<class Unit>
Unit Spawner<Unit>::Spawn() {
  Unit unit(0, "orcan", nullptr, nullptr);

  return unit;
}

#endif
