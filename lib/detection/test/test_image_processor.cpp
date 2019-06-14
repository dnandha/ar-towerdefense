#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "image_processor.h"

TEST_CASE("ImageProcessor detects marker 0", "[improctest]")
{
    ImageProcessor _improc;
    Mat _image;

    SECTION("detect marker 0")
    {
        _image = imread("test/marker0.jpg");
        vector<Marker> markers = _improc.DetectMarkers(_image);
        REQUIRE(markers.size() > 0);
        REQUIRE(markers[0].category == Border0);
    }
}

TEST_CASE("ImageProcessor detects no marker", "[improctest]")
{
    ImageProcessor _improc;
    Mat _image;

    SECTION("detect no marker")
    {
        _image = imread("test/no_marker.jpg");
        vector<Marker> markers = _improc.DetectMarkers(_image);
        REQUIRE(markers.size() == 0);
    }
}

TEST_CASE("ImageProcessor detects marker 0 and marker 1", "[improctest]")
{
    ImageProcessor _improc;
    Mat _image;

    SECTION("detect marker 0 and marker 1")
    {
        _image = imread("test/paper_borders.jpg");
        vector<Marker> markers = _improc.DetectMarkers(_image);
        REQUIRE(markers.size() == 2);

        Marker marker0 = _improc.GetMarkerOfCategory(markers, Border0);
        REQUIRE(marker0.category == Border0);

        Marker marker1 = _improc.GetMarkerOfCategory(markers, Border1);
        REQUIRE(marker1.category == Border1);
    }
}