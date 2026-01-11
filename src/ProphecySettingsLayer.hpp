#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ProphecySettingsLayer : public geode::Popup<> {
protected:
    CCMenu* m_toggleMenu;
    CCMenu* m_presetMenu;
    
    bool setup() override;
    void createProphecyToggle(const char* key, const char* label, float yPos);
    void createPresetButton(const char* label, std::function<void()> callback, float xPos, float yPos);
    
    // Toggle callback
    void onToggle(CCObject*);
    
    // Preset callbacks
    void onDisableBad(CCObject*);
    void onDisableGood(CCObject*);
    void onEnableAll(CCObject*);
    void onChaosMode(CCObject*);
    
public:
    static ProphecySettingsLayer* create();
    static void show();
};
