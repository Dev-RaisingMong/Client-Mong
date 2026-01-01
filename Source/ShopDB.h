#pragma once

#include <vector>
#include "ShopData.h"

class ShopDB
{
public:
    static const ShopData* GetByID(MongCode mong_code);
    static const std::vector<ShopData>& GetAll();

private:
    static void Initialize();
};
