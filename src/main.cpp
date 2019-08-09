#include "game.h"
#include "unit.h"
#include "tower.h"
#include "scene.h"
#include "event_bus.hpp"
#include "spawner.hpp"

#include "paths_initializer.h"

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
    WinSize winsize = {800, 600};
    Renderer renderer(winsize, {"packs/Sinbad.zip", "packs/tower.zip"});

    // create game objects
    Player player("Mongo");
    Game game(&player);
    MarkerDetection detector;
    Cam cam(2); // webcam

    // path detection
    PathsInitializer initializer(cam);
    std::vector<std::vector<cv::Point2f>> paths = initializer.InitializePaths();
    std::vector<AbsolutePath*> abspaths;
    for (auto& path : paths) {
        AbsolutePath* abspath = new AbsolutePath(path);
        abspaths.push_back(abspath);
    }

    // spawn mobs using spawner
    Spawner<MobSinbad> mobspawner(3.0);
    mobspawner.SetPaths(abspaths);
    Scene::GetInstance().AddEntity(&mobspawner);

    // spawn tower // todo: spawner wasn't intended for non-unit types
    Spawner<DragonTower> towerspawner;
    towerspawner.Spawn(4);

    // generate and start game
    game.Generate();
    game.Start();

    double delta;
    // enter game loop
    while (game.state != State::Ended && renderer.WaitKey(1) != 27 && cam.Grab()) {
        delta = Clock::GetInstance().Tick();
        // get frame
        Mat img = cam.GetFrame();
        renderer.UpdateBackground(img); // todo: put into scene::update?
        // detect markers
        detector.Detect(img);
        // update and render scene
        Scene::GetInstance().Update(delta);
        Scene::GetInstance().Render(&renderer);
    }

    return 1;
}
