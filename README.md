# ARTD
Augemented Reality Tower Defense - 
Classical tower defense game played on a sheet of paper with markers

## Requirements
* OpenCV 4 build with extra modules `Ovis` and `Aruco`
* Ogre 1.12

## Build
1. `git clone https://github.com/dnandha/ar-towerdefense`
1. `cd ar-towerdefense`
1. `mkdir build && cd build`
1. `cmake . && make`

## Command line arguments
Programm can be launched with additional parameter to specify camera id.
`./artd [<webcam-id>]`

## Gameplay

* Mount webcam on tripod or stand looking down to table.
* Print board (found in `resources/board.png`)
* Print markers and cut out (found in `resources/markers.png`)
* Place printed board on table under webcam
* Start program (see above)
* Webcam should capture board and present found path in another window
* Accept detected paths with marker 2, decline and repeat with marker 3
* Game window with status text pops up
* Place towers with markers 6, 7, 8, 9 (maybe try with few first)
* Start game with marker 3 (freezes towers and starts spawning units)
* Follow your performance by checking your score and remaining lives
* Pause game with marker 4 or prematurely end game with marker 5

## License
See `LICENSE.txt`
