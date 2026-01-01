#pragma once

#include "Mong.h"
#include "IState.h"

class MongDraggedState : public IState<Mong>
{
public:
    void ExecuteEnter(Mong* sender) override;
    void Execute(Mong* sender, float dt) override;
    void ExecuteExit(Mong* sender) override;
};
