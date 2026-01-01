#pragma once

#include "axmol/axmol.h"
#include "axmol/ui/UISlider.h"
#include "SettingService.h"

class SettingUI : public ax::Layer
{
public:
    CREATE_FUNC(SettingUI)
    bool init() override;
    void Inject(SettingService* setting_service);

public:
    void OpenUI();
    void CloseUI();

private:
    void InitUI();
    void ToggleUI(bool active);
    void CreateUI();
    void CreateBlockingLayer();
    void CreateSettingPanel();
    void CreateTitleGroup();
    void CreateToggleGroup();
    void CreateSliderGroup();
    void CreateExitButton();

    void OnClickedBGMPrintToggle(Object* sender);
    void OnClickedSFXPrintToggle(Object* sender);
    void OnBGMVolumeChanged(Object* sender, ax::ui::Slider::EventType type);
    void OnSFXVolumeChanged(Object* sender, ax::ui::Slider::EventType type);
    void OnClickedExit();

private:
    ax::Node* m_setting_panel;
    ax::MenuItemToggle* m_bgm_print_toggle;
    ax::MenuItemToggle* m_sfx_print_toggle;
    ax::ui::Slider* m_bgm_volume_slider;
    ax::ui::Slider* m_sfx_volume_slider;

private:
    ax::Size m_window_size;
    ax::Size m_panel_size;

    SettingService* m_setting_service;

    ax::EventListenerTouchOneByOne* m_block_listener;
    ax::Layer* m_block_layer;
};
