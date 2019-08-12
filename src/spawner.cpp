#include <spawner.hpp>

template<>
void Spawner<MobSinbad>::Spawn(int count) {
    for (int i = 0; i < count; ++i ) {
        MobSinbad* mob = new MobSinbad("orc"+std::to_string(++_counter), _paths[_counter % _paths.size()]);
        _counter++;

        mob->SetScale(0.3);
        mob->SetPosition(Vec3d(-500,-500,-500));

        // careful: use newly constructed / pushed object and not "mob"
        Scene::GetInstance().AddEntity(mob);
    }
}

template<>
void Spawner<DragonTower>::Spawn(int count) {
    for (int i = 0; i < count; ++i ) {
        DragonTower* tower = new DragonTower(i, "dragon"+std::to_string(_counter));
        _counter++;

        tower->SetScale(0.3);
        //Vec3d pos{10.0, 5.0, - (i+1) * 50.0};
        //tower->SetPosition(_pos);
        tower->SetPosition(Vec3d(500,500,500));

        // careful: use newly constructed / pushed object and not "mob"
        Scene::GetInstance().AddEntity(tower);
    }
}
