#include "MongIdleState.h"

void MongIdleState::ExecuteEnter(Mong* sender)
{
    m_elapsed_time = 0.0f;
    m_target_time  = RandomHelper::random_real(MIN_IDLE_TIME, MAX_IDLE_TIME);

    sender->PlayIdleAnime();
}

void MongIdleState::Execute(Mong* sender, float dt)
{
    m_elapsed_time += dt;
    if (m_elapsed_time >= m_target_time)
        sender->ChangeState(MongState::WANDER);
}

void MongIdleState::ExecuteExit(Mong* sender) {}
