#ifndef _ENTITIES_H
#define _ENTITIES_H

#include "common.h" 

#include "extern.h"

/*
 * Describes the walking path of a unit
 * todo: put in another file or make entity type
 */
struct Path {
  Position points;
};

class Scene; // forward declaration

/*
 * Each object should be able to update/render itself
 * (Visitor pattern)
 */
struct SceneEntity {
  virtual void Update(double delta, Scene scene);
  virtual void Render(/*Window*/);
};

/*
 * Holds mesh data
 */
struct MeshEntity {
  const char* mesh_name;
  const char* pack_name;
};

/*
 * Abstract object which represents game entity
 */
class GameEntity : SceneEntity {
  int _id;
  const char* _name;
  MeshEntity* _mesh;
  Position _pos;
  public:
    GameEntity(int id, const char* name, MeshEntity* mesh) : _id(id), _name(name), _mesh(mesh) {}
};

/*
 * Level/GameState/Scene
 */
class Scene : SceneEntity {
  std::list<GameEntity> _gameEntities;

  public:
    void Save(const char* filename);
    void Load(const char* filename);
};


/*
 * Enemy unit
 */
class Unit : GameEntity {
  PathFinding* _pf;

  float _damagetaken;
  float _hitpoints;

  public:
    Unit(int id, const char* name, int hp, MeshEntity* mesh, PathFinding* pf) : 
      GameEntity(id, name, mesh), _hitpoints(hp), _damagetaken(.0f), _pf(pf) {}

    void Walk(); // pf contains walking instructions
    void Stop();
    //void LookAt(Position lookAt);
    //
    bool HasReachedEnd();

    void Kill();

    void TakeDamage(int dmg);
    bool IsDead() { return _damagetaken >= _hitpoints; }
};

/*
 * Describes tower game objects
 */
class Tower : GameEntity { 
  int _type;
  float _damage;
  float _range;
  float _fov;
  
  public:
    void Build();
    void Destroy();

    bool Hits(Unit unit);

    float GetDamage() { return _damage; }
};

/*
 * Unit spawner
 */
template<class T>
class Spawner : GameEntity {
  T _type;
  public:
    void Start();
    void Stop();
};

#endif
