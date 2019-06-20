#ifndef _GAME_H
#define _GAME_H

#include <list>

#include "common.h"
#include "entities.h"

/*
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

/*
 * Level/GameState/Scene
 * Uses Singleton pattern
 */
class Scene
{
  Scene() {};
  Scene(Scene const&) = delete;
  void operator=(Scene const&) = delete;

  std::list<GameEntity*> _gameEntities;

public:
  static Scene& GetInstance() {
    static Scene instance;
    return instance;
  }

  void AddEntity(GameEntity* entity) {
    _gameEntities.push_back(entity);
  }

  //void RemoveEntity(GameEntity& entity) {
  //  _gameEntities.remove(entity);
  //}

  std::list<GameEntity*> GetEntities() {
    return _gameEntities;
  }

  template <class T>
  std::list<T*> GetEntities() {
    std::list<T*> matches;

    T* c;
    for (GameEntity* ent : _gameEntities) {
      if ((c = dynamic_cast<T*>(ent)) != nullptr) {
	matches.push_back(c);
      }
    }

    return matches;
  }

  void Save(const char *filename);
  void Load(const char *filename);
};

/*
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
    virtual void Loop() {}
};

struct Game : GameBase {
    Game(Player* player) : GameBase(player) {};
    void Start();
    void Pause();
    void Resume();
    void End();
    void GenerateLevels();
    void Loop();
};

#endif
