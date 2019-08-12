/**
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

#ifndef _UI_H
#define _UI_H

#include "common.h"
#include "game.h"
#include "extern.h"

/**
 * Class represents UI for stats
 */
class UI
{
    Game* _game; // game contains all the stats
public:
    UI(Game *game) : _game(game) {}

    void Render(Renderer* renderer);
};

#endif
