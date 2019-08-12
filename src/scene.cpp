/**
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

#include "scene.h"

void Scene::Update(double delta) {
    for (auto* ent : GetEntities()) {
        ent->Update(delta);
    }
}

void Scene::Render(Renderer* renderer) {
    // update camera position
    Vec3d tvec = (_corner_ul.tvec+_corner_br.tvec)/2.0;
    renderer->UpdateView(Vec3d(0.0, 0.0, 0.0), Vec3d(0.0, 0.0, 0.0));
    //renderer->UpdateView(tvec, _corner_ul.rvec);
    for (auto* ent : GetEntities()) {
        ent->Render(renderer);
    }
}

//void Scene::OnEvent(UnitKilledEvent& e) {
//}

void Scene::OnEvent(MarkersDetectedEvent& e) {
    Marker m = e.GetMarker();
    switch (m.category) {
        case 0:
            _corner_ul = CamPosition{m.tvec, m.rvec};
            break;
        case 1:
            _corner_br = CamPosition{m.tvec, m.rvec};
            break;
    }
}
