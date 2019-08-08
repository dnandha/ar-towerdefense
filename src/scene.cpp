#include "scene.h"

void Scene::Update(double delta) {
    for (auto* ent : GetEntities()) {
        ent->Update(delta);
    }
}

void Scene::Render(Renderer* renderer) {
    // update camera position
    renderer->UpdateView(_cam_pos.tvec, _cam_pos.rvec);
    for (auto* ent : GetEntities()) {
        ent->Render(renderer);
    }
}

//void Scene::OnEvent(UnitKilledEvent& e) {
//}

void Scene::OnEvent(MarkersDetectedEvent& e) {
    Marker m = e.GetMarker();
    _cam_pos = CamPosition{m.tvec, m.rvec};
    switch (m.category) {
        case MarkerCategory::Start:
            break;
        case MarkerCategory::Pause:
            break;
        case MarkerCategory::Action:
            //Scene::GetInstance()->BuildTower();
            break;
        case MarkerCategory::Cursor0:
            //Scene::GetInstance()->PlaceTower(0);
            break;
        case MarkerCategory::Cursor1:
            break;
        case MarkerCategory::Cursor2:
            break;
        case MarkerCategory::Cursor3:
            break;
        case MarkerCategory::Cursor4:
            break;
    }
}
