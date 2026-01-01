#include <unordered_map>
#include "MongDB.h"

USING_NS_AX;

static std::vector<MongData> s_mong_list;
static std::unordered_map<MongCode, const MongData*> s_mong_dict;

static bool s_initialized = false;

void MongDB::Initialize()
{
    if (s_initialized)
        return;

    s_initialized = true;

    s_mong_list =
    {
        { MongCode::GREEN_MONG, "초록몽이", Rect(0, 0, 32 * 4, 32 * 4), 1, 100, 10, 10 },
        { MongCode::RED_MONG, "빨간몽이", Rect(128, 0, 32 * 4, 32 * 4), 10, 1000, 100, 20 },
        { MongCode::PINK_MONG, "핑크몽이", Rect(256, 0, 32 * 4, 32 * 4), 100, 10000, 1000, 30 },
        { MongCode::BLUE_MONG, "파랑몽이", Rect(384, 0, 32 * 4, 32 * 4), 1000, 100000, 10000, 40 },
    };

    for (const MongData& mong : s_mong_list)
        s_mong_dict[mong.m_code] = &mong;
};

const MongData* MongDB::GetByID(MongCode mong_code)
{
    Initialize();

    auto iterator = s_mong_dict.find(mong_code);
    if (iterator != s_mong_dict.end())
        return s_mong_dict[mong_code];

    return nullptr;
}

const std::vector<MongData>& MongDB::GetAll()
{
    Initialize();

    return s_mong_list;
}
