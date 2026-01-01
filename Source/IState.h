#pragma once

template <typename T>
class IState
{
public:
    virtual ~IState()                 = default;

    virtual void ExecuteEnter(T* sender)      = 0;
    virtual void Execute(T* sender, float dt) = 0;
    virtual void ExecuteExit(T* sender)       = 0;
};
