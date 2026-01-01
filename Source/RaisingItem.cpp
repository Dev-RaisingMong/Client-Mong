#include "axmol/ui/UIScale9Sprite.h"
#include "RaisingItem.h"
#include "NumberFormatter.h"
#include "AudioManager.h"

using namespace ax::ui;
USING_NS_AX;

RaisingItem* RaisingItem::create(MoneyService* money_service, RaisingService* raising_service)
{
    RaisingItem* raising_item = new (std::nothrow) RaisingItem();

    if (raising_item && raising_item->init(money_service, raising_service))
    {
        raising_item->autorelease();
        return raising_item;
    }

    AX_SAFE_DELETE(raising_item);
    return nullptr;
}

bool RaisingItem::init(MoneyService* money_service, RaisingService* raising_service)
{
    if (!Layer::init())
        return false;

    m_money_service = money_service;
    m_raising_service = raising_service;

    CreateUI();

    return true;
}

void RaisingItem::onEnter()
{
    Layer::onEnter();

    auto update_money_action = [&](EventCustom* event) { UpdateUI(); };

    m_money_update_event_listener = EventListenerCustom::create("UPDATE_MONEY", update_money_action);
    _eventDispatcher->addEventListenerWithFixedPriority(m_money_update_event_listener, 1);

    auto raising_load_action = [&](EventCustom* event)
    {
        UpdateUI();
        _eventDispatcher->removeEventListener(m_raising_load_event_listener);
    };

    m_raising_load_event_listener = EventListenerCustom::create("LOAD_RAISING", raising_load_action);
    _eventDispatcher->addEventListenerWithFixedPriority(m_raising_load_event_listener, 1);
}

void RaisingItem::InitUI(RaisingData raising_data)
{
    m_raising_data = raising_data;

    m_portrait_image->setTexture("Game/UI/Mong Attribute Icon.png");
    m_portrait_image->setTextureRect(raising_data.m_atlas_rect);
    m_portrait_image->setContentSize(raising_data.m_atlas_rect.size);

    m_name_label->setString(m_raising_data.m_raising_name);
    m_description_label->setString(m_raising_data.m_raising_description);

    UpdateUI();
}

void RaisingItem::UpdateUI()
{
    int raising_level = m_raising_service->GetRaising(m_raising_data.m_raising_type);
    m_level_label->setString(fmt::format("LV.{}", raising_level));

    Money raising_cost = m_raising_data.GetCost(raising_level);
    bool can_raising   = m_money_service->GetMoney() >= raising_cost;

    Color32 enable_label_color  = Color32(87, 72, 82);
    Color32 disable_label_color = Color32::RED;
    m_raising_button->setTitleText(fmt::format("{}G", NumberFormatter::Format(raising_cost)));
    m_raising_button->setTitleFontName("fonts/exqt.ttf");
    m_raising_button->setTitleFontSize(12);
    m_raising_button->setTitleColor(can_raising ? enable_label_color : disable_label_color);
    m_raising_button->setEnabled(can_raising);
    m_raising_button->setBright(can_raising);
}

void RaisingItem::CreateUI()
{
    this->setCascadeOpacityEnabled(true);

    CreateItemPanel();
    CreatePortraitGroup();
    CreateDescriptionGroup();
    CreateRaisingButton();
}

void RaisingItem::CreateItemPanel()
{
    Rect raising_item_sprite_rect = Rect(0, 0, 56, 57);
    Rect raising_item_center_rect = Rect(4, 4, 56 - 4 * 2, 57 - 4 * 2);
    Vec2 raising_panel_size       = Vec2(192, 48);
    Vec2 raising_panel_position   = Vec2::ZERO;
    m_raising_item_panel = Scale9Sprite::create("Game/UI/Char Box.png", raising_item_sprite_rect, raising_item_center_rect);
    m_raising_item_panel->setCascadeOpacityEnabled(true);
    m_raising_item_panel->setPosition(raising_panel_position);
    m_raising_item_panel->setContentSize(raising_panel_size);
    this->addChild(m_raising_item_panel);

    m_item_size = m_raising_item_panel->getContentSize();
}

void RaisingItem::CreatePortraitGroup()
{
    Rect portrait_panel_sprite_rect = Rect(0, 0, 56, 57);
    Rect portrait_panel_center_rect = Rect(4, 4, 56 - 4 * 2, 57 - 4 * 2);
    Vec2 portrait_panel_size        = Vec2(40, 40);
    Vec2 portrait_panel_position    = Vec2(25, m_item_size.y * 0.5f);
    Color32 portrait_panel_color    = Color32(230, 230, 230);
    Scale9Sprite* portrait_panel = Scale9Sprite::create("Game/UI/Char Box.png", portrait_panel_sprite_rect, portrait_panel_center_rect);
    portrait_panel->setCascadeOpacityEnabled(true);
    portrait_panel->setPosition(portrait_panel_position);
    portrait_panel->setContentSize(portrait_panel_size);
    portrait_panel->setColor(portrait_panel_color);
    m_raising_item_panel->addChild(portrait_panel);

    const Vec2 PORTRAIT_SIZE = portrait_panel->getContentSize();

    Vec2 portrait_image_size = Vec2(32, 32);
    Vec2 portrait_image_position = Vec2(PORTRAIT_SIZE.x * 0.5f, PORTRAIT_SIZE.y * 0.5f);
    m_portrait_image = Sprite::create();
    m_portrait_image->setPosition(portrait_image_position);
    m_portrait_image->setContentSize(portrait_image_size);
    portrait_panel->addChild(m_portrait_image);
}

void RaisingItem::CreateDescriptionGroup()
{
    Vec2 name_label_position              = Vec2(50, m_item_size.y * 0.5f + 12);
    TextHAlignment name_label_h_alignment = TextHAlignment::LEFT;
    Color32 name_label_color              = Color32(89, 148, 104);
    m_name_label                          = Label::createWithTTF("", "fonts/exqt.ttf", 12);
    m_name_label->setPosition(name_label_position);
    m_name_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    m_name_label->setColor(name_label_color);
    m_name_label->setHorizontalAlignment(name_label_h_alignment);
    m_raising_item_panel->addChild(m_name_label);

    Vec2 desc_label_position              = Vec2(50, m_item_size.y * 0.5f - 1);
    TextHAlignment desc_label_h_alignment = TextHAlignment::LEFT;
    Color32 desc_label_color              = Color32(186, 145, 88);
    m_description_label                   = Label::createWithTTF("", "fonts/exqt.ttf", 12);
    m_description_label->setPosition(desc_label_position);
    m_description_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    m_description_label->setColor(desc_label_color);
    m_description_label->setHorizontalAlignment(desc_label_h_alignment);
    m_raising_item_panel->addChild(m_description_label);

    Vec2 level_label_position              = Vec2(50, m_item_size.y * 0.5f - 14);
    TextHAlignment level_label_h_alignment = TextHAlignment::LEFT;
    Color32 level_label_color              = Color32(255, 190, 152);
    m_level_label                          = Label::createWithTTF("", "fonts/exqt.ttf", 12);
    m_level_label->setPosition(level_label_position);
    m_level_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    m_level_label->setColor(level_label_color);
    m_level_label->setHorizontalAlignment(level_label_h_alignment);
    m_raising_item_panel->addChild(m_level_label);
}

void RaisingItem::CreateRaisingButton()
{
    Vec2 raising_button_position = Vec2(m_item_size.x - 24, m_item_size.y * 0.5f);
    Vec2 raising_button_size     = Vec2(34, 34);
    m_raising_button             = Button::create("Game/UI/Menu Box.png");
    m_raising_button->setCascadeOpacityEnabled(true);
    m_raising_button->setScale9Enabled(true);
    m_raising_button->setPosition(raising_button_position);
    m_raising_button->setContentSize(raising_button_size);
    m_raising_button->addClickEventListener(AX_CALLBACK_0(RaisingItem::OnClickedRaising, this));
    m_raising_item_panel->addChild(m_raising_button);
}

void RaisingItem::OnClickedRaising()
{
    int raising_level = m_raising_service->GetRaising(m_raising_data.m_raising_type);
    Money raising_cost = m_raising_data.GetCost(raising_level);
    bool can_raising   = m_money_service->GetMoney() >= raising_cost;

    if (!can_raising)
        return;

    m_raising_service->UpdateRaising(m_raising_data.m_raising_type);
    m_money_service->UpdateMoney(-raising_cost);

    AudioManager::getInstance()->PlaySFX("Audios/SFX/Purchase.mp3");
}
