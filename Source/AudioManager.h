#pragma once

#include "axmol/axmol.h"
#include "axmol/audio/AudioEngine.h"
#include "SettingService.h"
#include "Singleton.h"

USING_NS_AX;

class AudioManager : public Singleton<AudioManager>
{
public:
    CREATE_FUNC(AudioManager)
    bool init() override;
    void Inject(SettingService* setting_service);

    void PlayBGM(const std::string& bgm_path, float fadee_time = 0.5f);
    void PlaySFX(const std::string& sfx_path);

public:
    void UpdateBGMVolume();
    void OnBGMVolumeChanged(float volume);

private:
    void SwapAndFadeInBGM(float duration);
    void FadeInCurrentBGM(float duration);
    void FadeOutCurrentBGM(float duration);
    void StopAllBGM(bool immediately);
    void Schedule(const std::function<void(float)>& callback, float interval, const std::string& key);
    void Unschedule(const std::string& key);

private:
    SettingService* m_setting_service;
    int m_current_bgm_id;
    int m_next_bgm_id;
};
