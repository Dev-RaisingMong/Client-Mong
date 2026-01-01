#pragma once

#include "axmol/axmol.h"
#include "MoneyService.h"
#include "RaisingService.h"
#include "SettingService.h"
#include "MongService.h"
#include "SaleUI.h"
#include "Mong.h"

class Mong;
class MongService;

class Game : public ax::Scene
{
public:
    enum LAYER
    {
        BACKGROUND_LAYER = 0,
        GAME_LAYER = 1,
        UI_LAYER = 2,
        SETTING_LAYER = 3,
    };

    enum UI
    {
        MONEY_UI = 100,
        SHOP_UI = 101,
        TOGGLE_UI = 102,
        RAISING_UI = 103,
        SALE_UI = 104, 
        SETTING_UI = 105,
        SETTING_BUTTON = 106,  
    };

public:
    bool init() override;
    void onEnter() override;

    void AddMong(Mong* mong);
    void RemoveMong(Mong* mong);
    int GetMongCount() const;
    void RestoreMong(MongCode code, int satiety, ax::Vec2 position);
    const ax::Vector<Mong*> GetMongList() const;

    void Notify(std::string notice_string, ax::Color32 color = ax::Color32::WHITE);

    ax::Layer* GetGameLayer() const { return m_game_layer; }

private:
    void InitDefendency();
    void InitLayers();
    void CreateSetting();
    void CreateTileMap();
    void CreateLightParticle();

private:
    ax::Layer* m_game_layer;
    ax::Layer* m_ui_layer;
    ax::Layer* m_setting_layer;
    ax::Layer* m_background_layer;

    MoneyService* m_money_service;
    RaisingService* m_raising_service;
    SettingService* m_setting_service;
    MongService* m_mong_service;

    ax::Vec2 m_window_size;
    ax::TMXTiledMap* m_tilemap;
    SaleUI* m_sale_ui;
    ax::Vector<Mong*> m_mong_vector;
};
