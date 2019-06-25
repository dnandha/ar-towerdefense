#include "unit.h"

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
    Position pos = {5.0, 5.0, 5.0};
    this->SetPosition(pos);
  } else if (this->HasReachedEnd()) {
  } else {
    // get position from pathfinding
    // _pf->getPosition(id, delta)
    Position pos = {0.0, 0.0, this->GetPosition()[2] + this->walkspeed * -delta};
    this->SetPosition(pos);
  }
}

void Unit::Render(Renderer* renderer) {
    if(!on_screen) {
        std::cout << "adding: " << this->GetName() << std::endl;
        renderer->AddEntity(this->GetName(), this->GetMeshName());
        renderer->PlayEntityAnimation(this->GetName(), "RunBase");
        renderer->PlayEntityAnimation(this->GetName(), "RunTop");
        on_screen = true;
    }

    renderer->SetEntityPosition(this->GetName(), this->GetPosition());
    if (this->IsDead()) {
        renderer->StopEntityAnimation(this->GetName(), "RunBase");
        renderer->StopEntityAnimation(this->GetName(), "RunTop");
        // todo: death animation! blooodd!!!
        //renderer->RemoveEntity(this->GetName());
    }
}
