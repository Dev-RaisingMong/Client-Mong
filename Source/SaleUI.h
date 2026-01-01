#pragma once

#include "axmol/axmol.h"
#include "RaisingService.h"
#include "MoneyService.h"
#include "MongData.h"

class Mong;
class Game;

class SaleUI : public ax::Layer
{
public:
    CREATE_FUNC(SaleUI)
    bool init() override;
    void Inject(Game* game, RaisingService* raising_service, MoneyService* money_service);

public:
    void Sell(Mong* target_mong, const MongData* mong_data);
    void ToggleHightlight(bool active);
    Rect GetWorldRect() const;

private:
    void CreateUI();
    RaisingType GetRaisingType(MongCode mong_code);

private:
    ax::Sprite* m_sale_zone_sprite;

private:
    Game* m_game;
    RaisingService* m_raising_service;
    MoneyService* m_money_service;
    
};
