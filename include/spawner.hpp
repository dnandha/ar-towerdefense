#ifndef _SPAWNER_H
#define _SPAWNER_H

#include "common.h"
#include "scene.h"

#include <iostream>

// todo: move spawner into own header file
/**
 * Unit spawner
 */
template <class T>
class Spawner : public Entity
{
    int _counter;
    double _time;
    bool _running;
    double _interval; // in seconds
    Vec3d _pos;
    std::vector<AbsolutePath*> _paths;

public:

    Spawner(double interval = 3.0, Vec3d position = Vec3d(0.0, 0.0, 0.0)) : 
        Entity(200, "spawner", "Sinbad.mesh"),
        _interval(interval), _pos(position),
        _running(false), _time(0.0), _counter(0) {}

    /**
     * Spawns entity into scene
     */
    void Spawn(int count = 1);
    void Start() { _running = true; }
    void Stop() { _running = false; }

    void SetPaths(std::vector<AbsolutePath*> paths) { _paths = paths; }
    void SetPosition(Vec3d pos) { _pos = pos; }

    void Update(double delta);
};

template<class T>
void Spawner<T>::Update(double delta) {
    _time += delta;
    if (_time > _interval) {
        _time = 0.0;
        this->Spawn();
    }
}

template<>
void Spawner<MobSinbad>::Spawn(int count) {
    for (int i = 0; i < count; ++i ) {
        //MobSinbad* mob = new MobSinbad("orcan"+std::to_string(++_counter), nullptr);
        //if (_paths.size() > 0) {
            MobSinbad* mob = new MobSinbad("orcan"+std::to_string(++_counter), _paths[0]);
        //}
        //Position pos = {0, 0, i * 5.0};
        mob->SetPosition(_pos);

        // careful: use newly constructed / pushed object and not "mob"
        Scene::GetInstance().AddEntity(mob);
    }
}

template<>
void Spawner<DragonTower>::Spawn(int count) {
    for (int i = 0; i < count; ++i ) {
        DragonTower* tower = new DragonTower(i, "drogon"+std::to_string(++_counter));
        //Vec3d pos{10.0, 5.0, - (i+1) * 50.0};
        tower->SetPosition(_pos);

        // careful: use newly constructed / pushed object and not "mob"
        Scene::GetInstance().AddEntity(tower);
    }
}

#endif
