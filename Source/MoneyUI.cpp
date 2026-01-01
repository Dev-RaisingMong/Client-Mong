#include <algorithm>
#include "MoneyUI.h"
#include "MoneyEventData.h"
#include "NumberFormatter.h"

USING_NS_AX;

bool MoneyUI::init()
{
    if (!Layer::init())
        return false;

    m_window_size   = _director->getCanvasSize();

    InitUI();

    return true;
}

void MoneyUI::onEnter()
{
    Layer::onEnter();

    auto update_money_action = [&](EventCustom* event)
    {
        auto data = static_cast<MoneyEventData*>(event->getUserData());

        int before = data->m_before;
        int amount = data->m_amount;

        delete data;

        UpdateMoney(before, amount);
    };

    m_event_listener = EventListenerCustom::create("UPDATE_MONEY", update_money_action);
    _eventDispatcher->addEventListenerWithFixedPriority(m_event_listener, 1);
}

void MoneyUI::onExit()
{
    _eventDispatcher->removeEventListener(m_event_listener);
    Layer::onExit();
}

void MoneyUI::InitUI()
{
    CreateMoneyPanel();
    CreateMoneyLabel();
}

void MoneyUI::UpdateMoney(int before, int amount, bool is_action)
{
    int current = before + amount;

    if (is_action)
    {
        RunMoneyChangeAction(m_money_label, before, current);
    }
    else
    {
        using namespace std;

        string target_string = NumberFormatter::Format(current);
        m_money_label->setString(target_string);
    }
}

void MoneyUI::CreateMoneyPanel()
{
    Vec2 money_panel_position = Vec2(m_window_size.x - 95.0f, m_window_size.y - 25.0f);
    Vec2 money_panel_size     = Vec2(96, 24);
    m_money_panel             = Sprite::create("Game/UI/Base.png", Rect(125, 56, 64, 16));
    m_money_panel->setPosition(money_panel_position);
    m_money_panel->setContentSize(money_panel_size);
    this->addChild(m_money_panel);

    m_panel_size = m_money_panel->getContentSize();

    Vec2 coin_position = Vec2(5, m_panel_size.y * 0.5f + 5);
    Sprite* coin_icon = Sprite::create("Game/UI/Coin.png");
    coin_icon->setPosition(coin_position);
    m_money_panel->addChild(coin_icon);
}

void MoneyUI::CreateMoneyLabel()
{
    Color32 money_label_color            = Color32(186, 145, 88);
    Vec2 money_label_position            = Vec2(m_panel_size.x - 10.0f, m_panel_size.y * 0.5f);
    Vec2 money_label_anchor              = Vec2(1.0f, 0.5f);
    TextHAlignment money_label_alignment = TextHAlignment::RIGHT;
    m_money_label                        = Label::createWithTTF("000", "fonts/exqt.ttf", 16);
    m_money_label->setColor(money_label_color);
    m_money_label->setAnchorPoint(money_label_anchor);
    m_money_label->setPosition(money_label_position);
    m_money_label->setHorizontalAlignment(money_label_alignment);
    m_money_panel->addChild(m_money_label);
}

void MoneyUI::RunMoneyChangeAction(Label* label, int from, int to)
{
    using namespace std;

    label->stopAllActions();

    const float duration = 0.5f;

    auto update_money_action = [=](float value)
    {
        label->setString(NumberFormatter::Format(value));
    };

    ActionFloat* action = ActionFloat::create(duration, static_cast<float>(from), static_cast<float>(to), update_money_action);
    label->runAction(action);
}
