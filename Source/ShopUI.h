#pragma once

#include "axmol/axmol.h"
#include "axmol/ui/UIButton.h"
#include "ShopData.h"

class MoneyService;
class RaisingService;
class Game;
class SaleUI;

class ShopUI : public ax::Layer
{
public:
    CREATE_FUNC(ShopUI)
    bool init() override;

public:
    void OpenUI();
    void CloseUI();
    void Inject(Game* game,
                ax::TMXTiledMap* tilemap,
                RaisingService* raising_service,
                MoneyService* money_service,
                SaleUI* sale_ui);

private:
    void CreateUI();
    void CreateShopPanel();
    void CreateNameGroup();
    void CreatePortraitGroup();
    void CreateButtonGroup();

    void UpdateUI(int index);
    void ToggleUI(bool active);

    void OnClickedLeftArrow();
    void OnClickedRightArrow();
    void OnClickedPurchase();

private:
    ax::Size m_window_size;
    ax::Size m_panel_size;

    ax::Node* m_shop_panel;
    ax::Sprite* m_portrait_sprite;
    ax::ui::Button* m_left_button;
    ax::ui::Button* m_right_button;
    ax::ui::Button* m_purchase_button;
    ax::Label* m_name_label;

private:
    MoneyService* m_money_service;
    RaisingService* m_raising_service;
    
    int m_current_index = 0;
    std::vector<ShopData> m_shop_data_list;

    Game* m_game;
    SaleUI* m_sale_ui;
    ax::TMXTiledMap* m_tilemap;
};
