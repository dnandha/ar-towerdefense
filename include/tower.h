/**
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

#ifndef _TOWER_H
#define _TOWER_H

#include "entity.h"

class Unit;

/**
 * Describes general tower game object
 */
class Tower : public Entity, public EventHandler<MarkersDetectedEvent> {
  EventRegistration* _registration;

  int _type;
  float _fov;
  double _time;
  bool _allow_placement;
  bool _freeze = false;
  Marker _lastDetectedTowerMarker;
  cv::Vec3d _relativeTranslation;

 protected:
  void OnEvent(MarkersDetectedEvent& e);

  float DistanceTo(Unit* unit);
  bool Hits(Unit* unit);

 public:
  float damage = 10;  // todo: make virtual and override in each sub class
  float range = 20;
  float cooldown = 1;  // in seconds

  Tower(int id, const std::string& name, const std::string& meshname)
      : Entity(id, name, meshname) {
    _registration = EventBus::AddHandler(*this);
  }

  ~Tower() {
    // remove event handlers
    _registration->RemoveHandler();
  }

  void AllowPlacement(bool allow) { _allow_placement = allow; }
  void SetFreeze(){_freeze = true;}
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
  DragonTower(int subid, const std::string& name)
      : Tower(6 + subid, name, "Medieval_tower_low.mesh"){};
};

#endif
