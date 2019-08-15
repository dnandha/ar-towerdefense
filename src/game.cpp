/*
 * ARTD (Augmented Reality Tower Defense)
 * Copyright (C) 2019 Jaeger,Stegmueller,Boche,Nandha
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <ctime>

#include "game.h"
#include "scene.h"
#include "spawner.hpp"

void Game::Start() {
  GameBase::Start();

  // freeze towers
  for (auto* ent : Scene::GetInstance().GetEntities<DragonTower>()) {
    ent->AllowPlacement(false);
    ent->SetFreeze();
  }

  // let the battle begin, start spawning mobs
  for (auto* ent : Scene::GetInstance().GetEntities<Spawner<MobSinbad>>()) {
    ent->Start();
  }
}

void Game::Pause() { GameBase::Pause(); }
void Game::Resume() { GameBase::Resume(); }
void Game::End() {
  GameBase::End();

  for (auto* ent : Scene::GetInstance().GetEntities<Spawner<MobSinbad>>()) {
    ent->Stop();
  }
}

void Game::OnEvent(MarkersDetectedEvent& e) {
  Marker m = e.GetMarker();
  switch (m.category) {
    case 3:
      Start();
      break;
    case 4:
      if (state == State::Running) {
        Pause();
      } else if (state == State::Paused) {
        Resume();
      }
      break;
    case 5:
      End();
      break;
  }
}

void Game::OnEvent(GameEvent& e) {
  switch (e.GetType()) {
    case GameEvent::EventType::PlayerHit:
      this->GetPlayer()->TakeHit();
      break;
    case GameEvent::EventType::PlayerScore:
      this->GetPlayer()->ScorePlus();
      break;
  }
}
