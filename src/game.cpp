#include "game.h"


/*
 * Example Implementation
 */
void Game::Run() {
  if (!this->_isRunning)
    this->_isRunning = true;

  while (_isRunning) {
    for (Unit& unit : this->GetUnits()) {
      if (unit.HasReachedEnd()) {
	unit.DeSpawn();
	this->_player->TakeHit();
      } else {
	unit.Walk();
	for (Tower& tower : this->GetTowers()) { 
	  if (tower.Hits(unit)) {
	    unit.TakeDamage(tower.GetDamage());
	    if (unit.IsDead()) { 
	      unit.DeSpawn();
	      this->_player->ScorePlus();
	    }
	  }
	}
      }
    }
  }
}
