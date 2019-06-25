#include "tower.h"
#include "unit.h"
#include "scene.h"

#include <iostream>


bool Tower::Hits(Unit* unit) {
    return true;
}

void Tower::Update(double delta) {
    if ((chrono::now() - this->_time)/1000.0 < this->cooldown) return;
    this->_time = chrono::now();

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
    renderer->SetEntityPosition(this->GetName(), this->GetPosition());
  // render projectile
}
