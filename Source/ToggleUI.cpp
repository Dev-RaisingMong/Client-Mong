#include "ToggleUI.h"
#include "axmol/ui/UIScale9Sprite.h"
#include "ShopUI.h"
#include "RaisingUI.h"
#include "AudioManager.h"

using namespace ax::ui;
USING_NS_AX;

bool ToggleUI::init()
{
    if (!Layer::init())
        return false;

    InitUI();

    return true;
}

void ToggleUI::InitUI()
{
    m_window_size = _director->getCanvasSize();

    CreateMenuBase();
    CreateMenuInner();
}

void ToggleUI::Inject(ShopUI* shop_ui, RaisingUI* raising_ui)
{
    m_shop_ui = shop_ui;
    m_raising_ui = raising_ui;
}

void ToggleUI::CreateMenuBase()
{
    Rect menu_panel_altas_rect      = Rect(0, 0, 96, 64);
    Rect menu_panel_center_rect     = Rect(10, 10, 96 - 10 * 2, 96 - 10 * 2);
    Vec2 menu_panel_position        = Vec2(m_window_size.x * 0.5f - 6, 20);
    Vec2 menu_panel_size            = Vec2(160, 64);
    Scale9Sprite* menu_panel_sprite = Scale9Sprite::create("Game/UI/Base.png", menu_panel_altas_rect, menu_panel_center_rect);

    //Sprite* menu_panel = Sprite::create("Game/Toggle UI.png", Rect(0, 64, 192, 64));
    
    menu_panel_sprite->setContentSize(menu_panel_size);
    menu_panel_sprite->setPosition(menu_panel_position);
    //menu_panel->setScale(0.75f);
    //this->addChild(menu_panel_sprite);
}

void ToggleUI::CreateMenuInner()
{
    Vec2 toggle_menu_position = Vec2(224, -140);
    Menu* toggle_menu         = Menu::create(CreateMongPurchaseToggle(), CreateMongUpgradeToggle(), nullptr);

    toggle_menu->alignItemsHorizontallyWithPadding(16);
    toggle_menu->setPosition(toggle_menu_position);
    toggle_menu->setScale(0.5f);
    this->addChild(toggle_menu);
}

MenuItemSprite* ToggleUI::CreateMongPurchaseToggle()
{
    Color32 nonselect_color                = Color32::WHITE;
    Color32 select_color                   = Color32(213,218, 227);

    Sprite* mong_purchase_nonselect_sprite = Sprite::create("Game/UI/Button Icon.png", Rect(192, 0, 64, 64));
    mong_purchase_nonselect_sprite->setColor(nonselect_color);

    Sprite* mong_purcahse_select_sprite    = Sprite::create("Game/UI/Button Icon.png", Rect(192, 0, 64, 64));
    mong_purcahse_select_sprite->setColor(select_color);

    MenuItemSprite* mong_purchase_item = MenuItemSprite::create(mong_purchase_nonselect_sprite,
                                                                mong_purcahse_select_sprite,
                                                                AX_CALLBACK_0(ToggleUI::OnClickedMongPurchase, this));

    return mong_purchase_item;
}

MenuItemSprite* ToggleUI::CreateMongUpgradeToggle()
{
    Color32 nonselect_color                            = Color32::WHITE;
    Color32 select_color                               = Color32(213,218, 227);

    Sprite* mong_upgrade_nonselect_sprite = Sprite::create("Game/UI/Button Icon.png", Rect(0, 64, 64, 64));
    mong_upgrade_nonselect_sprite->setColor(nonselect_color);

    Sprite* mong_upgrade_select_sprite = Sprite::create("Game/UI/Button Icon.png", Rect(0, 64, 64, 64));
    mong_upgrade_select_sprite->setColor(select_color);

    MenuItemSprite* mong_upgrade_item = MenuItemSprite::create(mong_upgrade_nonselect_sprite,
                                                               mong_upgrade_select_sprite,
                                                               AX_CALLBACK_0(ToggleUI::OnClickedMongUpgrade, this));

    return mong_upgrade_item;
}

void ToggleUI::OnClickedMongPurchase()
{
    AudioManager::getInstance()->PlaySFX("Audios/SFX/Main Button Click.mp3");

    m_shop_ui->OpenUI();
    m_raising_ui->CloseUI();
}

void ToggleUI::OnClickedMongUpgrade()
{
    AudioManager::getInstance()->PlaySFX("Audios/SFX/Main Button Click.mp3");

    m_shop_ui->CloseUI();
    m_raising_ui->OpenUI();
}
