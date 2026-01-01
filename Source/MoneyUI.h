#pragma once

#include "axmol/axmol.h"

class MoneyUI : public ax::Layer
{
public:
    CREATE_FUNC(MoneyUI)
    bool init() override;
    void onEnter() override;
    void onExit() override;

public:
    void InitUI();
    void UpdateMoney(int before, int amount, bool is_action = true);

private:
    void CreateMoneyPanel();
    void CreateMoneyLabel();
    void RunMoneyChangeAction(ax::Label* label, int from , int to);

private:
    ax::Size m_window_size;
    ax::Size m_panel_size;

    ax::Node* m_money_panel;
    ax::Label* m_money_label;

    ax::EventListenerCustom* m_event_listener;
};
