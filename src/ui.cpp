#include "ui.h"

void UI::Render(Renderer* renderer) {
    renderer->ShowText(_game->GetStateString(), Point2f(10,30));
    renderer->ShowText(_game->GetPlayer()->GetNameString(), Point2f(10,70));
    renderer->ShowText(_game->GetPlayer()->GetLivesString(), Point2f(10,110));
    renderer->ShowText(_game->GetPlayer()->GetScoreString(), Point2f(10,150));
}
