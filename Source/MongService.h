#pragma once

#include <vector>
#include "axmol/axmol.h"
#include "MongSaveData.h"
#include "Game.h"

class Game;

class MongService : public ax::Node
{
public:
    static MongService* create(Game* game);
    bool virtual init(Game* game);
    void onExit() override;

    void RestoreToScene();

private:
    void Load();
    void Save();

private:
    Game* m_game;
    std::vector<MongSaveData> m_save_data_list;
};
