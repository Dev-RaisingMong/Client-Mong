#pragma once

#include "axmol/axmol.h"
#include "axmol/ui/UIScrollView.h"
#include "MoneyService.h"
#include "RaisingService.h"
#include "RaisingData.h"

class RaisingService;
class RaisingItem;

class RaisingUI : public ax::Layer
{
public:
    CREATE_FUNC(RaisingUI)
    virtual bool init() override;

public:
    void OpenUI();
    void CloseUI();
    void Inject(MoneyService* money_service, RaisingService* raising_service);

private:
    void ToggleUI(bool active);
    void InitUI();
    void CreateUI();
    void CreateRaisingPanel();
    void CreateTitleGroup();
    void CreateScrollView();

private:
    ax::Size m_window_size;
    ax::Size m_panel_size;

    const ax::Vec2 START_POSITION = Vec2(0, 75);
    const float SLOT_HEIGHT = 48.0f;
    const float SLOT_SPACE  = 6.0f;

    ax::Node* m_raising_panel;
    ax::ui::ScrollView* m_scroll_view;

private:
    MoneyService* m_money_service;
    RaisingService* m_raising_service;
    std::vector<RaisingData> m_raising_data_list;
    std::vector<RaisingItem*> m_raising_slot_list;
};
