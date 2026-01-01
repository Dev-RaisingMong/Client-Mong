#include <unordered_map>
#include "RaisingDB.h"

static std::vector<RaisingData> s_data_list;
static std::unordered_map<RaisingType, const RaisingData*> s_data_dict;

static bool s_initialized = false;

void RaisingDB::Initialize()
{
    if (s_initialized)
        return;

    s_initialized = true;

    s_data_list =
    {
        { RaisingType::GREEN_MONG_DECORATION, "초록몽이의 꽃단장", "판매가 상승 +10", Rect(0, 0, 32, 32), 100, 1 },
        { RaisingType::RED_MONG_DECORTAION, "빨간몽이의 꽃단장", "판매가 상승 +100", Rect(32, 0, 32, 32), 1000, 10 },
        { RaisingType::PINK_MONG_DECORATION, "핑크몽이의 꽃단장", "판매가 상승 +1A", Rect(64, 0, 32, 32), 10000, 100 },
        { RaisingType::BLUE_MONG_DECORATION, "파랑몽이의 꽃단장", "판매가 상승 +10A", Rect(96, 0, 32, 32), 100000, 1000 },
        { RaisingType::MONG_APARTMENT, "몽이 아파트", "최대 개체 수 +1", Rect(128, 0, 32, 32), 1000, 5000 },
        { RaisingType::AWESOME_FEEDSTUFF, "질 좋은 사료", "사료 효율 +1", Rect(160, 0, 32, 32), 100000, 100000 },
    };

    for (const RaisingData& item : s_data_list)
        s_data_dict[item.m_raising_type] = &item;
}

const RaisingData* RaisingDB::GetByType(RaisingType raising_type)
{
    Initialize();

    auto iterator = s_data_dict.find(raising_type);
    if (iterator != s_data_dict.end())
        return s_data_dict[raising_type];

    return nullptr;
}

const std::vector<RaisingData>& RaisingDB::GetAll()
{
    Initialize();

    return s_data_list;
}
