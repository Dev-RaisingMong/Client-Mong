#pragma once

#include "axmol/axmol.h"
#include "axmol/ui/UIButton.h"
#include "MoneyService.h"
#include "RaisingService.h"
#include "RaisingData.h"

class RaisingService;

class RaisingItem : public ax::Layer
{
public:
    static RaisingItem* create(MoneyService* money_service, RaisingService* raising_service);
    virtual bool init(MoneyService* money_service, RaisingService* raising_service);
    void onEnter() override;

public:
    void InitUI(RaisingData raising_data);

private:
    void UpdateUI();

    void CreateUI();
    void CreateItemPanel();
    void CreatePortraitGroup();
    void CreateDescriptionGroup();
    void CreateRaisingButton();

    void OnClickedRaising();

private:
    ax::Size m_item_size;

    ax::Node* m_raising_item_panel;
    ax::Sprite* m_portrait_image;
    ax::Label* m_name_label;
    ax::Label* m_description_label;
    ax::Label* m_level_label;
    ax::ui::Button* m_raising_button;

private:
    MoneyService* m_money_service;
    RaisingService* m_raising_service;
    RaisingData m_raising_data;
    ax::EventListenerCustom* m_money_update_event_listener;
    ax::EventListenerCustom* m_raising_load_event_listener;
};
