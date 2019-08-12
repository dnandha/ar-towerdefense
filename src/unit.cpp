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

#include "unit.h"
#include "events.h"

void Unit::Kill() {
  // play death animation
  // blood!!!
  // call desctructor
}

void Unit::Update(double delta) {
  if (this->IsDead()) {
    // send to valhalla // todo
    this->SetPosition(Vec3d(500.0, 500.0, 500.0));

    if (!_mark_removed) {  // todo
      _mark_removed = true;

      // give player points once
      GameEvent e(GameEvent::EventType::PlayerScore);
      EventBus::FireEvent(e);
    }
  } else if (this->HasJustSpawned()) {
    this->SetPosition(_pf->GetPoint(0), _pf->GetRotation());
    _i_pos++;
  } else if (this->HasReachedEnd()) {
    // send to valhalla // todo
    this->SetPosition(Vec3d(500.0, 500.0, 500.0));

    // hurt player
    GameEvent e(GameEvent::EventType::PlayerHit);
    EventBus::FireEvent(e);
  } else {
    // get position from pathfinding
    for (_i_pos; _i_pos < _pf->Length(); ++_i_pos) {
      Vec3d dir = _pf->GetPoint(_i_pos) - this->GetPosition();
      Vec3d pos = this->GetPosition();
      if (norm(dir) > 1.0) {
        pos += dir * this->walkspeed * delta;
        this->SetPosition(pos);
        break;
      }
    }
  }
}

void Unit::Render(Renderer* renderer) {
  if (!on_screen) {
    std::cout << "adding: " << this->GetName() << std::endl;
    renderer->AddEntity(this->GetName(), this->GetMeshName(),
                        Vec3d(-1.3, 0.0, 0.0));
    renderer->PlayEntityAnimation(this->GetName(), "RunBase");
    renderer->PlayEntityAnimation(this->GetName(), "RunTop");
    on_screen = true;
  }

  renderer->SetEntityPosition(this->GetName(), this->GetPosition(),
                              this->GetRotation());
  if (this->IsDead()) {
    renderer->StopEntityAnimation(this->GetName(), "RunBase");
    renderer->StopEntityAnimation(this->GetName(), "RunTop");
    // todo: death animation! blooodd!!!
    // renderer->RemoveEntity(this->GetName());
  }
}
