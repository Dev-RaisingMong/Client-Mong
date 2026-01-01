#include <algorithm>
#include <memory>
#include "MoneyService.h"
#include "MoneyEventData.h"

USING_NS_AX;

MoneyService* MoneyService::create(MoneyUI* money_ui)
{
    MoneyService* money_service = new (std::nothrow) MoneyService();

    if (money_service && money_service->init(money_ui))
    {
        money_service->autorelease();
        return money_service;
    }

    AX_SAFE_DELETE(money_service);
    return nullptr;
}

bool MoneyService::init(MoneyUI* money_ui)
{
    if (!Node::init())
        return false;

    m_money_ui = money_ui;

    Load();

    return true;
}

void MoneyService::onEnter()
{
    Node::onEnter();
    
    m_money_ui->UpdateMoney(m_money, 0, false);
}

void MoneyService::onExit()
{
    Save();
    Node::onExit();
}

void MoneyService::UpdateMoney(int amount)
{
    using namespace std;

    int before_money = m_money;

    m_money += amount;
    m_money = max(m_money, 0);

    MoneyEventData* money_event_data = new MoneyEventData{before_money, amount};
    EventCustom money_update_event("UPDATE_MONEY");
    money_update_event.setUserData(money_event_data);

    _eventDispatcher->dispatchEvent(&money_update_event);
}

int MoneyService::GetMoney() const { return m_money; }

void MoneyService::Load() { m_money = UserDefault::getInstance()->getIntegerForKey("MONEY", 200); }

void MoneyService::Save() const { UserDefault::getInstance()->setIntegerForKey("MONEY", m_money); }
