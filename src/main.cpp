#include "game.h"
#include "unit.h"
#include "tower.h"
#include "scene.h"
#include "event_bus.hpp"
#include "spawner.hpp"

#include <iostream>
#include <ctime>

/*
 * Init Game
 */
void initialize() {
  // init cv
  // init pathfinding
  // generate path
  // load meshes
}

/*
 * Main
 */
int main() {
    // create rendering window
    Size2i winsize(800, 600);
    Renderer renderer(winsize, {"packs/Sinbad.zip", "packs/dragon.zip"});

    // create game objects
    Player player("Mongo");
    Game game(&player);
    MarkerDetection detector;

    // spawn mobs using spawner
    Spawner<MobSinbad> mobspawner;
    Scene::GetInstance().AddEntity(&mobspawner);

    // spawn tower // todo: spawner wasn't intended for non-unit types
    Spawner<DragonTower> towerspawner;
    towerspawner.Spawn();

    // generate and start game
    game.Generate();
    game.Start();

    double delta;
    // enter game loop
    while (game.state != State::Ended && renderer.WaitKey(1) != 27) {
        delta = Clock::GetInstance().Tick();
        // detect markers
        detector.Detect();
        // update and render scene
        for (auto* ent : Scene::GetInstance().GetEntities()) {
            ent->Update(delta);
            ent->Render(&renderer);
        }
    }

    return 1;
}
