#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "entities.h"


TEST_CASE( "Successfully tested unit", "[unittest]" ) {
  MobSinbad mob("orcan", nullptr);

  SECTION ("kill unit") {
    REQUIRE(!mob.IsDead());
    
    mob.TakeDamage(90);
    REQUIRE(!mob.IsDead());

    mob.TakeDamage(10);
    REQUIRE(mob.IsDead());
  }
}
