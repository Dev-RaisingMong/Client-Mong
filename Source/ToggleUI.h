#pragma once

#include "axmol/axmol.h"

class ShopUI;
class RaisingUI;

class ToggleUI : public ax::Layer
{
public:
    CREATE_FUNC(ToggleUI)
    bool init() override;
    void Inject(ShopUI* shop_ui, RaisingUI* raising_ui);

private:
    void InitUI();
    void CreateMenuBase();
    void CreateMenuInner();
    ax::MenuItemSprite* CreateMongPurchaseToggle();
    ax::MenuItemSprite* CreateMongUpgradeToggle();

    void OnClickedMongPurchase();
    void OnClickedMongUpgrade();

private:
    ax::Vec2 m_window_size;
    ax::Menu* m_toggle_menu;

    ax::MenuItemToggle* m_purchase_toggle;
    ax::MenuItemToggle* m_upgrade_toggle;

private:
    ShopUI* m_shop_ui;
    RaisingUI* m_raising_ui;
};
