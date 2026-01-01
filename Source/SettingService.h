#pragma once

#include "axmol/axmol.h"

using VolumeChangedCallback = std::function<void(float)>;
using PrintChangedCallback = std::function<void(bool)>;

class SettingService : public ax::Node
{
public:
    CREATE_FUNC(SettingService)
    bool init() override;
    void onExit() override;

public:
    void SetBGMPrint(bool active);
    void SetBGMVolume(float volume);
    void SetBGMPrintChanged(PrintChangedCallback changed_callback);
    void SetBGMVolumeChanged(VolumeChangedCallback changed_callback);

    void SetSFXPrint(bool active);
    void SetSFXVolume(float volume);

    bool IsBGMPrint() const;
    float GetBGMVolume() const;
    float GetEffectiveBGMVolume() const;

    bool IsSFXPrint() const;
    float GetSFXVolume() const;
    float GetEffectiveSFXVolume() const;

private:
    void Load();
    void Save() const;

private:
    bool m_bgm_enabled;
    float m_bgm_volume;

    bool m_sfx_enabled;
    float m_sfx_volume;

    VolumeChangedCallback m_bgm_volume_changed_callback;
    PrintChangedCallback m_bgm_print_changed_callback;
};
