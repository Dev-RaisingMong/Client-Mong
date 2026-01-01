#pragma once

#include "axmol/axmol.h"
#include "RaisingUI.h"
#include "RaisingType.h"

class RaisingUI;

class RaisingService : public ax::Node
{
public:
    static RaisingService* create(RaisingUI* raising_ui);
    virtual bool init(RaisingUI* raising_ui);
    void onEnter() override;
    void onExit() override;

public:
    void UpdateRaising(RaisingType raising_type, int raising_count = 1);
    int GetRaising(RaisingType raising_type) const;

private:
    void Load();
    void Save() const;

private:
    RaisingUI* m_raising_ui;

    int m_green_mong_decoration;
    int m_red_mong_decoration;
    int m_pink_mong_decortaion;
    int m_blue_mong_decoration;
    int m_mong_apartment;
    int m_awesome_feedstuff;
};
