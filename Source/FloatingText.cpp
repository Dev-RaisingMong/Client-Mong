#include "FloatingText.h"
#include "ObjectPoolManager.h"

bool FloatingText::init()
{
    if (!Node::init())
        return false;

    m_label = Label::createWithTTF("", "fonts/exqt.ttf", 14);
    m_label->getFontAtlas()->setAliasTexParameters();
    m_label->setAnchorPoint(Vec2(0.5f, 0.5f));
    addChild(m_label);

    setVisible(false);
    return true;
}

void FloatingText::Show(const std::string& text, const Vec2& world_pos, const Color32& color)
{
    setPosition(world_pos);

    m_label->setPosition(Vec2::ZERO);
    m_label->setString(text);
    m_label->setColor(color);
    m_label->setOpacity(0);

    setScale(1.0f);
    setVisible(true);

    PlayAnimation();
}

void FloatingText::PlayAnimation()
{
    stopAllActions();

    auto fade_in  = FadeIn::create(1.0f);
    auto move_up  = MoveBy::create(0.5f, Vec2(0, 24));
    auto fade_out = FadeOut::create(1.0f);

    auto seq = Sequence::create(fade_in, Spawn::create(move_up, fade_out, nullptr),
                                CallFunc::create([this]() { ObjectPoolManager::Release(this); }), nullptr);

    m_label->runAction(seq);
}

void FloatingText::OnEnable()
{
    setVisible(true);
}

void FloatingText::OnDisable()
{
    stopAllActions();
    setVisible(false);
}

ObjectType FloatingText::GetPoolType() const
{
    return ObjectType::FLOATING_TEXT;
}
