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

public:
    double Interval = 2.0; // in seconds

    Spawner() : 
        Entity(200, "spawner", "Sinbad.mesh"),
        _running(false), _time(0.0), _counter(0) {}

    //Spawner(Position pos) : _pos(pos) {}
    /**
     * Spawns entity into scene
     */
    void Spawn(int count = 1);
    void Start() { _running = true; }
    void Stop() { _running = false; }

    void Update(double delta);
};

template<class T>
void Spawner<T>::Update(double delta) {
    _time += delta;
    if (_time > Interval) {
        _time = 0.0;
        this->Spawn();
    }
}

template<>
void Spawner<MobSinbad>::Spawn(int count) {
    for (int i = 0; i < count; ++i ) {
        MobSinbad* mob = new MobSinbad("orcan"+std::to_string(++_counter), nullptr);
        //Position pos = {0, 0, i * 5.0};
        Position pos = {0, 0, 0};
        mob->SetPosition(pos);

        // careful: use newly constructed / pushed object and not "mob"
        Scene::GetInstance().AddEntity(mob);
    }
}

template<>
void Spawner<DragonTower>::Spawn(int count) {
    for (int i = 0; i < count; ++i ) {
        DragonTower* tower = new DragonTower("drogon"+std::to_string(++_counter));
        Position pos = {10.0, 5.0, - (i+1) * 50.0};
        tower->SetPosition(pos);

        // careful: use newly constructed / pushed object and not "mob"
        Scene::GetInstance().AddEntity(tower);
    }
}

#endif
