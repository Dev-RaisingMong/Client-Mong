#pragma once

#include "axmol/axmol.h"
#include "IPoolable.h"

USING_NS_AX;

class FloatingText : public Node, public IPoolable
{
public:
    CREATE_FUNC(FloatingText)
    bool init() override;

public:
    void Show(const std::string& text, const Vec2& world_pos, const Color32& color = Color32::WHITE);

    void OnEnable() override;
    void OnDisable() override;
    ObjectType GetPoolType() const override;

private:
    void PlayAnimation();

private:
    Label* m_label = nullptr;
};
