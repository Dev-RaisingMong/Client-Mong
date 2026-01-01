#pragma once

#include <iostream>
#include "axmol/axmol.h"
#include "RaisingType.h"

using Money = int;
USING_NS_AX;

struct RaisingData
{
public:
    Money GetCost(int upgrade_level) const;

public:
    RaisingType m_raising_type;

    std::string m_raising_name;
    std::string m_raising_description;

    Rect m_atlas_rect;

    Money m_default_cost;
    Money m_growth_cost;
};
