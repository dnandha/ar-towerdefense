#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "entities.h"


TEST_CASE( "Successfully tested unit", "[unittest]" ) {
  Spawner<Unit> _spawner;

  SECTION ("kill unit") {
    Unit unit = _spawner.Spawn();

    REQUIRE(!unit.IsDead());
    
    unit.TakeDamage(90);
    REQUIRE(!unit.IsDead());

    unit.TakeDamage(10);
    REQUIRE(unit.IsDead());
  }
}
