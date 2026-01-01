#pragma once

#include "axmol/axmol.h"
#include "MoneyUI.h"

class MoneyService : public ax::Node
{
public:
    static MoneyService* create(MoneyUI* money_ui);
    virtual bool init(MoneyUI* money_ui);
    void onEnter() override;
    void onExit() override;

public:
    void UpdateMoney(int amount);
    int GetMoney() const;

private:
    void Load();
    void Save() const;

private:
    MoneyUI* m_money_ui;
    int m_money;
};
