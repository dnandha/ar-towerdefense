/*
* ARTD (Augmented Reality Tower Defense)
* Copyright (C) 2019 Jaeger,Stegmueller,Boche,Nandha 
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _COMMAND_H
#define _COMMAND_H

/**
 * User interaction
 * (Command pattern)
 */
class Command {
  public:
    Command() {}
    Tower PlaceTower(int id);
    void BuildTower(Tower tower);
    void DestructTower(Tower tower);
    void StartGame();
    void PauseGame();
    void EndGame();
};

#endif
