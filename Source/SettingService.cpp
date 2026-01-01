#include "SettingService.h"

USING_NS_AX;

bool SettingService::init()
{
    if (!Node::init())
        return false;

    Load();

    return true;
}

void SettingService::onExit()
{
    Save();
    Node::onExit();
}

void SettingService::SetBGMPrint(bool active)
{
    m_bgm_enabled = active;

    if (m_bgm_print_changed_callback)
        m_bgm_print_changed_callback(active);
}

void SettingService::SetBGMVolume(float volume)
{
    m_bgm_volume = clampf(volume, 0.0f, 1.0f);

    if (m_bgm_volume_changed_callback)
        m_bgm_volume_changed_callback(m_bgm_volume);
}

void SettingService::SetBGMPrintChanged(PrintChangedCallback changed_callback) { m_bgm_print_changed_callback = changed_callback; }

void SettingService::SetBGMVolumeChanged(VolumeChangedCallback changed_callback) { m_bgm_volume_changed_callback = changed_callback; }

void SettingService::SetSFXPrint(bool active) { m_sfx_enabled = active; }

void SettingService::SetSFXVolume(float volume) { m_sfx_volume = clampf(volume, 0.0f, 1.0f); }

bool SettingService::IsBGMPrint() const { return m_bgm_enabled; }

float SettingService::GetBGMVolume() const { return m_bgm_volume; }

float SettingService::GetEffectiveBGMVolume() const { return m_bgm_enabled ? m_bgm_volume : 0.0f; }

bool SettingService::IsSFXPrint() const { return m_sfx_enabled; }

float SettingService::GetSFXVolume() const { return m_sfx_volume; }

float SettingService::GetEffectiveSFXVolume() const { return m_sfx_enabled ? m_sfx_volume : 0.0f; }

void SettingService::Load()
{
    UserDefault* user_default = UserDefault::getInstance();

    m_bgm_enabled = user_default->getBoolForKey("BGM_ENABLED", true);
    m_bgm_volume  = user_default->getFloatForKey("BGM_VOLUME", 0.75f);

    m_sfx_enabled = user_default->getBoolForKey("SFX_ENABLED", true);
    m_sfx_volume  = user_default->getFloatForKey("SFX_VOLUME", 1.0f);
}

void SettingService::Save() const
{
    UserDefault* user_default = UserDefault::getInstance();

    user_default->setBoolForKey("BGM_ENABLED", m_bgm_enabled);
    user_default->setFloatForKey("BGM_VOLUME", m_bgm_volume);

    user_default->setBoolForKey("SFX_ENABLED", m_sfx_enabled);
    user_default->setFloatForKey("SFX_VOLUME", m_sfx_volume);
}
