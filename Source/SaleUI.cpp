#include "SaleUI.h"
#include "ObjectPoolManager.h"
#include "AudioManager.h"
#include "Mong.h"
#include "Game.h"

USING_NS_AX;

bool SaleUI::init()
{
    if (!Layer::init())
        return false;

    CreateUI();

    return true;
}

void SaleUI::Inject(Game* game, RaisingService* raising_service, MoneyService* money_service)
{
    m_game = game;
    m_raising_service = raising_service;
    m_money_service   = money_service;
}

void SaleUI::Sell(Mong* target_mong, const MongData* mong_data)
{
    ToggleHightlight(false);
    AudioManager::getInstance()->PlaySFX("Audios/SFX/Purchase.mp3");

    RaisingType raising_type = GetRaisingType(mong_data->m_code);
    int upgrade_level        = m_raising_service->GetRaising(raising_type);
    Money sale_money = mong_data->GetSaleCost(upgrade_level);

    m_money_service->UpdateMoney(sale_money);
    m_game->RemoveMong(target_mong);
    ObjectPoolManager::Release(target_mong);
}

void SaleUI::ToggleHightlight(bool active)
{
    if (!m_sale_zone_sprite)
        return;

    m_sale_zone_sprite->stopAllActions();

    if (active)
    {
        m_sale_zone_sprite->setColor(Color32(255, 220, 120));
        m_sale_zone_sprite->setOpacity(255);

        ScaleTo* scale_up   = ScaleTo::create(0.15f, 1.15f);
        ScaleTo* scale_down = ScaleTo::create(0.15f, 1.0f);

        m_sale_zone_sprite->runAction(RepeatForever::create(Sequence::create(scale_up, scale_down, nullptr)));
    }
    else
    {
        m_sale_zone_sprite->setColor(Color32::WHITE);
        m_sale_zone_sprite->setScale(1.0f);
        m_sale_zone_sprite->setOpacity(180);
    }
}

Rect SaleUI::GetWorldRect() const
{
    if (!m_sale_zone_sprite)
        return Rect::ZERO;

    return m_sale_zone_sprite->getBoundingBox();
}

RaisingType SaleUI::GetRaisingType(MongCode mong_code)
{
    switch (mong_code)
    {
    case MongCode::GREEN_MONG:
        return RaisingType::GREEN_MONG_DECORATION;

    case MongCode::RED_MONG:
        return RaisingType::RED_MONG_DECORTAION;

    case MongCode::PINK_MONG:
        return RaisingType::PINK_MONG_DECORATION;

    case MongCode::BLUE_MONG:
        return RaisingType::BLUE_MONG_DECORATION;
    }
}

void SaleUI::CreateUI()
{
    Vec2 sale_zone_position  = Vec2(20, 20);
    Vec2 sale_zone_size      = Vec2(32, 32);
    m_sale_zone_sprite       = Sprite::create("Game/UI/Button Icon.png", Rect(64, 64, 64, 64));
    m_sale_zone_sprite->setPosition(sale_zone_position);
    m_sale_zone_sprite->setContentSize(sale_zone_size);
    this->addChild(m_sale_zone_sprite);
}
