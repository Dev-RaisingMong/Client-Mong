#include "axmol/ui/UIScale9Sprite.h"
#include "Title.h"
#include "Game.h"
#include "SettingService.h"
#include "AudioManager.h"

using namespace ax::ui;
USING_NS_AX;

bool Title::init()
{
    if (!Scene::init())
    {
        return false;
    }

    Initialize();
    CreateTileMap();
    CreateLightParticle();
    CreateUI();

    return true;
}

void Title::onEnter()
{
    Scene::onEnter();

    m_single_touch_listener = EventListenerTouchOneByOne::create();
    m_single_touch_listener->onTouchBegan = [=](Touch*, Event*)
    {
        Game* game_scene                  = utils::createInstance<Game>();
        TransitionFadeDown* scene_transition = TransitionFadeDown::create(1.0f, game_scene);

        _director->replaceScene(scene_transition);

        return true;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_single_touch_listener, this);
}

void Title::onExit()
{
    _eventDispatcher->removeEventListener(m_single_touch_listener);
    Scene::onExit();
}

void Title::Initialize()
{
    m_window_size      = _director->getCanvasSize();

    m_background_layer = Layer::create();
    this->addChild(m_background_layer);

    SettingService* setting_service = SettingService::create();
    this->addChild(setting_service);

    AudioManager::getInstance()->Inject(setting_service);
    AudioManager::getInstance()->PlayBGM("Audios/BGM/Title BGM.mp3");
}

void Title::CreateTileMap()
{
    TMXTiledMap* title_tilemap = TMXTiledMap::create("Tile/Title Map.tmx");
    Point tilemap_position = Point(m_window_size.x * 0.5f, m_window_size.y * 0.5f);
    title_tilemap->setAnchorPoint(Vec2(0.5f, 0.5f));
    title_tilemap->setPosition(tilemap_position);
    title_tilemap->setScale(2.0f);
    m_background_layer->addChild(title_tilemap);
}

void Title::CreateUI()
{
    Vec2 title_image_size      = Vec2(306, 204);
    Sprite* title_image = Sprite::create("Title/Title Text Image 2.png");
    Point title_image_position = Point(m_window_size.x * 0.5f, m_window_size.y * 0.5f);
    title_image->setPosition(title_image_position);
    title_image->setScale(0.75f);
    m_background_layer->addChild(title_image);
}

void Title::CreateLightParticle()
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
