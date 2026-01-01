#pragma once

#include <vector>
#include "MongData.h"

class MongDB
{
public:
    static const MongData* GetByID(MongCode mong_code);
    static const std::vector<MongData>& GetAll();

private:
    static void Initialize();
};
