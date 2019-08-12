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

#ifndef _GAME_H
#define _GAME_H

#include <list>
#include <string>

#include "common.h"
#include "event_bus.hpp"

/**
 * Player information and stats
 */
class Player
{
  int _lives;
  int _score;
  const char *_name;

public:
  Player(const char *name, int lives = 10) : _name(name), _score(0), _lives(lives) {}

  std::string GetLivesString() { 
    std::ostringstream ss;
    ss << "Remaining lives: " << _lives;
    return ss.str();
  }
  std::string GetScoreString() { 
    std::ostringstream ss;
    ss << "Current score: " << _score;
    return ss.str();
  }
  std::string GetNameString() {
    std::ostringstream ss;
    ss << "Player: " << _name;
    return ss.str();
  }

  bool IsDead() {return _lives == 0;}

  void TakeHit() { _lives > 0 ? _lives-- : _lives = 0; }
  void ScorePlus() { _score++; }
};

/**
 * Main game class, puts it all together
 */
class GameBase
{
protected:
  Player* _player;

public:
    enum State{
      Init,
      Running,
      Paused,
      Ended
    };

    GameBase::State state;

    GameBase(Player* player) : _player(player), state(GameBase::State::Init) {}

    Player* GetPlayer() { return _player; }

    std::string GetStateString() {
        std::string statestr = "";
        switch (state) {
            case GameBase::State::Init: statestr = "Init"; break;
            case GameBase::State::Paused: statestr = "Paused"; break;
            case GameBase::State::Running: statestr = "Running"; break;
            case GameBase::State::Ended: statestr = "Ended"; break;
        }
        std::ostringstream ss;
        ss << "Game state: " << statestr;
        return ss.str();
    }

    virtual void Start() {
      state = GameBase::State::Running;
    }
    virtual void Pause() {
      state = GameBase::State::Paused;
    }
    virtual void Resume() {
      state = GameBase::State::Running;
    }
    virtual void End()
    {
      state = GameBase::State::Ended;
    }
};

/**
 * Game implementation, event aware
 */
class Game : public GameBase,
    public EventHandler<MarkersDetectedEvent>,
    public EventHandler<GameEvent>
{

    EventRegistration* _evreg_marker;
    EventRegistration* _evreg_game;

protected:
    void OnEvent(MarkersDetectedEvent& e);
    void OnEvent(GameEvent& e);

public:
    Game(Player* player) : GameBase(player) {
      _evreg_marker = EventBus::AddHandler<MarkersDetectedEvent>(*this);
      _evreg_game = EventBus::AddHandler<GameEvent>(*this);
    };
    ~Game() {
      // remove event handlers
      _evreg_marker->RemoveHandler();
      _evreg_game->RemoveHandler();
    }

    Player* GetPlayer() { return _player; }

    void Start();
    void Pause();
    void Resume();
    void End();
};

#endif
