#ifndef _COMMAND_H
#define _COMMAND_H

/*
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
