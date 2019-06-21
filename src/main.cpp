#include "game.h"
#include "events.h"

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

struct MarkerHandler : EventHandler<MarkersDetectedEvent> {
  void OnEvent(MarkersDetectedEvent& e) {
    if (e.GetMockArg() == "mock") {
      std::cout << "mock event received" << std::endl;
    }
  }
};

/*
 * Main
 */
int main() {
  Player player("Mongo");
  Game game(&player);
  MarkerDetection detector;
  MarkerHandler markerhandler;
  EventBus::GetInstance()->AddHandler(markerhandler);

  game.Generate();
  game.Start();

  double start_time = std::time(nullptr);
  while (game.state != State::Ended) {
    detector.Detect();
    for (GameEntity* ent : Scene::GetInstance().GetEntities()) {
      ent->Update((std::time(nullptr) - start_time)/1000.0);
    }
  }

  return 1;
}
