/*
 *
 *
 *
 *
 *
 *
 */

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
    void Load(char* filename);
};

/*
 * Computer vision component
 */
class CV {
  // todo
};

/*
 * Holds some sort of path finding logic
 */
class PathFinding {
  public:
    PathFinding(CV* cv);
    MeshEntity GenerateNavMesh();

    void SetStart(Position pos);
    void SetEnd(Position pos);

    // todo
};

/*
 * Abstract object which represents game entity
 */
class GameObject {
  int _id;
  Position _pos;
  MeshEntity* _mesh;
  char* _name;

  public:
    GameObject(int id, char* name, MeshEntity* mesh) :
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

  protected:
    void IsDead() {_damagetaken >= _hitpoints; }

  public:
    Unit(int id, char* name, int hp, MeshEntity* mesh, PathFinding* pf) : 
      GameObject(id, name, mesh), _hitpoints(hp), _damagetaken(.0f), _pf(pf) {}

    void Spawn(Position spawnPos);
    void DeSpawn();

    void Walk(); // pf contains walking instructions
    void Stop();
    //void LookAt(Position lookAt);

    void TakeDamage(int dmg);
};

/*
 * Describes tower game objects
 */
class Tower : GameObject { 
  int _type;
  float _damage;
  
  public:
    void Spawn(); // build
    void DeSpawn(); // destruct
};

/*
 * Level
 */
class LevelData {
  GameObject* _gameobjects;

  public:
    void Load(char* filename);
};

/*
 * Rendering backend / engine
 */
class Scene {
  MeshEntity* _meshes;
  public:
    void AddMesh(MeshEntity mesh);
    void Render();
};

/*
 * User interaction
 */
class Interface {
  public:
    Interface(CV* cv) {}
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
class Game {
  LevelData* _levels;

  public:
    Game() {}
    void GenerateLevels();
    void Start();
    void Pause();
    void End();
};

/*
 * Defines and checks game rules
class Rules {
  Game* _game;

  public:
    Rules(Game* game) : _game(game) {}
    void Check() { if (true) _game->DenyAction(); }
};
*/
