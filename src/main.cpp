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
  _game.Run();

  return 1;
}
