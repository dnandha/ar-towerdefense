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

#include "tower.h"
#include "scene.h"
#include "unit.h"

#include <iostream>

float Tower::DistanceTo(Unit* unit) {
  return norm(this->GetPosition() - unit->GetPosition());
}

bool Tower::Hits(Unit* unit) {
  std::cout << this->DistanceTo(unit) << std::endl;
  if (this->range > this->DistanceTo(unit)) return true;

  return false;
}

void Tower::Update(double delta) {
  this->_time += delta;
  if (this->_time < this->cooldown) return;
  this->_time = 0.0;

  for (Unit* unit : Scene::GetInstance().GetEntities<Unit>()) {
    if (unit->IsDead()) continue;
    if (this->Hits(unit)) {
      std::cout << "hitting: " << unit->GetName() << std::endl;
      unit->TakeDamage(this->damage);
      break;  // only damage first unit in sight
    }
  }
}

void Tower::Render(Renderer* renderer) {
  if (!on_screen) {
    std::cout << "adding: " << this->GetName() << std::endl;
    renderer->AddEntity(this->GetName(), this->GetMeshName());
    renderer->SetEntityScale(this->GetName(), this->GetScale());
    on_screen = true;
  }
  for (Unit* unit : Scene::GetInstance().GetEntities<Unit>()) {
    if (this->Hits(unit)) {
      renderer->DrawLine(this->GetPosition(), unit->GetPosition());
      break;  // only damage first unit in sight
    }
  }
  renderer->SetEntityPosition(this->GetName(), this->GetPosition(),
                              this->GetRotation());
  // std::cout << this->GetPosition() << std::endl;
  // render projectile
}

void Tower::OnEvent(MarkersDetectedEvent& e) {
  Marker m = e.GetMarker();

  if (m.category == _id) {
    if (this->IsPlacementAllowed()) {
      this->SetPosition(m.tvec, m.rvec);
      _lastDetectedTowerMarker = m;
      _gotDetected = true;
    }
  }
  if (m.category == Border0) {
    if (!this->IsPlacementAllowed() && _gotDetected) {
      if (_freeze) {
        _relativeTranslation = _lastDetectedTowerMarker.tvec - m.tvec;
        _freeze = false;
      }
      this->SetPosition(m.tvec + _relativeTranslation,
                        _lastDetectedTowerMarker.rvec);
    }
  }
}
