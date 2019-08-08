#ifndef _ENTITY_H
#define _ENTITY_H

#include "common.h"
#include "extern.h"

/**
 * Abstract class which represents game entity
 */
class Entity
{
protected:
  int _id;
  std::string _name;
  std::string _meshname;
  Vec3d _pos;
  Vec3d _rot;

public:
  bool on_screen;

  Entity(int id, const std::string& name, const std::string& meshname) :
    _id(id), _name(name), _meshname(meshname), on_screen(false) {}

  int GetId() { return _id; }
  std::string GetName() { return _name; }
  std::string GetMeshName() { return _meshname; }

  void SetPosition(Vec3d pos) { _pos = pos; }
  void SetPosition(Vec3d pos, Vec3d rot) { _pos = pos; _rot = rot; }
  void SetRotation(Vec3d rot) { _rot = rot; }
  Vec3d GetPosition() { return _pos; }
  Vec3d GetRotation() { return _pos; }

  // each entity should be able to update and render itself (visitor pattern)
  virtual void Update(double delta) {};
  virtual void Render(Renderer* renderer) {};
};

#endif
