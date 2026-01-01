#include "MongWanderState.h"

void MongWanderState::ExecuteEnter(Mong* sender)
{
    const float WANDER_RADIUS = 50.0f;
    const int MAX_TRY = 40;

    for (int i = 0; i < MAX_TRY; i++)
    {
        Vec2 candidate = sender->GetRandomPointInRadius(WANDER_RADIUS);

        if (sender->CheckDestination(candidate))
        {
            m_target_position = candidate;

            Vec2 direction = m_target_position - sender->getPosition();
            direction.normalize();
            sender->SetVelocity(direction);
            sender->PlayMoveAnime();

            return;
        }
    }

    m_target_position = sender->getPosition();
}

void MongWanderState::Execute(Mong* sender, float dt)
{
    bool arrived = sender->MoveToDestination(m_target_position, dt);
    if (arrived)
        sender->ChangeState(MongState::IDLE);
}

void MongWanderState::ExecuteExit(Mong* sender) {}
