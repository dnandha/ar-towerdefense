#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cam.h"

TEST_CASE("Testing cam", "[camtest]")
{
  Cam _cam(1);

  SECTION("take frame")
  {
    Mat frame = _cam.GetFrame();
    REQUIRE(frame.size().width > 0);
  }
}
