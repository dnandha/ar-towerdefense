#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "game.h"

TEST_CASE( "Successfully tested game", "[gametest]" ) {
  Player _player("Mongo");
  Game _game(&_player);

  SECTION ("starting game") {
    _game.Start();
    REQUIRE(!_game.HasEnded());
    REQUIRE(!_game.IsPaused());
  }

  SECTION ("pausing game") {
    _game.Pause();
    REQUIRE(!_game.HasEnded());
    REQUIRE(_game.IsPaused());
  }

  SECTION ("resuming game") {
    _game.Resume();
    REQUIRE(!_game.HasEnded());
    REQUIRE(!_game.IsPaused());
  }

  SECTION ("ending game") {
    _game.End();
    REQUIRE(_game.HasEnded());
    REQUIRE(!_game.IsPaused());
  }
}
