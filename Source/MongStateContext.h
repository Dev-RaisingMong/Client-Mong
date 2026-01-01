#pragma once

#include "Mong.h"
#include "IState.h"

class Mong;

class MongStateContext
{
public:
    MongStateContext(Mong* mong_controller);
    ~MongStateContext();

public:
    void Transition(IState<Mong>* state);
    void ExecuteUpdate(float dt);
    void Reset();

private:
    Mong* m_mong_controller;
    IState<Mong>* m_current_state;
};
