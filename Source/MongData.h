#pragma once

#include <string>
#include "axmol/axmol.h"

using ID = int;
using Money = int;

enum class MongCode
{
    GREEN_MONG = 0,
    RED_MONG,
    PINK_MONG,
    BLUE_MONG,
};

struct MongData
{
public:
    Money GetTouchCost() const;
    Money GetSaleCost(int upgrade_level) const;

public:
    MongCode m_code;
    std::string m_name;

    ax::Rect m_altas_rect;

    Money m_touch_cost;

    Money m_sale_cost_base;
    Money m_sale_cost_delta;

    int m_max_satiety;
};
