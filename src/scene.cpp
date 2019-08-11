#include "scene.h"

void Scene::Update(double delta) {
    for (auto* ent : GetEntities()) {
        ent->Update(delta);
    }
}

void Scene::Render(Renderer* renderer) {
    // update camera position
    Vec3d tvec = (_corner_ul.tvec+_corner_br.tvec)/2.0;
    //renderer->UpdateView(Vec3d(0.0, 0.0, 0.0), Vec3d(0.0, 0.0, 0.0));
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
