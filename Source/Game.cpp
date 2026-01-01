#include "axmol/ui/UIButton.h"
#include "Game.h"
#include "ToggleUI.h"
#include "ShopUI.h"
#include "RaisingUI.h"
#include "SettingUI.h"
#include "SaleUI.h"
#include "ObjectPoolManager.h"
#include "AudioManager.h"
#include "FloatingText.h"
#include "MongDB.h"

using namespace ax::ui;
USING_NS_AX;

bool Game::init()
{
    if (!Scene::init())
    {
        return false;
    }

    m_window_size = _director->getCanvasSize();

    InitLayers();
    CreateTileMap();
    InitDefendency();
    CreateSetting();
    CreateLightParticle();

    return true;
}

void Game::onEnter()
{
    Scene::onEnter();

    m_mong_service->RestoreToScene();
}

void Game::InitLayers()
{
    m_background_layer = Layer::create();
    this->addChild(m_background_layer, LAYER::BACKGROUND_LAYER);

    m_game_layer = Layer::create();
    this->addChild(m_game_layer, LAYER::GAME_LAYER);

    m_ui_layer = Layer::create();
    this->addChild(m_ui_layer, LAYER::UI_LAYER);

    m_setting_layer = Layer::create();
    this->addChild(m_setting_layer, LAYER::SETTING_LAYER);
}

void Game::InitDefendency()
{
    ObjectPoolManager::Initialize();

    m_setting_service = SettingService::create();
    m_setting_layer->addChild(m_setting_service);

    AudioManager::getInstance()->Inject(m_setting_service);
    AudioManager::getInstance()->PlayBGM("Audios/BGM/Game BGM.mp3");

    MoneyUI* money_ui = MoneyUI::create();
    m_ui_layer->addChild(money_ui, 1, UI::MONEY_UI);

    m_money_service = MoneyService::create(money_ui);
    m_game_layer->addChild(m_money_service);

    ShopUI* shop_ui = ShopUI::create();
    m_ui_layer->addChild(shop_ui, 1, UI::SHOP_UI);

    RaisingUI* raising_ui = RaisingUI::create();
    m_ui_layer->addChild(raising_ui, 1, UI::RAISING_UI);

    m_sale_ui = SaleUI::create();
    m_ui_layer->addChild(m_sale_ui, 1, UI::SALE_UI);

    SettingUI* setting_ui = SettingUI::create();
    m_setting_layer->addChild(setting_ui, 1, UI::SETTING_UI);

    m_raising_service = RaisingService::create(raising_ui);
    m_game_layer->addChild(m_raising_service);
  
    ToggleUI* toggle_ui = ToggleUI::create();
    m_ui_layer->addChild(toggle_ui, 1, UI::TOGGLE_UI);

    toggle_ui->Inject(shop_ui, raising_ui);
    m_sale_ui->Inject(this, m_raising_service, m_money_service);
    shop_ui->Inject(this, m_tilemap, m_raising_service, m_money_service, m_sale_ui);
    raising_ui->Inject(m_money_service, m_raising_service);
    setting_ui->Inject(m_setting_service);

    m_mong_service = MongService::create(this);
    m_game_layer->addChild(m_mong_service);
}

void Game::CreateTileMap()
{
    m_tilemap = TMXTiledMap::create("Tile/Map.tmx");
    Point game_tilemap_position = Point(m_window_size.x * 0.5f, m_window_size.y * 0.5f);
    m_tilemap->setAnchorPoint(Vec2(0.5f, 0.5f));
    m_tilemap->setPosition(game_tilemap_position);
    m_tilemap->setScale(2.0f);
    m_background_layer->addChild(m_tilemap);

    TMXLayer* meta_layer = m_tilemap->getLayer("Meta");
    meta_layer->setVisible(false);
}

void Game::CreateLightParticle()
{
    ParticleFlower* light_emitter = ParticleFlower::create();
    light_emitter->setDuration(ParticleSystem::DURATION_INFINITY);
    light_emitter->setEmitterMode(ParticleSystem::Mode::RADIUS);
    light_emitter->setStartRadius(500.0f);
    light_emitter->setStartRadiusVar(150.0f);
    light_emitter->setEndRadius(150.0f);
    light_emitter->setEmissionRate(8.0f);
    m_background_layer->addChild(light_emitter);
}

void Game::CreateSetting()
{
    Vec2 setting_button_position = Vec2(m_window_size.x - 25, m_window_size.y -25);
    Vec2 setting_button_size     = Vec2(32, 32);
    Button* setting_button = Button::create("Game/UI/Setting Button.png");
    setting_button->setPosition(setting_button_position);
    setting_button->setScale(0.5f);
    setting_button->addClickEventListener([this](Object*)
    {
        SettingUI* setting_ui = static_cast<SettingUI*>(m_setting_layer->getChildByTag(UI::SETTING_UI));
        if (setting_ui)
        {
            AudioManager::getInstance()->PlaySFX("Audios/SFX/Main Button Click.mp3");

            m_game_layer->pause();
            m_ui_layer->pause();
            setting_ui->OpenUI();
        }
    });

    this->addChild(setting_button, UI::SETTING_BUTTON);
}

void Game::AddMong(Mong* mong)
{
    m_mong_vector.pushBack(mong);
}

void Game::RemoveMong(Mong* mong)
{
    int index = static_cast<int>(m_mong_vector.getIndex(mong));
    m_mong_vector.erase(index);
}

int Game::GetMongCount() const
{
    return m_mong_vector.size();
}

void Game::RestoreMong(MongCode code, int satiety, ax::Vec2 position)
{
    Mong* new_mong = dynamic_cast<Mong*>(ObjectPoolManager::Get(ObjectType::MONG));
    this->addChild(new_mong);
    AddMong(new_mong);
    new_mong->Inject(m_tilemap, m_raising_service, m_money_service, MongDB::GetByID(code), this, m_sale_ui, satiety);
    new_mong->setPosition(position);
}

const ax::Vector<Mong*> Game::GetMongList() const
{
    return m_mong_vector;
}

void Game::Notify(std::string notice_string, Color32 color)
{
    Vec2 floating_text_position = Vec2(m_window_size.x * 0.5f, 50);
    FloatingText* floating_text = static_cast<FloatingText*>(ObjectPoolManager::Get(ObjectType::FLOATING_TEXT));
    m_ui_layer->addChild(floating_text);

    floating_text->Show(notice_string, floating_text_position, color);
}
