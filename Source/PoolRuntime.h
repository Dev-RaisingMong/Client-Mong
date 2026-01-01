#pragma once

#include <vector>
#include "axmol/axmol.h"

USING_NS_AX;

struct PoolRuntime
{
    std::vector<Node*> m_pool;
    int m_current_count = 0;
};
