#include "scene.h"

//void Scene::Render(Renderer* renderer) {
//    if (renderer != nullptr) {
//        // todo: check if already added
//        renderer->AddEntity(mob.GetName(), mob.GetMeshName());
//    }
//}
//void Scene::OnEvent(UnitKilledEvent& e) {
//}

void Scene::OnEvent(MarkersDetectedEvent& e) {
    if (e.GetMockArg() == "mock") {
      //std::cout << "mock event received" << std::endl;
    }
}
