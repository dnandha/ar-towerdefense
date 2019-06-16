#ifndef _GAME_H
#define _GAME_H

#include <list>

#include "common.h"
#include "entities.h"

/*
 * Track player statistics
 */
class Player {
  int _health;
  int _lives;
  int _score;
  const char* _name;

  protected:
    bool IsDead();

  public:
    Player(const char* name) : _name(name), _health(100), _lives(3) {}
    Player(int health, int lives) : _health(health), _lives(lives) {}

    void TakeHit();
    void ScorePlus() { _score++; }
};

/*
 * Put it all together
 */
class Game : EventListener {
  std::list<Scene> _scenes;
  Player* _player;
  int _phase;
  bool _hasEnded;
  bool _isPaused;

  public:
    Game(Player* player) : _player(player), _phase(0) {}
    void GenerateLevels();
    //void Run() {_hasEnded = false; _isPaused = false;}
    void Start();
    void Loop();
    void Pause() {_isPaused = true;}
    void Resume() {_isPaused = false;}
    void End() {_hasEnded = true; _isPaused = false;}

    bool HasEnded() {return _hasEnded;}
    bool IsPaused() {return _isPaused;}

    void HandleEvent(EventType event);

    std::list<Unit> GetUnits();
    std::list<Tower> GetTowers();
};

#endif
