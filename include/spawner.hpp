#ifndef _SPAWNER_H
#define _SPAWNER_H

#include "common.h"
#include "scene.h"

#include <iostream>

/**
 * Unit spawner
 */
template <class T>
class Spawner : public Entity
{
    int _counter; // total spawned count
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

/**
 * Spawn units on update
 */
template<class T>
void Spawner<T>::Update(double delta) {
    if (!_running)
        return;

    _time += delta;
    if (_time > _interval) {
        _time = 0.0;
        this->Spawn();
    }
}


#endif
