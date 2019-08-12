#include "tower.h"
#include "unit.h"
#include "scene.h"

#include <iostream>


float Tower::DistanceTo(Unit* unit) {
    return norm(this->GetPosition() - unit->GetPosition());
}

bool Tower::Hits(Unit* unit) {
    std::cout << this->DistanceTo(unit) << std::endl;
    if (this->range > this->DistanceTo(unit))
        return true;

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
            break; // only damage first unit in sight
        }
    }
}

void Tower::Render(Renderer* renderer) {
    if(!on_screen) {
        std::cout << "adding: " << this->GetName() << std::endl;
        renderer->AddEntity(this->GetName(), this->GetMeshName());
        on_screen = true;
    }
    for (Unit* unit : Scene::GetInstance().GetEntities<Unit>()) {
        if (this->Hits(unit)) {
            renderer->DrawLine(this->GetPosition(), unit->GetPosition());
            break; // only damage first unit in sight
        }
    }

    renderer->SetEntityPosition(this->GetName(), this->GetPosition(), Vec3d(1.3, 0.0, 0.0));//this->GetRotation());
    //std::cout << this->GetPosition() << std::endl;
  // render projectile
}

void Tower::OnEvent(MarkersDetectedEvent& e) {
    Marker m = e.GetMarker();

    if (m.category == _id) {
        if (this->IsPlacementAllowed()) {
            this->SetPosition(m.tvec, m.rvec);
        }
    }
}
