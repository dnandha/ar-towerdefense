#include "game.h"

/*
 * Global vars
 */
Player _player("Mongo");
Game _game(&_player);

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
  _game.Start();
  _game.Loop();

  return 1;
}
