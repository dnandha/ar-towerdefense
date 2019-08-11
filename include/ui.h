#ifndef _UI_H
#define _UI_H

#include "common.h"
#include "game.h"
#include "extern.h"

/**
 * Class which represents UI for stats
 **/
class UI
{
    Game* _game; // game has all the stats
public:
    UI(Game *game) : _game(game) {}

    void Render(Renderer* renderer);
};

#endif
