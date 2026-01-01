#pragma once

#include "ObjectType.h"

struct PoolInfo
{
    ObjectType m_type;
    int m_initial_count;
    int m_max_count;
    bool m_can_expand;
};

static const PoolInfo POOL_INFO_TABLE[] =
{
    { ObjectType::MONG, 10, 30, true },
    { ObjectType::FLOATING_TEXT, 5, 5, false },
};
