#include "tower.h"
#include "unit.h"
#include "scene.h"

#include <iostream>


bool Tower::Hits(Unit* unit) {
    return true;
}

void Tower::Update(double delta) {
    this->_time += delta; 
    if (this->_time < this->cooldown) return;
    this->_time = 0.0;

    for (Unit* unit : Scene::GetInstance().GetEntities<Unit>()) {
        if (unit->IsDead()) continue;
        if (this->Hits(unit)) {
            std::cout << "hitting: " << unit->GetName() << std::endl;
            float dmg = this->GetDamage(delta);
            unit->TakeDamage(dmg);
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
    renderer->SetEntityPosition(this->GetName(), this->GetPosition(), Vec3d(1.3, 0.0, 0.0));//this->GetRotation());
    //std::cout << this->GetPosition() << std::endl;
  // render projectile
}

void Tower::OnEvent(MarkersDetectedEvent& e) {
    Marker m = e.GetMarker();

    if (m.category == _id) {
        this->SetPosition(m.tvec, m.rvec);
    }
}
