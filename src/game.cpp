#include "game.h"


/*
 * Example Implementation
 */
void Game::Start() {
  if (_hasEnded || _isPaused) {
    _hasEnded = false;
    _isPaused = false;
  }
}

void Game::Loop() {
  while (!this->HasEnded()) {
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

std::list<Unit> Game::GetUnits() {
  return std::list<Unit>();
}

void Game::EventReceived(EventType event) {
}
