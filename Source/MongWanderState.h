#pragma once

#include "Mong.h"
#include "IState.h"

class MongWanderState : public IState<Mong>
{
public:
    void ExecuteEnter(Mong* sender) override;
    void Execute(Mong* sender, float dt) override;
    void ExecuteExit(Mong* sender) override;

private:
    Vec2 m_target_position;

private:
    const float MIN_WANDER_TIME = 1.0f;
    const float MAX_WANDER_TIME = 3.0f;
    const float MOVE_SPEED      = 10.0f;
};
