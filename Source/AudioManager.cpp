#include "AudioManager.h"

bool AudioManager::init()
{
    if (!Node::init())
        return false;

    m_current_bgm_id = AudioEngine::INVALID_AUDIO_ID;
    m_next_bgm_id    = AudioEngine::INVALID_AUDIO_ID;

    return true;
}

void AudioManager::Inject(SettingService* setting_service)
{
    m_setting_service = setting_service;

    m_setting_service->SetBGMVolumeChanged(AX_CALLBACK_1(AudioManager::OnBGMVolumeChanged, this));
    m_setting_service->SetBGMPrintChanged([this](bool) { UpdateBGMVolume(); });
}

void AudioManager::PlayBGM(const std::string& bgm_path, float fade_time)
{
    if (!m_setting_service)
        return;

    StopAllBGM(true);

    if (m_next_bgm_id != AudioEngine::INVALID_AUDIO_ID)
        return;

    m_next_bgm_id = AudioEngine::play2d(bgm_path, true, 0.0f);

    if (m_current_bgm_id != AudioEngine::INVALID_AUDIO_ID)
    {
        AXLOGD("AudioManager: 페이드 교체 진행");
        FadeOutCurrentBGM(fade_time);
    }
    else
    {
        AXLOGD("AudioManager: 단순 교체 진행");
        SwapAndFadeInBGM(fade_time);
    }

}

void AudioManager::PlaySFX(const std::string& sfx_path)
{
    if (!m_setting_service)
        return;

    float volume = m_setting_service->GetEffectiveSFXVolume();
    if (volume <= 0.0f)
        return;

    AudioEngine::play2d(sfx_path, false, volume);
}

void AudioManager::SwapAndFadeInBGM(float duration)
{
    m_current_bgm_id = m_next_bgm_id;
    m_next_bgm_id    = AudioEngine::INVALID_AUDIO_ID;

    FadeInCurrentBGM(duration);
}

void AudioManager::FadeInCurrentBGM(float duration)
{
    if (m_current_bgm_id == AudioEngine::INVALID_AUDIO_ID || !m_setting_service)
        return;

    float target_volume = m_setting_service->GetEffectiveBGMVolume();
    float step = 0.05f;
    float increment = target_volume * step / duration;

    this->schedule([this, increment, target_volume](float dt)
    {
        float volume = AudioEngine::getVolume(m_current_bgm_id);
        volume += increment * dt;

        if (volume >= target_volume)
        {
            AudioEngine::setVolume(m_current_bgm_id, target_volume);
            this->unschedule("bgm_fade_in");
        }
        else
        {
            AudioEngine::setVolume(m_current_bgm_id, volume);
        }
    }, step, "bgm_fade_in");
}

void AudioManager::FadeOutCurrentBGM(float duration)
{
    int fading_id = m_current_bgm_id;

    float step = 0.05f;
    float start_volume = AudioEngine::getVolume(fading_id);
    float decrement = start_volume / duration;

    this->schedule([this, fading_id, decrement, duration](float dt)
    {
        float vol = AudioEngine::getVolume(fading_id);
        vol -= decrement * dt;

        if (vol <= 0.0f)
        {
            AudioEngine::stop(fading_id);
            this->unschedule("bgm_fade_out");

            m_current_bgm_id = AudioEngine::INVALID_AUDIO_ID;

            SwapAndFadeInBGM(duration);
        }
        else
        {
            AudioEngine::setVolume(fading_id, vol);
        }
    }, step, "bgm_fade_out");
}

void AudioManager::StopAllBGM(bool immediately)
{
    this->unschedule("bgm_fade_in");
    this->unschedule("bgm_fade_out");

    if (m_current_bgm_id != AudioEngine::INVALID_AUDIO_ID)
    {
        AudioEngine::stop(m_current_bgm_id);
        m_current_bgm_id = AudioEngine::INVALID_AUDIO_ID;
    }

    if (m_next_bgm_id != AudioEngine::INVALID_AUDIO_ID)
    {
        AudioEngine::stop(m_next_bgm_id);
        m_next_bgm_id = AudioEngine::INVALID_AUDIO_ID;
    }
}

void AudioManager::Schedule(const std::function<void(float)>& callback, float interval, const std::string& key)
{
    Director::getInstance()->getScheduler()->schedule([callback](float dt){callback(dt);}, this, interval, false, key);
}

void AudioManager::Unschedule(const std::string& key)
{
    Director::getInstance()->getScheduler()->unschedule(key, this);
}

void AudioManager::OnBGMVolumeChanged(float volume)
{
    if (!m_setting_service)
        return;

    volume = clampf(volume, 0.0f, 1.0f);

    if (m_current_bgm_id != AudioEngine::INVALID_AUDIO_ID)
        AudioEngine::setVolume(m_current_bgm_id, volume);

    if (m_next_bgm_id != AudioEngine::INVALID_AUDIO_ID)
        AudioEngine::setVolume(m_next_bgm_id, volume);
}

void AudioManager::UpdateBGMVolume()
{
    if (!m_setting_service)
        return;

    float volume = m_setting_service->GetEffectiveBGMVolume();

    if (m_current_bgm_id != AudioEngine::INVALID_AUDIO_ID)
        AudioEngine::setVolume(m_current_bgm_id, volume);

    if (m_next_bgm_id != AudioEngine::INVALID_AUDIO_ID)
        AudioEngine::setVolume(m_next_bgm_id, volume);
}
