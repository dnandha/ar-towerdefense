/*
* ARTD (Augmented Reality Tower Defense)
* Copyright (C) 2019 Jaeger,Stegmueller,Boche,Nandha 
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _ENTITY_H
#define _ENTITY_H

#include "common.h"
#include "extern.h"

/**
 * Abstract class representing game entity
 */
class Entity
{
protected:
  int _id;
  std::string _name;
  std::string _meshname;
  Vec3d _pos;
  Vec3d _rot;
  float _scale;

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
  void SetScale(float scale) { _scale = scale; }

  Vec3d GetPosition() { return _pos; }
  Vec3d GetRotation() { return _rot; }
  float GetScale() { return _scale; }


  // each entity should be able to update and render itself (visitor pattern)
  virtual void Update(double delta) {};
  virtual void Render(Renderer* renderer) {};
};

#endif
