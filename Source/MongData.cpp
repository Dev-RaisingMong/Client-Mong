#include "MongData.h"

Money MongData::GetTouchCost() const
{
    return m_touch_cost;
}

Money MongData::GetSaleCost(int upgrade_level) const
{
    return m_sale_cost_base + m_sale_cost_delta * upgrade_level;
}
