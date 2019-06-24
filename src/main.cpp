#include "game.h"
#include "entities.h"
#include "scene.h"
#include "events.h"
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
    Renderer renderer(winsize, {"packs/Sinbad.zip"});

    // create game objects
    Player player("Mongo");
    Game game(&player);
    MarkerDetection detector;

    // spawn mobs using spawner
    Spawner<MobSinbad> mobspawner;
    mobspawner.Spawn(10);

    // generate and start game
    game.Generate();
    game.Start();

    // start time and enter game loop
    double start_time = std::time(nullptr);
    while (game.state != State::Ended && renderer.WaitKey(1) != 27) {
        // detect markers
        detector.Detect();
        // update and render scene
        for (Unit* ent : Scene::GetInstance().GetEntities<Unit>()) {
            ent->Update((std::time(nullptr) - start_time)/1000.0);
            ent->Render(&renderer);
        }
    }

    return 1;
}
