/*
 *
 *
 *
 *
 *
 *
 */
#include <list>

/*
 * 3d position
 */
struct Position {
  float x;
  float y;
  float z;

  Position(float x,float y,float z) : x(x), y(y), z(z) {}
};

/*
 * Holds mesh data
 */
class MeshEntity {
  // todo
  public:
    MeshEntity();
    void Load(const char* filename);
};

/*
 * todo: alternative to "Interface"
 */
enum EventType {
  MarkerPlaceTower1,
  MarkerBuildTower,
  MarkerDestroyTower,
  MarkerStartGame,
  MarkerPauseGame,
  NavMeshLoaded,
  NavStartDefined,
  NavEndDefined,
  NavRouteFound
};

/*
 *
 */
class EventListener {
  public:
    virtual void EventReceived();
};

/*
 *
 */
class EventBus {
  std::list<EventListener> _listeners;
  public:
    EventBus() {}
    void Listen(EventListener listener) { _listeners.push_back(listener); }
    void FireEvent(EventType event) { for (EventListener listener : _listeners) listener.EventReceived(); }
};

/*
 * Computer vision component
 */
class CV {
  // todo
};

/*
 *
 */
class MarkerDetection {
  public:
    MarkerDetection(CV* cv, EventBus bus);
};

/*
 * Holds some sort of path finding logic
 */
class PathFinding {
  public:
    PathFinding(CV* cv, EventBus bus);
};

/*
 * Abstract object which represents game entity
 */
class GameObject {
  int _id;
  Position _pos;
  MeshEntity* _mesh;
  const char* _name;

  public:
    GameObject(int id, const char* name, MeshEntity* mesh) :
      _id(id), _name(name), _mesh(mesh), _pos(0,0,0) {}

    virtual void Spawn(Position spawnPos); // override in derived class
    virtual void DeSpawn(); // override in derived class
};

/*
 * Enemy unit
 */
class Unit : GameObject {
  PathFinding* _pf;

  float _damagetaken;
  float _hitpoints;

  public:
    Unit(int id, const char* name, int hp, MeshEntity* mesh, PathFinding* pf) : 
      GameObject(id, name, mesh), _hitpoints(hp), _damagetaken(.0f), _pf(pf) {}

    void Spawn(Position spawnPos);
    void DeSpawn();

    void Walk(); // pf contains walking instructions
    void Stop();
    //void LookAt(Position lookAt);
    //
    bool HasReachedEnd();

    void TakeDamage(int dmg);
    bool IsDead() { return _damagetaken >= _hitpoints; }
};

/*
 * Describes tower game objects
 */
class Tower : GameObject { 
  int _type;
  float _damage;
  float _range;
  float _fov;
  
  public:
    void Spawn(); // build
    void DeSpawn(); // destruct

    bool Hits(Unit unit);

    float GetDamage() { return _damage; }
};

/*
 * Level
 */
class LevelData {
  std::list<GameObject> _gameobjects;

  public:
    void Load(const char* filename);
};

/*
 * Rendering backend / engine
 */
class Scene {
  std::list<MeshEntity> _meshes;
  public:
    void AddMesh(MeshEntity mesh);
    void Render();
};

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
 * User interaction
 */
class Interface {
  public:
    Interface() {}
    Tower PlaceTower(int id);
    void BuildTower(Tower tower);
    void DestructTower(Tower tower);
    void StartGame();
    void PauseGame();
    void EndGame();
};

/*
 * Put it all together
 */
class Game : EventListener {
  std::list<LevelData> _levels;
  Player* _player;
  int _phase;
  bool _isRunning;

  public:
    Game(Player* player) : _player(player), _phase(0) {}
    void GenerateLevels();
    void Run();
    void Pause();
    void End();

    std::list<Unit> GetUnits();
    std::list<Tower> GetTowers();
};

/*
 * Defines and checks game rules
class Rules {
  Game* _game;

  public:
    Rules(Game* game) : _game(game) {}
    void Check() { if (true) _game->DenyAction(); }
*/
