#include "cam.h"
#include "image_processor.h"

/*
 * Main
 */
int main() {
  try {
    Cam cam(1);
    ImageProcessor improc;

    while (cam.Grab()) {
      Mat image = cam.GetFrame();
      vector<Marker> markers = improc.DetectMarkers(image);

      Mat markerImage = improc.DrawMarkers(image, markers);
      imshow("marker image", markerImage);

      if (improc.ContainsBorderMarkers(markers)) {
        Mat warpedPaperImage = improc.WarpPaperImage(image, markers, 900, 600);
        imshow("warped image", warpedPaperImage);
      }
    }

    std::cout << "Cant grab camera image";
  } catch (std::exception &ex) {
    std::cerr << ex.what() << std::endl;
  } catch (...) {
    std::cerr << "Caught unknown exception." << std::endl;
  }

  return 1;
}
