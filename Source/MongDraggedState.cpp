#include "MongDraggedState.h"

void MongDraggedState::ExecuteEnter(Mong* sender)
{
    sender->PlayMoveAnime();
}

void MongDraggedState::Execute(Mong* sender, float dt) {}

void MongDraggedState::ExecuteExit(Mong* sender) {}
