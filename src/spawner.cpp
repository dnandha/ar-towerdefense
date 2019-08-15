/*
* ARTD (Augmented Reality Tower Defense)
* Copyright (C) 2019 Jaeger,Stegmueller,Boche,Nandha 
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <spawner.hpp>

template <>
void Spawner<MobSinbad>::Spawn(int count) {
  for (int i = 0; i < count; ++i) {
    MobSinbad* mob = new MobSinbad("orc" + std::to_string(_counter + 1),
                                   _paths[_counter % _paths.size()]);
    _counter++;

    mob->SetScale(0.3);
    mob->SetPosition(Vec3d(-500, -500, -500));

    // careful: use newly constructed / pushed object and not "mob"
    Scene::GetInstance().AddEntity(mob);
  }
}

template <>
void Spawner<DragonTower>::Spawn(int count) {
  for (int i = 0; i < count; ++i) {
    DragonTower* tower =
        new DragonTower(i, "dragon" + std::to_string(_counter));
    _counter++;

    tower->SetScale(0.3);
    // Vec3d pos{10.0, 5.0, - (i+1) * 50.0};
    // tower->SetPosition(_pos);
    tower->SetPosition(Vec3d(500, 500, 500));

    // careful: use newly constructed / pushed object and not "mob"
    Scene::GetInstance().AddEntity(tower);
  }
}
