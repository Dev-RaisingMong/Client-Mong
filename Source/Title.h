#pragma once

#include "axmol/axmol.h"

class Title : public ax::Scene
{
public:
    bool init() override;
    void onEnter() override;
    void onExit() override;

private:
    void Initialize();
    void CreateTileMap();
    void CreateUI();
    void CreateLightParticle();

private:
    ax::Layer* m_background_layer;
    ax::Vec2 m_window_size;
    ax::EventListenerTouchOneByOne* m_single_touch_listener;
};
