#pragma once

#include "Mong.h"
#include "IState.h"

class MongIdleState : public IState<Mong>
{
public:
    void ExecuteEnter(Mong* sender) override;
    void Execute(Mong* sender, float dt) override;
    void ExecuteExit(Mong* sender) override;

private:
    float m_elapsed_time;
    float m_target_time;

private:
    const float MIN_IDLE_TIME = 1.0f;
    const float MAX_IDLE_TIME = 2.0f;
};
