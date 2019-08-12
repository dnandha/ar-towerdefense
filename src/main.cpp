/**
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

#include "game.h"
#include "unit.h"
#include "tower.h"
#include "scene.h"
#include "event_bus.hpp"
#include "spawner.hpp"

#include "ui.h"

#include "paths_initializer.h"

#include <iostream>
#include <ctime>

/**
 * Main Loop
 */
int main(int argc, char** argv) {
    int webcamId = 0;
    if (argc > 1)
        webcamId = atoi(argv[1]);

    // create rendering window
    WinSize winsize = {800, 600};
    Renderer renderer(winsize, {"packs/Sinbad.zip", "packs/tower.zip"});

    // create game objects
    Player player("Mongo");
    Game game(&player);
    UI ui(&game);

    // detection and cam
    MarkerDetection detector;
    Cam cam(webcamId);

    // set camera instrinsics in renderer
    renderer.SetCamera(detector.GetCamMatrix(), detector.GetDistCoeffs());

    // path finding
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
    towerspawner.Spawn(4); // spawn 4 dummy towers in valhalla

    double delta;
    // enter game loop
    while (game.state != GameBase::State::Ended
            && renderer.WaitKey(1) != 27
            && cam.Grab()) {

        delta = Clock::GetInstance().Tick();

        // get frame
        Mat img = cam.GetFrame();
        renderer.UpdateBackground(img, false); // todo: put into scene::update?

        if (game.state != GameBase::State::Paused) {
            // detect markers
            detector.Detect(img);
            // update and render scene
            Scene::GetInstance().Update(delta);
            Scene::GetInstance().Render(&renderer);
            // draw ui
            ui.Render(&renderer);
        }
    }

    return 1;
}
