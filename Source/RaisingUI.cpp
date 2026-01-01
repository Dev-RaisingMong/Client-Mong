#include "axmol/ui/UIScale9Sprite.h"
#include "axmol/ui/UIButton.h"
#include "RaisingUI.h"
#include "RaisingDB.h"
#include "RaisingItem.h"
#include "AudioManager.h"

using namespace ax::ui;
USING_NS_AX;

bool RaisingUI::init()
{
    if (!Layer::init())
        return false;

    m_raising_data_list = RaisingDB::GetAll();
    m_window_size       = _director->getCanvasSize();

    CreateUI();

    return true;
}

void RaisingUI::Inject(MoneyService* money_service, RaisingService* raising_service)
{
    m_money_service   = money_service;
    m_raising_service = raising_service;

    InitUI();
}

void RaisingUI::OpenUI()
{
    if (m_scroll_view)
        m_scroll_view->jumpToTop();

    ToggleUI(true);
}

void RaisingUI::CloseUI() { ToggleUI(false); }

void RaisingUI::ToggleUI(bool active)
{
    const float TARGET_TIME = 0.25f;

    this->stopAllActions();

    if (active)
    {
        CallFunc* enable_action   = CallFunc::create([this]() { this->setVisible(true); });
        FadeIn* fade_in_action    = FadeIn::create(TARGET_TIME);
        Sequence* sequence_action = Sequence::create(enable_action, fade_in_action, nullptr);

        this->runAction(sequence_action);
    }
    else
    {
        FadeOut* fade_out_action  = FadeOut::create(TARGET_TIME);
        CallFunc* disable_action  = CallFunc::create([this]() { this->setVisible(false); });
        Sequence* sequence_action = Sequence::create(fade_out_action, disable_action, nullptr);

        this->runAction(sequence_action);
    }
}

void RaisingUI::CreateUI()
{
    this->setCascadeOpacityEnabled(true);
    this->setVisible(false);
    this->setOpacity(0.0f);

    CreateRaisingPanel();
    CreateTitleGroup();
    CreateScrollView();
}

void RaisingUI::CreateRaisingPanel()
{
    Vec2 raising_panel_position    = Vec2(m_window_size.x * 0.5f, m_window_size.y * 0.5f);
    Vec2 raising_panel_size        = Vec2(224, 288);
    Rect raising_panel_altas_rect  = Rect(0, 0, 96, 96);
    Rect raising_panel_center_rect = Rect(15, 15, 96 - 15 * 2, 96 - 15 * 2);
    m_raising_panel = Scale9Sprite::create("Game/UI/Base.png", raising_panel_altas_rect, raising_panel_center_rect);
    m_raising_panel->setCascadeOpacityEnabled(true);
    m_raising_panel->setPosition(raising_panel_position);
    m_raising_panel->setContentSize(raising_panel_size);
    this->addChild(m_raising_panel);

    m_panel_size           = m_raising_panel->getContentSize();

    Vec2 pattern_position  = Vec2(m_panel_size.x * 0.5f, m_panel_size.y - 28);
    Vec2 pattern_size      = Vec2(166, 16);
    Sprite* pattern_sprite = Sprite::create("Game/UI/Top Pattern 1.png");
    pattern_sprite->setPosition(pattern_position);
    pattern_sprite->setContentSize(pattern_size);
    m_raising_panel->addChild(pattern_sprite);

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
            m_raising_panel->addChild(corner_sprite);
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
    m_raising_panel->addChild(close_button);
}

void RaisingUI::CreateTitleGroup()
{
    Rect name_panel_sprite_rect = Rect(125, 56, 64, 16);
    Rect name_panel_center_rect = Rect(4, 2, 64 - 4 * 2, 16 - 2 * 2);
    Vec2 name_panel_position    = Vec2(m_panel_size.x * 0.5f, m_panel_size.y - 4);
    Size name_panel_size        = Size(96, 24);
    Scale9Sprite* name_panel = Scale9Sprite::create("Game/UI/Base.png", name_panel_sprite_rect, name_panel_center_rect);
    name_panel->setCascadeOpacityEnabled(true);
    name_panel->setContentSize(name_panel_size);
    name_panel->setPosition(name_panel_position);
    m_raising_panel->addChild(name_panel);

    const Vec2 NAME_PANEL_SIZE = name_panel->getContentSize();

    Vec2 title_label_position = Vec2(NAME_PANEL_SIZE.x * 0.5f, NAME_PANEL_SIZE.y * 0.5f);
    Color32 title_label_color = Color32(186, 145, 88);
    Label* title_label        = Label::createWithTTF("ATTRIBUTE", "fonts/exqt.ttf", 16);
    title_label->setCascadeOpacityEnabled(true);
    title_label->setColor(title_label_color);
    title_label->setPosition(title_label_position);
    name_panel->addChild(title_label);
}

void RaisingUI::CreateScrollView()
{
    Vec2 scroll_view_size     = Vec2(204, 224);
    Vec2 scroll_view_position = Vec2(m_panel_size.x * 0.5f, m_panel_size.y * 0.5f - 10);
    m_scroll_view             = ScrollView::create();
    m_scroll_view->setDirection(ScrollView::Direction::VERTICAL);
    m_scroll_view->setBounceEnabled(false);
    m_scroll_view->setScrollBarEnabled(false);
    m_scroll_view->setContentSize(scroll_view_size);
    m_scroll_view->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_scroll_view->setPosition(scroll_view_position);
    m_raising_panel->addChild(m_scroll_view);
}

void RaisingUI::InitUI()
{
    int slot_count       = static_cast<int>(m_raising_data_list.size());
    float content_height = slot_count * SLOT_HEIGHT + (slot_count + 1) * SLOT_SPACE;
    float view_height    = m_scroll_view->getContentSize().height;

    content_height = std::max(content_height, view_height);

    Size container_size = Size(216, content_height);
    m_scroll_view->setInnerContainerSize(container_size);

    Layout* inner = m_scroll_view->getInnerContainer();

    for (const RaisingData& raising_data : m_raising_data_list)
    {
        RaisingItem* raising_slot = RaisingItem::create(m_money_service, m_raising_service);
        raising_slot->InitUI(raising_data);

        int index = static_cast<int>(m_raising_slot_list.size());

        float x = container_size.width * 0.5f - 6;
        float y = content_height - index * (SLOT_HEIGHT + SLOT_SPACE) - SLOT_HEIGHT * 0.5f - SLOT_SPACE;

        raising_slot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        raising_slot->setPosition(Vec2(x, y));

        inner->addChild(raising_slot);
        m_raising_slot_list.push_back(raising_slot);
    }
}
