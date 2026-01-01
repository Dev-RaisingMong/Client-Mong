#include "MongService.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

USING_NS_AX;

MongService* MongService::create(Game* game)
{
    MongService* mong_service = new (std::nothrow) MongService();

    if (mong_service && mong_service->init(game))
    {
        mong_service->autorelease();
        return mong_service;
    }

    AX_SAFE_DELETE(mong_service);
    return nullptr;
}

bool MongService::init(Game* game)
{
    if (!Node::init())
        return false;

    m_game = game;

    Load();

    return true;
}

void MongService::onExit()
{
    Save();
    Node::onExit();
}

void MongService::RestoreToScene()
{
    if (!m_game)
        return;

    for (const auto& data : m_save_data_list)
        m_game->RestoreMong(data.m_code, data.m_satiety, data.m_position);
}

void MongService::Load()
{
    std::string path = FileUtils::getInstance()->getWritablePath() + "mong_save.json";

    if (!FileUtils::getInstance()->isFileExist(path))
        return;

    std::string json = FileUtils::getInstance()->getStringFromFile(path);

    rapidjson::Document doc;
    doc.Parse(json.c_str());

    if (!doc.IsObject() || !doc.HasMember("mongs"))
        return;

    m_save_data_list.clear();

    const rapidjson::Value& mong_array = doc["mongs"];
    if (!mong_array.IsArray())
        return;

    for (const auto& v : mong_array.GetArray())
    {
        MongSaveData data;
        data.m_code    = static_cast<MongCode>(v["code"].GetInt());
        data.m_satiety = v["satiety"].GetInt();

        const auto& position = v["position"];
        data.m_position.x    = position["x"].GetFloat();
        data.m_position.y    = position["y"].GetFloat();

        m_save_data_list.push_back(data);
    }
}

void MongService::Save()
{
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    rapidjson::Value mong_array(rapidjson::kArrayType);

    for (const auto& data : m_game->GetMongList())
    {
        rapidjson::Value mong(rapidjson::kObjectType);

        MongSaveData save_data = data->GetMongSaveData();

        mong.AddMember("code", static_cast<int>(save_data.m_code), allocator);
        mong.AddMember("satiety", save_data.m_satiety, allocator);

        rapidjson::Value position(rapidjson::kObjectType);

        position.AddMember("x", save_data.m_position.x, allocator);
        position.AddMember("y", save_data.m_position.y, allocator);

        mong.AddMember("position", position, allocator);

        mong_array.PushBack(mong, allocator);
    }

    doc.AddMember("mongs", mong_array, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::string path = FileUtils::getInstance()->getWritablePath() + "mong_save.json";
    FileUtils::getInstance()->writeStringToFile(buffer.GetString(), path);
}
