#include <ctime>

#include "game.h"


/*
 * Example Implementation
 */

void Game::Start() {}
void Game::Pause() {}
void Game::Resume() {}
void Game::End() {}
void Game::GenerateLevels() {}
void Game::Loop() {
  double start_time = std::time(nullptr);
  while (this->state != State::Ended) {
    for (GameEntity& ent : Scene::GetInstance().GetEntities()) {
      ent.Update(std::time(nullptr) - start_time);
    }
  }
}
