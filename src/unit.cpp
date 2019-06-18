#include "game.h"


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

void Unit::Render(/*Window*/) {
}
