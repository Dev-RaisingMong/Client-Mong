#include "ShopDB.h"

static std::vector<ShopData> s_item_list;
static std::unordered_map<MongCode, const ShopData*> s_item_dict;

static bool s_initialized = false;

void ShopDB::Initialize()
{
    if (s_initialized)
        return;

    s_initialized = true;

    s_item_list =
    {
        { MongCode::GREEN_MONG, 50 },
        { MongCode::RED_MONG, 500 },
        { MongCode::PINK_MONG, 5000 },
        { MongCode::BLUE_MONG, 50000 }
    };

    for (const ShopData& item : s_item_list)
        s_item_dict[item.m_mong_code] = &item;
}

const ShopData* ShopDB::GetByID(MongCode mong_code)
{
    Initialize();

    auto iterator = s_item_dict.find(mong_code);
    if (iterator != s_item_dict.end())
        return s_item_dict[mong_code];

    return nullptr;
}

const std::vector<ShopData>& ShopDB::GetAll()
{
    Initialize();

    return s_item_list;
}
