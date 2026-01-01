#include "Mong.h"
#include "MongIdleState.h"
#include "MongWanderState.h"
#include "MongDraggedState.h"
#include "NumberFormatter.h"
#include "AudioManager.h"

Mong::~Mong()
{
    for (auto& pair : m_idle_anime_map)
        pair.second->release();

    for (auto& pair : m_move_anime_map)
        pair.second->release();
}

bool Mong::init()
{
    if (!Node::init())
        return false;

    m_state_context = new MongStateContext(this);

    m_idle_state    = new MongIdleState();
    m_wander_state  = new MongWanderState();
    m_dragged_state = new MongDraggedState();

    return true;
}

void Mong::onEnter()
{
    Node::onEnter();
    scheduleUpdate();
}

void Mong::onExit()
{
    //delete m_idle_state;
    //delete m_wander_state;
    //delete m_dragged_state;

    //delete m_state_context;

    unscheduleUpdate();
    Node::onExit();
}

void Mong::update(float dt)
{
    m_state_context->ExecuteUpdate(dt);
}


void Mong::OnEnable()
{
    setVisible(true);
    m_velocity = Vec2::ZERO;
}

void Mong::OnDisable()
{
    if (m_sprite)
        m_sprite->stopAllActions();

    if (m_touch_listener)
        _eventDispatcher->removeEventListener(m_touch_listener);

    if (m_satiety_label)
    {
        m_satiety_label->stopAllActions();
        m_satiety_label->setVisible(false);
        m_satiety_label_visible = false;
    }

    m_is_dragging = false;

    stopAllActions();
    m_state_context->Reset();
    setVisible(false);
}

ObjectType Mong::GetPoolType() const { return ObjectType::MONG; }

void Mong::Inject(TMXTiledMap* tilemap,
                  RaisingService* raising_service,
                  MoneyService* money_service,
                  const MongData* mong_data,
                  Game* game, 
                  SaleUI* sale_ui,
                  int satiety)
{
    m_tilemap         = tilemap;
    m_money_service   = money_service;
    m_raising_service = raising_service;
    m_mong_data       = mong_data;
    m_game            = game;
    m_sale_ui         = sale_ui;

    m_current_satiety = satiety;

    if (m_touch_listener == nullptr)
    {
        m_touch_listener = EventListenerTouchOneByOne::create();
        m_touch_listener->setSwallowTouches(true);
        m_touch_listener->retain();

        m_touch_listener->onTouchBegan = [this](Touch* touch, Event* unused_event)
        {
            if (!m_raising_service || !m_money_service)
                return false;

            Vec2 local = m_sprite->convertToNodeSpace(touch->getLocation());
            Rect rect(Vec2::ZERO, m_sprite->getContentSize());

            if (!rect.containsPoint(local))
                return false;

            m_touch_start_local = local;
            m_touch_start_world = getPosition();
            m_drag_candidate    = true;
            m_is_dragging       = false;

            return true;
        };

        m_touch_listener->onTouchMoved = [this](Touch* touch, Event* unused_event)
        {
            if (!m_drag_candidate)
                return;

            Vec2 local     = m_sprite->convertToNodeSpace(touch->getLocation());
            float distance = local.distance(m_touch_start_local);

            const float DRAG_THRESHOLD = 12.0f;

            if (!m_is_dragging && distance >= DRAG_THRESHOLD)
            {
                m_is_dragging = true;
                ChangeState(MongState::DRAGGED);
            }

            if (m_is_dragging)
            {
                Vec2 parentLocal = getParent()->convertToNodeSpace(touch->getLocation());
                this->setPosition(parentLocal + m_drag_offset);

                if (m_sale_ui)
                {
                    Rect sell_rect = m_sale_ui->GetWorldRect();
                    m_sale_ui->ToggleHightlight(sell_rect.containsPoint(touch->getLocation()));
                }               
            }
        };

        m_touch_listener->onTouchEnded = [this](Touch* touch, Event* unused_event)
        {
            if (!m_drag_candidate)
                return;

            if (!m_is_dragging)
            {
                if (m_current_satiety < m_mong_data->m_max_satiety)
                     m_money_service->UpdateMoney(-m_mong_data->GetTouchCost());
                UpdateSatiety();
                PlayTouchAnimation();

                AudioManager::getInstance()->PlaySFX("Audios/SFX/feed.mp3");
            }
            else
            {
                if (!m_sale_ui)
                    return;

                Rect sell_rect = m_sale_ui->GetWorldRect();

                if (sell_rect.containsPoint(touch->getLocation()))
                {
                    if (m_current_satiety >= m_mong_data->m_max_satiety)
                    {
                        m_sale_ui->Sell(this, m_mong_data);
                    }
                    else
                    {
                        m_game->Notify("성장기인 몽은 판매할 수 없습니다!", Color32::RED);
                        AudioManager::getInstance()->PlaySFX("Audios/SFX/Sale Fail.mp3");

                        m_sale_ui->ToggleHightlight(false);

                        if (CheckDestination(touch->getLocation()))
                            setPosition(touch->getLocation());
                        else
                            setPosition(m_touch_start_world);
                        ChangeState(MongState::IDLE);
                    }
                }
                else
                {
                    if (CheckDestination(touch->getLocation()))
                        setPosition(touch->getLocation());
                    else
                        setPosition(m_touch_start_world);
                    ChangeState(MongState::IDLE);
                }
            }

            m_drag_candidate = false;
            m_is_dragging    = false;
        };
    }

    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touch_listener, this);

    Initialize();
    ChangeState(MongState::IDLE);
}

void Mong::ChangeState(MongState state)
{
    switch (state)
    {
    case MongState::IDLE:
        m_state_context->Transition(m_idle_state);
        break;

    case MongState::WANDER:
        m_state_context->Transition(m_wander_state);
        break;

    case MongState::DRAGGED:
        m_state_context->Transition(m_dragged_state);
        break;
    }
}

void Mong::Initialize()
{
    Texture2D* texture = _director->getTextureCache()->addImage("Game/Mongs.png");

    if (m_sprite)
    {
        m_sprite->removeFromParent();
        m_sprite = nullptr;
    }

    m_sprite = Sprite::create("Game/Mongs.png", Rect(m_mong_data->m_altas_rect.origin.x, m_mong_data->m_altas_rect.origin.y, 32, 32));
    m_sprite->setPosition(Vec2(getContentSize().x * 0.5f, getContentSize().y * 0.5f));
    this->addChild(m_sprite);

    Size idle_frame_size = Size(32, 32);
    int idle_frame_count = 1;
    float idle_frame_delay = 0.24f;
    for (int direction = 0; direction < 4; direction++)
    {
        Direction current_direction = static_cast<Direction>(direction);

        Animation* anime = CreateAnimation(texture, current_direction, m_mong_data->m_altas_rect, idle_frame_count, idle_frame_size, idle_frame_delay);
        m_idle_anime_map[current_direction] = RepeatForever::create(Animate::create(anime));
        m_idle_anime_map[current_direction]->retain();
    }

    Size move_frame_size = Size(32, 32);
    int move_frame_count = 4;
    float move_frame_delay = 0.24f;
    for (int direction = 0; direction < 4; direction++)
    {
        Direction current_direction = static_cast<Direction>(direction);

        Animation* anime = CreateAnimation(texture, current_direction, m_mong_data->m_altas_rect, move_frame_count, move_frame_size, move_frame_delay);
        m_move_anime_map[current_direction] = RepeatForever::create(Animate::create(anime));
        m_move_anime_map[current_direction]->retain();
    }
}

Animation* Mong::CreateAnimation(Texture2D* texture, Direction direction, Rect frame_rect, int frame_count, Size frame_size, float frame_delay)
{
    Vector<SpriteFrame*> frames;

    int direction_index = static_cast<int>(direction);

    for (int i = 0; i < frame_count; i++)
    {
        Rect rect = Rect(frame_rect.origin.x + i * frame_size.width,
                         frame_rect.origin.y + direction_index * frame_size.height,
                         frame_size.width,
                         frame_size.height);

        SpriteFrame* frame = SpriteFrame::createWithTexture(texture, rect);
        frames.pushBack(frame);
    }

    return Animation::createWithSpriteFrames(frames, frame_delay);
}

Direction Mong::CalculateDirection()
{
    if (fabs(m_velocity.x) > fabs(m_velocity.y))
        return m_velocity.x > 0 ? Direction::RIGHT : Direction::LEFT;
    else
        return m_velocity.y > 0 ? Direction::UP : Direction::DOWN;
}

void Mong::PlayMoveAnime()
{
    m_sprite->stopAllActions();
    m_sprite->runAction(m_move_anime_map[CalculateDirection()]);
}

void Mong::PlayIdleAnime()
{
    m_sprite->stopAllActions();
    m_sprite->runAction(m_idle_anime_map[CalculateDirection()]);
}

void Mong::SetVelocity(Vec2 velocity)
{
    m_velocity = velocity;
}

Vec2 Mong::GetVelocity()
{
    return m_velocity;
}

Point Mong::TileCoordForPosition(Point position)
{
    float scaleX = m_tilemap->getScaleX();
    float scaleY = m_tilemap->getScaleY();

    float tileWidth  = m_tilemap->getTileSize().width * scaleX;
    float tileHeight = m_tilemap->getTileSize().height * scaleY;

    int x = position.x / tileWidth;
    int y = ((m_tilemap->getMapSize().height * tileHeight) - position.y) / tileHeight;

    return Point(x, y);
}

Vec2 Mong::GetRandomPointInRadius(float radius)
{
    float angle = RandomHelper::random_real(0.0f, static_cast<float>(M_PI) * 2.0f);
    float r     = sqrt(RandomHelper::random_real(0.0f, 1.0f)) * radius;

    float x = cos(angle) * r;
    float y = sin(angle) * r;

    return getPosition() + Vec2(x, y);
}

bool Mong::CheckDestination(Vec2 position)
{
    Point destination_position = TileCoordForPosition(position);

    TMXLayer* meta_layer = m_tilemap->getLayer("Meta");
    int tile_gid         = meta_layer->getTileGIDAt(destination_position);

    if (tile_gid)
    {
        Value properties = m_tilemap->getPropertiesForGID(tile_gid);
        if (properties.isNull())
        {
            AXLOGD("Tile GID에 해당하는 속성이 NULL입니다.");
            return false;
        }

        std::string wall = properties.asValueMap()["Obstacle"].asString();
        if (wall == "YES")
            return false;

        return true;
    }

    AXLOGD("Tile GID를 받아오는 데 실패했습니다.");
    return false;
}

bool Mong::MoveToDestination(Vec2 destination, float dt)
{
    Vec2 current_position = getPosition();
    Vec2 direction        = destination - current_position;

    float distance = direction.length();
    if (distance <= 0.1f)
        return true;

    direction.normalize();
    m_velocity = direction;

    float speed = 10.0f;
    Vec2 next_position = current_position + direction * speed * dt;

    if (!CheckDestination(next_position))
        return true;

    setPosition(next_position);

    return false;
}

MongSaveData Mong::GetMongSaveData()
{
    return MongSaveData
    {
        m_mong_data->m_code,
        m_current_satiety,
        getPosition()
    };
}

void Mong::PlayTouchAnimation()
{
    this->stopActionByTag(1);

    ScaleTo* scale_down = ScaleTo::create(0.05f, 0.9f);
    ScaleTo* scale_up   = ScaleTo::create(0.01f, 1.0f);

    Sequence* seq_action = Sequence::create(scale_down, scale_up, nullptr);
    seq_action->setTag(1);

    this->runAction(seq_action);
}

void Mong::PlayTouchEffect()
{

}

void Mong::UpdateSatiety()
{
    int add           = 1 + m_raising_service->GetRaising(RaisingType::AWESOME_FEEDSTUFF);
    m_current_satiety = std::min(m_current_satiety + add, m_mong_data->m_max_satiety);

    ShowSatietyText();
}

void Mong::ShowSatietyText()
{
    CreateSatietyLabel();

    Vec2 satiety_label_position = Vec2(0, m_sprite->getContentSize().height - 16);
    std::string text = fmt::format("성장도 [{} / {}]", NumberFormatter::Format(m_current_satiety), NumberFormatter::Format(m_mong_data->m_max_satiety));
    m_satiety_label->stopAllActions();
    m_satiety_label->setString(text);
    m_satiety_label->setPosition(satiety_label_position);
    m_satiety_label->setVisible(true);

    if (!m_satiety_label_visible)
    {
        m_satiety_label_visible = true;
        m_satiety_label->setOpacity(0);
        m_satiety_label->runAction(FadeIn::create(0.15f));
    }

    DelayTime* delay  = DelayTime::create(0.4f);
    FadeOut* fade_out = FadeOut::create(0.25f);
    CallFunc* hide    = CallFunc::create([this]()
    {
        m_satiety_label->setVisible(false);
        m_satiety_label_visible = false;
    });

    m_satiety_label->runAction(Sequence::create(delay, fade_out, hide, nullptr));
}

void Mong::CreateSatietyLabel()
{
    if (m_satiety_label)
        return;

    Color32 satiety_label_color = Color32::WHITE;
    Vec2 satiety_label_anchor   = Vec2(0.5f, 0.0f);
    m_satiety_label             = Label::createWithTTF("", "fonts/exqt.ttf", 12);
    m_satiety_label->setAnchorPoint(satiety_label_anchor);
    m_satiety_label->setColor(satiety_label_color);
    m_satiety_label->setOpacity(0);
    this->addChild(m_satiety_label);
}
