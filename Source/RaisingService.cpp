#include "RaisingService.h"

USING_NS_AX;

RaisingService* RaisingService::create(RaisingUI* raising_ui)
{
    RaisingService* raising_service = new (std::nothrow) RaisingService();

    if (raising_service && raising_service->init(raising_ui))
    {
        raising_service->autorelease();
        return raising_service;
    }

    AX_SAFE_DELETE(raising_service);
    return nullptr;
}

bool RaisingService::init(RaisingUI* raising_ui)
{
    if (!Node::init())
        return false;

    Load();

    return true;
}

void RaisingService::onEnter()
{
    Node::onEnter();
}
void RaisingService::onExit()
{
    Save();
    Node::onExit();
}

void RaisingService::UpdateRaising(RaisingType raising_type, int raising_count)
{
    switch (raising_type)
    {
    case RaisingType::GREEN_MONG_DECORATION:
        m_green_mong_decoration += raising_count;
        break;

    case RaisingType::RED_MONG_DECORTAION:
        m_red_mong_decoration += raising_count;
        break;

    case RaisingType::PINK_MONG_DECORATION:
        m_pink_mong_decortaion += raising_count;
        break;

    case RaisingType::BLUE_MONG_DECORATION:
        m_blue_mong_decoration += raising_count;
        break;

    case RaisingType::MONG_APARTMENT:
        m_mong_apartment += raising_count;
        break;

    case RaisingType::AWESOME_FEEDSTUFF:
        m_awesome_feedstuff += raising_count;
        break;

    default:
        break;
    }
}

int RaisingService::GetRaising(RaisingType raising_type) const
{
    switch (raising_type)
    {
    case RaisingType::GREEN_MONG_DECORATION:
        return m_green_mong_decoration;

    case RaisingType::RED_MONG_DECORTAION:
        return m_red_mong_decoration;

    case RaisingType::PINK_MONG_DECORATION:
        return m_pink_mong_decortaion;

    case RaisingType::BLUE_MONG_DECORATION:
        return m_blue_mong_decoration;

    case RaisingType::MONG_APARTMENT:
        return m_mong_apartment;

    case RaisingType::AWESOME_FEEDSTUFF:
        return m_awesome_feedstuff;

    default:
        return -1;
    }
}

void RaisingService::Load()
{
    m_green_mong_decoration = UserDefault::getInstance()->getIntegerForKey("GREEN_MONG_DECORATION");
    m_red_mong_decoration   = UserDefault::getInstance()->getIntegerForKey("RED_MONG_DECORATION");
    m_pink_mong_decortaion  = UserDefault::getInstance()->getIntegerForKey("PINK_MONG_DECORATION");
    m_blue_mong_decoration  = UserDefault::getInstance()->getIntegerForKey("BLUE_MONG_DECORATION");
    m_mong_apartment        = UserDefault::getInstance()->getIntegerForKey("MONG_APARTMENT");
    m_awesome_feedstuff     = UserDefault::getInstance()->getIntegerForKey("AWESOME_FEEDSTUFF");

    EventCustom raising_load_event("LOAD_RAISING");
    _eventDispatcher->dispatchEvent(&raising_load_event);
}

void RaisingService::Save() const
{
    UserDefault::getInstance()->setIntegerForKey("GREEN_MONG_DECORATION", m_green_mong_decoration);
    UserDefault::getInstance()->setIntegerForKey("RED_MONG_DECORATION", m_red_mong_decoration);
    UserDefault::getInstance()->setIntegerForKey("PINK_MONG_DECORATION", m_pink_mong_decortaion);
    UserDefault::getInstance()->setIntegerForKey("BLUE_MONG_DECORATION", m_blue_mong_decoration);
    UserDefault::getInstance()->setIntegerForKey("MONG_APARTMENT", m_mong_apartment);
    UserDefault::getInstance()->setIntegerForKey("AWESOME_FEEDSTUFF", m_awesome_feedstuff);
}
