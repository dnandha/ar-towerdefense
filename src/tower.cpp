#include "entities.h"
#include "scene.h"


void Tower::Update(double delta) {
  for (Unit* unit : Scene::GetInstance().GetEntities<Unit>()) {
      if (this->Hits(unit)) {
        float dmg = this->GetDamage(delta);
        unit->TakeDamage(dmg);
      }
  }
}

void Tower::Render(Renderer* renderer) {
  // if not exists in scene, draw into scene:
  //        win->createEntity("myname", "mymesh")
  // win->setEntityPose("myname", "myposition")
  
  // render projectile
}
