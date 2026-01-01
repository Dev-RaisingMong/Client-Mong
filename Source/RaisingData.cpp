#include "RaisingData.h"

Money RaisingData::GetCost(int upgrade_level) const
{
    return m_default_cost + m_growth_cost * upgrade_level;
}
