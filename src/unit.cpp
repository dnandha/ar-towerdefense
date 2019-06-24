#include "entities.h"

#include <iostream>


bool Unit::HasReachedEnd()
{
  return false;
}

void Unit::Kill() {
  // play death animation
  // blood!!!
  // call desctructor
}

void Unit::Update(double delta) {
  if (this->IsDead()) {
  } else if (this->HasReachedEnd()) {
  } else {
    // get position from pathfinding
    // _pf->getPosition(id, delta)
  }
}

void Unit::Render(Renderer* renderer) {
    if(!on_screen) {
        std::cout << "adding: " << this->GetName() << std::endl;
        renderer->AddEntity(this->GetName(), this->GetMeshName());
        renderer->SetEntityPosition(this->GetName(), this->GetPosition());
        renderer->SetEntityAnimation(this->GetName(), "RunBase");
        renderer->SetEntityAnimation(this->GetName(), "RunTop");
        on_screen = true;
    }
}
