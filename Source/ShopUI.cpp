#include "axmol/ui/UIScale9Sprite.h"
#include "ShopUI.h"
#include "MongDB.h"
#include "ShopDB.h"
#include "Mong.h"
#include "Game.h"
#include "ObjectPoolManager.h"
#include "NumberFormatter.h"
#include "AudioManager.h"

USING_NS_AX;
using namespace ax::ui;

bool ShopUI::init()
{
    if (!Layer::init())
        return false;

    m_window_size    = _director->getCanvasSize();
    m_shop_data_list = ShopDB::GetAll();

    CreateUI();

    return true;
}

void ShopUI::Inject(Game* game,
                    TMXTiledMap* tilemap,
                    RaisingService* raising_service,
                    MoneyService* money_service,
                    SaleUI* sale_ui)
{
    m_game            = game;
    m_tilemap         = tilemap;
    m_raising_service = raising_service;
    m_money_service   = money_service;
    m_sale_ui         = sale_ui;
}

void ShopUI::OpenUI()
{
    UpdateUI(0);
    ToggleUI(true);
}

void ShopUI::CloseUI()
{
    m_current_index = 0;
    ToggleUI(false);
}

void ShopUI::ToggleUI(bool active)
{
    const float TARGET_TIME = 0.25f;

    this->stopAllActions();

    if (active)
    {
        CallFunc* enable_action = CallFunc::create([this]() { this->setVisible(true); });
        FadeIn* fade_in_action   = FadeIn::create(TARGET_TIME);
        Sequence* sequence_action = Sequence::create(enable_action, fade_in_action, nullptr);

        this->runAction(sequence_action);
    }
    else
    {
        FadeOut* fade_out_action = FadeOut::create(TARGET_TIME);
        CallFunc* disable_action  = CallFunc::create([this]() { this->setVisible(false); });
        Sequence* sequence_action = Sequence::create(fade_out_action, disable_action, nullptr);

        this->runAction(sequence_action);
    }
}

void ShopUI::UpdateUI(int index)
{
    const ShopData& shop_data = m_shop_data_list[index];
    const MongData* mong_data = MongDB::GetByID(shop_data.m_mong_code);

    Rect default_sprite_rect = Rect(index * 64, 0, 64, 64);
    Vec2 portrait_sprite_size = Vec2(64, 64);
    m_portrait_sprite->setTexture("Game/UI/Mong Egg.png");
    m_portrait_sprite->setTextureRect(default_sprite_rect);
    m_portrait_sprite->setContentSize(portrait_sprite_size);

    m_name_label->setString(mong_data->m_name);

    bool can_purchase     = m_money_service->GetMoney() >= shop_data.m_price;
    bool can_accept       = m_game->GetMongCount() <= 1 + m_raising_service->GetRaising(RaisingType::MONG_APARTMENT);
    Color32 enable_color  = Color32(87, 72, 82);
    Color32 disable_color = Color32::RED;
    m_purchase_button->setTitleText(can_accept ? fmt::format("{}G", NumberFormatter::Format(shop_data.m_price))
                                               : "최대");
    m_purchase_button->setTitleFontName("fonts/exqt.ttf");
    m_purchase_button->setTitleColor(can_purchase && can_accept ? enable_color : disable_color);
    m_purchase_button->setTitleFontSize(12);
    m_purchase_button->setEnabled(can_purchase && can_accept);
    m_purchase_button->setBright(can_purchase && can_accept);
}

void ShopUI::CreateUI()
{
    this->setCascadeOpacityEnabled(true);
    this->setVisible(false);
    this->setOpacity(0.0f);

    CreateShopPanel();
    CreateNameGroup();
    CreatePortraitGroup();
    CreateButtonGroup();
}

void ShopUI::CreateShopPanel()
{
    Rect shop_panel_sprite_rect = Rect(0, 0, 96, 96);
    Rect shop_panel_center_rect = Rect(15, 15, 96 - 15 * 2, 96 - 15 * 2);
    Vec2 shop_panel_position    = Vec2(m_window_size.x * 0.5f, m_window_size.y * 0.5f);
    Size shop_panel_size        = Size(192, 192);
    m_shop_panel = Scale9Sprite::create("Game/UI/Base.png", shop_panel_sprite_rect, shop_panel_center_rect);
    m_shop_panel->setCascadeOpacityEnabled(true);
    m_shop_panel->setContentSize(shop_panel_size);
    m_shop_panel->setPosition(shop_panel_position);
    this->addChild(m_shop_panel);

    m_panel_size = m_shop_panel->getContentSize();

    Vec2 pattern_position  = Vec2(m_panel_size.x * 0.5f, m_panel_size.y - 28);
    Vec2 pattern_size      = Vec2(166, 16);
    Sprite* pattern_sprite = Sprite::create("Game/UI/Top Pattern 1.png");
    pattern_sprite->setPosition(pattern_position);
    pattern_sprite->setContentSize(pattern_size);
    m_shop_panel->addChild(pattern_sprite);

    float dx[] = {0, m_panel_size.x};
    float dy[] = {0, m_panel_size.y};

    Rect corner_sprite_rect = Rect(97, 0, 14, 14);
    for (int i = 0; i < 2; i++)
    {
        float x_offset = 5.0f * (i % 2 == 0 ? 1 : -1);

        for (int j = 0; j < 2; j++)
        {
            float y_offset        = 5.0f * (j % 2 == 0 ? 1 : -1);
            Sprite* corner_sprite = Sprite::create("Game/UI/Base.png", corner_sprite_rect);
            Vec2 corner_position  = Vec2(dx[i] + x_offset, dy[j] + y_offset);
            corner_sprite->setPosition(corner_position);
            m_shop_panel->addChild(corner_sprite);
        }
    }

    Vec2 close_button_position = Vec2(m_panel_size.x - 5, m_panel_size.y - 5);
    Vec2 close_button_size     = Vec2(21, 21);
    Button* close_button       = Button::create("Game/UI/Close Button.png");
    close_button->setPosition(close_button_position);
    close_button->setSizePercent(close_button_size);
    close_button->addClickEventListener([this](Object*)
    {
        AudioManager::getInstance()->PlaySFX("Audios/SFX/Main Button Click.mp3");
        this->CloseUI();
    });
    m_shop_panel->addChild(close_button);
}

void ShopUI::CreateNameGroup()
{
    Rect name_panel_sprite_rect = Rect(125, 56, 64, 16);
    Rect name_panel_center_rect = Rect(4, 2, 64 - 4 * 2, 16 - 2 * 2);
    Vec2 name_panel_position    = Vec2(m_panel_size.x * 0.5f, m_panel_size.y - 4);
    Size name_panel_size        = Size(96, 24);
    Scale9Sprite* name_panel = Scale9Sprite::create("Game/UI/Base.png", name_panel_sprite_rect, name_panel_center_rect);
    name_panel->setCascadeOpacityEnabled(true);
    name_panel->setContentSize(name_panel_size);
    name_panel->setPosition(name_panel_position);
    m_shop_panel->addChild(name_panel);

    const Vec2 NAME_PANEL_SIZE = name_panel->getContentSize();

    Vec2 title_label_position = Vec2(NAME_PANEL_SIZE.x * 0.5f, NAME_PANEL_SIZE.y * 0.5f);
    Color32 title_label_color = Color32(186, 145, 88);
    Label* title_label        = Label::createWithTTF("SHOP", "fonts/exqt.ttf", 16);
    title_label->setCascadeOpacityEnabled(true);
    title_label->setColor(title_label_color);
    title_label->setPosition(title_label_position);
    name_panel->addChild(title_label);
}

void ShopUI::CreatePortraitGroup()
{
    Rect mong_box_sprite_rect = Rect(0, 0, 56, 57);
    Rect mong_box_center_rect = Rect(4, 4, 56 - 4 * 2, 57 - 4 * 2);
    Vec2 mong_box_position    = Vec2(m_panel_size.x * 0.5f, m_panel_size.y * 0.5f + 4);
    Vec2 mong_box_size        = Vec2(64, 64);
    Scale9Sprite* mong_box_sprite =
        Scale9Sprite::create("Game/UI/Char Box.png", mong_box_sprite_rect, mong_box_center_rect);
    mong_box_sprite->setCascadeOpacityEnabled(true);
    mong_box_sprite->setPosition(mong_box_position);
    mong_box_sprite->setContentSize(mong_box_size);
    m_shop_panel->addChild(mong_box_sprite);

    const Vec2 BOX_PANEL_SIZE = mong_box_sprite->getContentSize();

    Vec2 mong_portrait_position = Vec2(BOX_PANEL_SIZE.x * 0.5f, BOX_PANEL_SIZE.y * 0.5f);
    Vec2 mong_portrait_size = Vec2(64, 64);
    m_portrait_sprite       = Sprite::create();
    m_portrait_sprite->setPosition(mong_portrait_position);
    m_portrait_sprite->setContentSize(mong_portrait_size);
    mong_box_sprite->addChild(m_portrait_sprite);

    Vec2 name_label_position = Vec2(m_panel_size.x * 0.5f, m_panel_size.y * 0.5f - 40);
    Color32 name_label_color = Color32(186, 145, 88);
    m_name_label             = Label::createWithTTF("Mong Name", "fonts/exqt.ttf", 12);
    m_name_label->setPosition(name_label_position);
    m_name_label->setColor(name_label_color);
    m_shop_panel->addChild(m_name_label);
}

void ShopUI::CreateButtonGroup()
{
    Vec2 left_button_position = Vec2(m_panel_size.x * 0.5f - 64, m_panel_size.y * 0.5f);
    float left_button_scale   = 2.0f;
    m_left_button             = Button::create("Game/UI/Left Arrow.png");
    m_left_button->setScale(left_button_scale);
    m_left_button->setPosition(left_button_position);
    m_left_button->addClickEventListener(AX_CALLBACK_0(ShopUI::OnClickedLeftArrow, this));
    m_shop_panel->addChild(m_left_button);

    Vec2 right_button_position = Vec2(m_panel_size.x * 0.5f + 64, m_panel_size.y * 0.5f);
    float right_button_scale = 2.0f;
    m_right_button           = Button::create("Game/UI/Right Arrow.png");
    m_right_button->setScale(right_button_scale);
    m_right_button->setPosition(right_button_position);
    m_right_button->addClickEventListener(AX_CALLBACK_0(ShopUI::OnClickedRightArrow, this));
    m_shop_panel->addChild(m_right_button);

    Vec2 purchase_button_position = Vec2(m_panel_size.x * 0.5f, m_panel_size.y * 0.5f - 60);
    Vec2 purchase_button_size     = Vec2(156, 52);
    m_purchase_button             = Button::create("Game/UI/Button.png");
    m_purchase_button->setPosition(purchase_button_position);
    m_purchase_button->setContentSize(purchase_button_size);
    m_purchase_button->addClickEventListener(AX_CALLBACK_0(ShopUI::OnClickedPurchase, this));
    m_shop_panel->addChild(m_purchase_button);
}

void ShopUI::OnClickedLeftArrow()
{
    if (m_shop_data_list.empty())
        return;

    int list_size   = static_cast<int>(m_shop_data_list.size());
    m_current_index = (m_current_index - 1 + list_size) % list_size;

    UpdateUI(m_current_index);

    AudioManager::getInstance()->PlaySFX("Audios/SFX/Button Click.mp3");
}

void ShopUI::OnClickedRightArrow()
{
    if (m_shop_data_list.empty())
        return;

    int list_size   = static_cast<int>(m_shop_data_list.size());
    m_current_index = (m_current_index + 1 + list_size) % list_size;

    UpdateUI(m_current_index);

    AudioManager::getInstance()->PlaySFX("Audios/SFX/Button Click.mp3");
}

void ShopUI::OnClickedPurchase()
{
    ShopData m_current_shop_data        = m_shop_data_list[m_current_index];
    const MongData* m_current_mong_data = MongDB::GetByID(m_current_shop_data.m_mong_code);

    Mong* new_mong = dynamic_cast<Mong*>(ObjectPoolManager::Get(ObjectType::MONG));
    m_game->addChild(new_mong);
    m_game->AddMong(new_mong);
    new_mong->Inject(m_tilemap, m_raising_service, m_money_service, m_current_mong_data, m_game, m_sale_ui);
    new_mong->setPosition(m_window_size.x * 0.5f, m_window_size.y * 0.5f);

    m_money_service->UpdateMoney(-m_current_shop_data.m_price);
    UpdateUI(m_current_index);

    AudioManager::getInstance()->PlaySFX("Audios/SFX/Purchase.mp3");
}
