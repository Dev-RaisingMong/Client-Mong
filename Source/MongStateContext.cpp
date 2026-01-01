#include "MongStateContext.h"

MongStateContext::MongStateContext(Mong* mong_controller)
    : m_mong_controller(mong_controller)
    , m_current_state(nullptr)
{}

MongStateContext::~MongStateContext()
{
    if (m_current_state)
        m_current_state = nullptr;
}

void MongStateContext::Reset()
{
    if (m_current_state)
    {
        m_current_state->ExecuteExit(m_mong_controller);
        m_current_state = nullptr;
    }
}

void MongStateContext::Transition(IState<Mong>* state)
{
    if (m_current_state)
    {
        if (m_current_state == state)
            return;

        if (m_mong_controller)
            m_current_state->ExecuteExit(m_mong_controller);
    }

    m_current_state = state;
    m_current_state->ExecuteEnter(m_mong_controller);
}

void MongStateContext::ExecuteUpdate(float dt)
{
    if (m_current_state)
        m_current_state->Execute(m_mong_controller, dt);
}
