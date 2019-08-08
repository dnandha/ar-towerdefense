#ifndef _GAME_H
#define _GAME_H

#include <list>

#include "common.h"

/**
 * Track player statistics
 */
class Player
{
  int _health;
  int _lives;
  int _score;
  const char *_name;

protected:
  bool IsDead();

public:
  Player(const char *name) : _name(name), _health(100), _lives(3) {}
  Player(int health, int lives) : _health(health), _lives(lives) {}

  void TakeHit();
  void ScorePlus() { _score++; }
};

enum State{
  Init,
  Running,
  Paused,
  Ended
};

/**
 * Main game class, puts it all together
 */
class GameBase
{
  Player* _player;

  public:
    State state;

    GameBase(Player* player) : _player(player), state(State::Init) {}

    virtual void Start() {
      state = State::Running;
    }
    virtual void Pause() {
      state = State::Paused;
    }
    virtual void Resume() {
      state = State::Running;
    }
    virtual void End()
    {
      state = State::Ended;
    }

    virtual void GenerateLevels() {}
};

struct Game : GameBase {
    Game(Player* player) : GameBase(player) {};
    void Generate();
    void Start();
    void Pause();
    void Resume();
    void End();
};

#endif
