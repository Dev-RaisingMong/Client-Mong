#pragma once

#include <vector>
#include "axmol/axmol.h"
#include "RaisingData.h"

class RaisingDB
{
public:
    static const RaisingData* GetByType(RaisingType raising_type);
    static const std::vector<RaisingData>& GetAll();

private:
    static void Initialize();
};
