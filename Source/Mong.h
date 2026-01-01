#pragma once

#include <unordered_map>
#include "axmol/axmol.h"
#include "MongState.h"
#include "MongStateContext.h"
#include "MongData.h"
#include "IState.h"
#include "IPoolable.h"
#include "RaisingService.h"
#include "SaleUI.h"
#include "Game.h"
#include "MongSaveData.h"

USING_NS_AX;

class MongStateContext;
class SaleUI;

enum class Direction
{
    DOWN = 0,
    LEFT,
    RIGHT,
    UP
};

class Mong : public Node, public IPoolable
{
public:
    ~Mong();
    bool init() override;
    CREATE_FUNC(Mong);

    void update(float dt) override;
    void onEnter() override;
    void onExit() override;
    void Inject(TMXTiledMap* tilemap,
                RaisingService* raising_service,
                MoneyService* money_service,
                const MongData* mong_data,
                Game* game,
                SaleUI* sale_ui,
                int satiety = 0);

public:
    void OnEnable() override;
    void OnDisable() override;
    ObjectType GetPoolType() const override;

public:
    virtual void ChangeState(MongState state);
    void Initialize();
    Animation* CreateAnimation(Texture2D* texture, Direction direction, Rect frame_rect, int frame_count, Size frame_size, float frame_delay);
    Direction CalculateDirection();
    void PlayMoveAnime();
    void PlayIdleAnime();
    void SetVelocity(Vec2 velocity);
    Vec2 GetVelocity();

    Vec2 GetRandomPointInRadius(float radius);
    bool CheckDestination(Vec2 position);
    bool MoveToDestination(Vec2 direction, float dt);

    MongSaveData GetMongSaveData();

private:
    Point TileCoordForPosition(Point position);
    void PlayTouchAnimation();
    void PlayTouchEffect();
    void UpdateSatiety();
    void CreateSatietyLabel();
    void ShowSatietyText();

private:
    MongStateContext* m_state_context;
    IState<Mong>* m_idle_state;
    IState<Mong>* m_wander_state;
    IState<Mong>* m_dragged_state;

private:
    std::unordered_map<Direction, RepeatForever*> m_idle_anime_map;
    std::unordered_map<Direction, RepeatForever*> m_move_anime_map;
    Sprite* m_sprite;
    Label* m_satiety_label;
    bool m_satiety_label_visible;
    Vec2 m_velocity;
    int m_current_satiety;
    TMXTiledMap* m_tilemap;
    const MongData* m_mong_data;
    MoneyService* m_money_service;
    RaisingService* m_raising_service;
    EventListenerTouchOneByOne* m_touch_listener;
    bool m_is_dragging;
    bool m_drag_candidate;
    Vec2 m_touch_start_local;
    Vec2 m_touch_start_world;
    Vec2 m_drag_offset;
    SaleUI* m_sale_ui;
    Game* m_game;
};
