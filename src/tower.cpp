#include "game.h"


void Tower::Update(double delta) {
  for (Unit &unit : Scene::GetInstance().GetEntities<Unit>()) {
      if (this->Hits(unit)) {
	float dmg = this->GetDamage(delta);
	unit.TakeDamage(dmg);
      }
  }
}

void Tower::Render(/*Window*/) {
  // render projectile
}
