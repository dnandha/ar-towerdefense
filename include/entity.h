#ifndef _ENTITY_H
#define _ENTITY_H

#include "common.h"
#include "extern.h"

/**
 * Abstract class which represents game entity
 */
class Entity
{
  int _id;
  std::string _name;
  std::string _meshname;
  Position _pos;

public:
  bool on_screen;

  Entity(int id, const std::string& name, const std::string& meshname) :
    _id(id), _name(name), _meshname(meshname), on_screen(false) {}

  std::string GetName() { return _name; }
  std::string GetMeshName() { return _meshname; }

  void SetPosition(Position pos) { _pos = pos; }
  Position GetPosition() { return _pos; }

  // each entity should be able to update and render itself (visitor pattern)
  virtual void Update(double delta) {};
  virtual void Render(Renderer* renderer) {};
};

#endif
