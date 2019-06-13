#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "image_processor.h"

TEST_CASE("ImageProcessor detect marker 0", "[camtest]")
{
    ImageProcessor _improc;
    Mat _image;

    SECTION("read test image")
    {
        _image = imread("test_images/marker0.jpg");
    }

    SECTION("detect marker 0")
    {
        vector<Marker> markers = _improc.DetectMarkers(_image);
        REQUIRE(markers.size() == 1);
        REQUIRE(markers[0].id == 0);
    }
}
