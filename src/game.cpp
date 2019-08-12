#include <ctime>

#include "game.h"
#include "scene.h"
#include "spawner.hpp"

void Game::Start() {
    GameBase::Start();

    // let the battle begin, start spawning mobs
    for (auto* ent : Scene::GetInstance().GetEntities<Spawner<MobSinbad>>()) {
        ent->Start();
    }
    // freeze towers
    //for (auto* ent : Scene::GetInstance().GetEntities<DragonTower>()) {
    //    ent->AllowPlacement(false);
    //}
}

void Game::Pause() {
    GameBase::Pause();
}
void Game::Resume() {
    GameBase::Resume();
}
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
    switch(e.GetType()) {
        case GameEvent::EventType::PlayerHit:
            this->GetPlayer()->TakeHit();
            break;
        case GameEvent::EventType::PlayerScore:
            this->GetPlayer()->ScorePlus();
            break;
    }
}
