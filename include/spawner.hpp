#ifndef _SPAWNER_H
#define _SPAWNER_H

#include "common.h"

// todo: move spawner into own header file
/*
 * Unit spawner
 */
template <class T>
class Spawner 
{
    Position _pos;
    int _counter;

public:
    //Spawner(Position pos) : _pos(pos) {}
    /**
     * Spawns entity into scene
     */
    void Spawn(int count);
    void Start(double interval);
    void Stop();
};

template<class MobSinbad>
void Spawner<MobSinbad>::Spawn(int count) {
    for (int i = 0; i < count; ++i ) {
        MobSinbad* mob = new MobSinbad("orcan"+std::to_string(++_counter), nullptr);
        Position pos = {0, 0, i*5.0};
        mob->SetPosition(pos);

        // careful: use newly constructed / pushed object and not "mob"
        Scene::GetInstance().AddEntity(mob);
    }
}

#endif
